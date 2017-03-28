#include <iostream>
#include "myParser.h"

Parser::Parser(){
	//初始化非终结符和终结符
	V = {"E", "T", "F", "M", "N"};
	T = {"+" ,"*", "(", ")", "id", "ε"};

	std::unordered_set<std::string> empty_set;
	for(auto i : V) { 
		first.insert({i, empty_set});
		follow.insert({i, empty_set});
	}

	follow["E"].insert("#");
}


void Parser::get_grammer(std::ifstream &in) {
	std::string tmp;
	Grammer gtmp;
	while(in >> tmp) {
		gtmp.left = tmp;
		gtmp.right.clear();
		in >> tmp >> tmp;
		while(tmp != "#") {
			gtmp.right.push_back(tmp);	
			in >> tmp;
		}
		G.push_back(gtmp);
	}
}

bool Parser::inVT(std::string s) {
	for(auto i : V) {
		if(i == s)
			return true;
	}

	return false;
}


void Parser::get_first() {
	bool change = true; //表示若改动了一处，则需要重新便利
	bool is_empty; //表示产生式右端为空串
	int t; 
	//循环，直到没有改动为止，即change = false
	while(change) {
		change = false;
		//循环每个文法
		for(auto &g : G) {
			is_empty = true;
			t = 0;
			while(is_empty && t < static_cast<int>(g.right.size())) {
				is_empty = false;
				if(!inVT(g.right[t])) {
					if(first[g.left].find(g.right[t]) == first[g.left].end()) {
						first[g.left].insert(g.right[t]);
						change = true;
					}
				} else {
					for(auto i : first[g.right[t]]) {
						if(first[g.left].find(i) == first[g.left].end()) {
							first[g.left].insert(i);
							change = true;
						}
					}

					if(first[g.right[t]].find("ε") != first[g.right[t]].end()) {
						is_empty = true;
						t++;
					}
				}		
			}
			if(t == static_cast<int>(g.right.size()) && first[g.left].find("ε") == first[g.left].end()) {
				first[g.left].insert("ε");
				change = true;
			}
		}
	}
}

void Parser::judge_first(std::vector<std::string> s, std::unordered_set<std::string> &result) {
	int count = 0;
	for(auto i : s) {
		if(!inVT(i)) {
			result.insert(i);
			break;
		}
		if(first[i].find("ε") == first[i].end()) {
			result.insert(first[i].begin(),first[i].end());
			break;
		} else {
			result.insert(first[i].begin(), first[i].end());
			result.erase("ε");
			count++;
		}
	}

	if(count == static_cast<int>(s.size()))
		result.insert("ε");
}

void Parser::get_follow() {
	//与求first集类似，设置change
	bool change = true;
	while(change) {
		change = false;
		//循环每个文法
		std::vector<std::string>::iterator search_next; //向后扫描指针
		for(auto &g : G) {
			for(auto it = g.right.begin(); it != g.right.end(); it++) {
				if(inVT(*it)) {
					auto original = follow[*it].size();
					search_next = it + 1;
					if(search_next != g.right.end()) {
						std::vector<std::string> tmp(search_next,g.right.end());	
						std::unordered_set<std::string> stmp;
						judge_first(tmp,stmp);
						follow[*it].insert(stmp.begin(),stmp.end());
						if(stmp.find("ε") != stmp.end()) {
							follow[*it].erase("ε");
							follow[*it].insert(follow[g.left].begin(),follow[g.left].end());
						} 
						if(original < follow[*it].size())
							change = true;
					} else {
						follow[*it].insert(follow[g.left].begin(), follow[g.left].end());
						if(original < follow[*it].size())
							change = true;
					}
				}
			}
		}
	}
}

int main() {
	Parser test;
	std::ifstream in("grammer.txt");
	test.get_grammer(in);
	test.get_first();
	test.get_follow();

	return 0;
}
