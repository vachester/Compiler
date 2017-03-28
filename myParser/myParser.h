#ifndef MYPARSER_H
#define MYPARSER_H

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>


struct Grammer {
	std::string left;
	std::vector<std::string> right;
};	

class Parser {
	public:
		Parser();
		void get_grammer(std::ifstream &in); //从文本中读取文法规则
		void get_first();	//获得first集合
		void get_follow(); //获得follow集合
		bool inVT(std::string s); //判断在V还是T里，1为V
		void judge_first(std::vector<std::string> s , std::unordered_set<std::string> & result); //判断串s的first集合是否包含ε
	private:
		std::vector<Grammer> G;  //存储文法
		std::vector<std::string> V, T; //存储非终结符和终结符
		std::unordered_map<std::string, std::unordered_set<std::string>> first, follow;  //存储first和follow集合

};




#endif
