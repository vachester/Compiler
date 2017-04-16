#ifndef TAG_H
#define TAG_H

namespace Tag {
	//保留字
	const int
		INT = 1, BOOL = 2, MAIN = 3, IF = 4,
		ELSE = 5, FOR = 6, WHILE = 7, FALSE = 8,
		BREAK = 9, RETURN = 10, TRUE = 11 ;	

	//运算符
	const int
		NOT = 20, NE = 21, AUTOMINUS =22, MINUS = 23,
		AUTOADD = 24, ADD = 25, OR = 26, 
		AND = 27, MUTIPLY = 28, DIVIDE = 29, MOD = 30,
		EQ = 31, ASSIN = 32, GE = 33, GT = 34,
		LE = 35, LS = 36;

	//分界符
	const int 
		COMMA = 40, SEMICOLON = 41, LLBRACKET = 42,
		RLBRACKET = 43, LMBRACKET = 44, RMBRACKET = 45,
		LGBRACKET = 46, RGBRACKET = 47;

	//整数常数
	const int NUM = 50;

	//标识符
	const int ID = 60;

	//错误
	const int ERROR = 404;

	//空
	const int  EMPTY = 70;

}

#endif
