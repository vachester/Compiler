#include <iostream>
#include <cctype>
#include <sstream>
#include "myLexer.h"

void Lexer::reserve(Word w) {
	words.insert({w.getLexeme(), w});
}

Lexer::Lexer() {
	//存入保留字，为了区分标识符
	reserve( Word("int", Tag::INT) );
	reserve( Word("bool", Tag::BOOL) );
	reserve( Word("main", Tag::MAIN) );
	reserve( Word("if", Tag::IF) );
	reserve( Word("else", Tag::ELSE) );
	reserve( Word("for", Tag::FOR) );
	reserve( Word("while", Tag::WHILE) );
	reserve( Word("break", Tag::BREAK) );
	reserve( Word("return", Tag::RETURN) );
	reserve( Word("true", Tag::TRUE) );
	reserve( Word("false", Tag::FALSE) );
	
	peek = ' ';
	line = 1;

}

//方便处理像>=,++等这些两个字符连在一起的运算符
bool Lexer::readnext(char c, std::ifstream &in) {
	in >> peek;
	if( peek != c)
		return false;
	peek = ' ';
	return true;
}


Word Lexer::scan(std::ifstream &in) {
	//跳过空白符
	while(!in.eof()) {
		if(peek == ' ' || peek == '\t') {
			in >> peek;
			continue;
		}
		else if(peek == '\n')
			++line;
		else
			break;
		in >> peek;
	}

	//处理分界符、运算符等
	switch(peek) {
		case '!':
			if(readnext('=', in))
				return Word("!=", Tag::NE);
			else
				return Word("!", Tag::NOT);
		case '-':
			if(readnext('-', in))
				return Word("--", Tag::AUTOMINUS);
			else
				return Word("-", Tag::MINUS);
		case '+':
			if(readnext('+', in)) 
				return Word("++", Tag::AUTOADD);
			else
				return Word("+", Tag::ADD);
		case '|':
			if(readnext('|', in)) 
				return Word("||", Tag::OR);
			else
				return Word("error", Tag::ERROR);
		case '&':
			if(readnext('&', in))
				return Word("&&", Tag::AND);
			else
				return Word("error", Tag::ERROR);
		case '*':
			in >> peek;
			return Word("*", Tag::MUTIPLY);
		case '/':
			in >> peek;
			return Word("/", Tag::DIVIDE);
		case '%':
			in >> peek;
			return Word("%", Tag::MOD);
		case '=':
			if(readnext('=', in))
				return Word("==", Tag::EQ);
			else
				return Word("=", Tag::ASSIN);
		case '>':
			if(readnext('=', in))
				return Word(">=", Tag::GE);
			else
				return Word(">", Tag::GT);
		case '<':
			if(readnext('=', in))
				return Word("<=", Tag::LE);
			else
				return Word("<", Tag::LS);
		case ',':
			in >> peek;
			return Word(",", Tag::COMMA);
		case ';':
			in >> peek;
			return Word(";", Tag::SEMICOLON);
		case '(':
			in >> peek;
			return Word("(", Tag::LLBRACKET);
		case ')':
			in >> peek;
			return Word(")", Tag::RLBRACKET);
		case '[':
			in >> peek;
			return Word("[", Tag::LMBRACKET);
		case ']':
			in >> peek;
			return Word("]", Tag::RMBRACKET);
		case '{':
			in >> peek;
			return Word("{", Tag::LGBRACKET);
		case '}':
			in >> peek;
			return Word("}", Tag::RGBRACKET);
	}
	
	//处理常数
	if(isdigit(peek)) {
		int v = 0;
		do {
			v = 10*v + peek - 48;
			in >> peek;
		} while(isdigit(peek));
		if(peek != '.')
			return Word(std::to_string(v), Tag::NUM);
	}	


	//处理标识符
	if(isalpha(peek)) {
		std::ostringstream b;		
		do {
			b << peek;
			in >> peek;
		} while(isalnum(peek) || peek == '_');

		std::string tmp = b.str();

		//判断是否为保留字
		if(words.find(tmp) != words.end()) 
			return words[tmp];
		else
			return Word(tmp, Tag::ID);
	}
	if(peek != ' ' && peek != '\t' && peek != '\n')	
		return Word("error", Tag::ERROR);
	return Word("empty", Tag::EMPTY);
}


