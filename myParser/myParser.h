#ifndef MYPARSER_H
#define MYPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>



struct Grammer {
	std::string left;
	std::vector<std::string> right;
};	

class Parser {
	public:
		typedef std::unordered_map<int, std::unordered_map<std::string, std::string>> Atable;
		typedef std::unordered_map<int, std::unordered_map<std::string, int>> Gtable;

		Parser();
		void getTable(std::ifstream &i1, std::ifstream &i2);
		void get_grammer(std::ifstream &in);
		bool stack_parser(std::ifstream &in); 
		void print(std::ofstream &out);
		void run();

	private:
		std::stack<int> status_stack,line_stack;
		std::stack<std::string> Vstack, token_stack;
		std::vector<int> parser_result, parser_line;	
		Atable action;
		Gtable goTo;
		std::vector<Grammer> G;
};








#endif
