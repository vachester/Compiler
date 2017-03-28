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

/*
//字符类
class Token {
	public:
		Token() = default;
		Token(int t) : tag(t) {};
		int getTag() { return tag; };
	private:
		int tag;
};

//整数类
class Num : public Token {
	public:
		Num() = default;
		Num(int v) : Token(Tag::NUM), value(v) {};
		int getValue() { return value; };
	private:
		int value;
};

//浮点数类
class Floating : public Token {
	public:
		Floating() = default;
		Floating(float v) : Token(Tag::FLOATING), value(v) {};
		float getValue() { return value; };
	private:
		float value;
};


//布尔常数类
class Boolvalue : public Token {
	public:
		Boolvalue() = default;
		Boolvalue(bool v) : Token(Tag::BOOLVALUE), value(v) {};
		bool getValue() { return value; };
	private:
		bool value;
};


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
	private:
		char peek;
		std::unordered_map<std::string, Word> words;
		int line;
};


#endif
