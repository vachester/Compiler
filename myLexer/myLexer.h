#ifndef MYLEXER_H
#define MYLEXER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include "tag.h"


/*
 * 主要是定义基本的词法单元类，
 * 声明了词法分析类
 */

//存储词法单元
class Word {
	public:
		Word() = default;
		Word(std::string s, int t) : lexeme(s), tag(t) {};
		std::string getLexeme() { return lexeme; };
		int getTag() { return tag; }
	private:
		std::string lexeme;
		int tag;
};

//词法分析器类
class Lexer {
	public:
		Lexer();
		void reserve(Word w);
		bool readnext(char c, std::ifstream &in);
		Word scan(std::ifstream &in);
		int getLine() { return line; }
		void run(); //执行
	private:
		char peek;
		std::unordered_map<std::string, Word> words;
		int line;
};


#endif
