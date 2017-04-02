#ifndef MYPARSER_H
#define MYPARSER_H

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstddef>
#include <functional>


struct Project {
	int pro_num;  //表示第pro_num个产生式，0表示拓展的产生式
	int dot_position;  //当前点的位置，位置的取值比产生式右端长度大1
	std::unordered_set<std::string> successors;  //后继符
	bool operator==(Project &right);
};

struct Grammer {
	std::string left;
	std::vector<std::string> right;
};	


/*
 * hash函数返回哈希值
 * eq则为set提供相等操作
 * 这样就可以把Project作为set的关键词
 */


size_t hasher(const Project &p) {
	return std::hash<int>() (p.pro_num*p.dot_position);
}


bool eq(const Project &p1, const Project &p2) {
	if(p1.pro_num == p2.pro_num && p1.dot_position == p2.dot_position) 
		return true;
	return false;
}



class Parser {
	public:
		typedef std::unordered_set<Project, decltype(hasher) *, decltype(eq) *> Status; 
		typedef std::unordered_map<int, std::unordered_map<std::string, std::string>> Atable;
		typedef std::unordered_map<int, std::unordered_map<std::string, int>> Gtable;
		Parser();
		void get_grammer(std::ifstream &in); //从文本中读取文法规则
		void get_first();	//获得first集合
		void get_follow(); //获得follow集合
		bool inVT(std::string s); //判断在V还是T里，1为V
		void judge_first(std::vector<std::string> s , std::unordered_set<std::string> & result); //判断串s的first集合是否包含ε
		void get_closure(Status &p); //求项目闭包
		bool judge_repeat(Status s1, Status s2); //判断两个状态是否重复
		void get_status(); //获得状态集合
	private:
		std::unordered_map<std::string,std::unordered_set<int>> index; //存储各非终结符的定义
		std::vector<Grammer> G;  //存储文法
		std::vector<std::string> V, T; //存储非终结符和终结符
		std::unordered_map<std::string, std::unordered_set<std::string>> first, follow;  //存储first和follow集合
		std::unordered_map<int, Status> statuses; //规范状态集
		Atable action;
		Gtable goTo;

};


#endif
