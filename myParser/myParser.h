#ifndef MYPARSER_H
#define MYPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include "/root/C++/Compiler/myLexer/myLexer.h"


#define MAX 1000
#define MAX_LAY 10
#define ERROR -1000

struct Grammer {
	std::string left;
	std::vector<std::string> right;
};	

enum TYPE
{
	INT,
	BOOL,
	ARRAY
};


struct Info {
	std::string name = ""; //变量名或者运算符
	int quad;
	int val;
	int width; //不同类型的所占偏移
	TYPE t;
	std::unordered_set<int> truelist, falselist, nextlist;
	int array_offset = 0;
	std::vector<std::string> code;
	std::string other = ""; //辅助
};


struct stack_node {
	int status;
	Word token;
	Info nodeInfo;
};


//符号表表项
struct Symbol {
	TYPE t;
	int n = 0 ;
	int offset; //该作用域下的偏移
};


class Parser {
	public:
		typedef std::unordered_map<int, std::unordered_map<std::string, std::string>> Atable;
		typedef std::unordered_map<int, std::unordered_map<std::string, int>> Gtable;
		typedef std::unordered_map<std::string, Symbol> STable;

		Parser();
		void getTable(std::ifstream &i1, std::ifstream &i2);
		void get_grammer(std::ifstream &in);
		bool stack_parser(std::ifstream &in); 
		void print(std::ofstream &out);
		void run();
		Symbol hasKey(std::string str); //判断是否在符号表存在
		void gen_code(std::string str);  //生成代码
		void gen_math_code(std::vector<stack_node> p); //在数学运算的时候产生代码
		void backpatch(std::unordered_set<int> l, int addr);  //回填
		std::unordered_set<int> merge(std::unordered_set<int> s1, std::unordered_set<int> s2); //合并

	private:
		std::stack<stack_node> stk;
		std::vector<int> parser_result, parser_line;	
		Atable action;
		Gtable goTo;
		std::vector<Grammer> G;
		std::vector<std::string> code; //三地址码
		std::stack<STable *> Utable; //符号表
		int offset_record[MAX_LAY] = {0};
		int tmp_count; //临时变量的下标
};




#endif
