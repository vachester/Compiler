#include <iostream>
#include "getTable.h"


bool Project::operator==(Project &right) {
	if(this->pro_num == right.pro_num && this->dot_position == right.dot_position && this->successors.size() == right.successors.size()) {
		for(auto t : right.successors) {
			if(this->successors.find(t) == this->successors.end())
				return false;
		}

		return true;
	}

	return false;
}


size_t hasher(const Project &p) {
	return std::hash<int>() (p.pro_num*p.dot_position);
}


bool eq(const Project &p1, const Project &p2) {
	if(p1.pro_num == p2.pro_num && p1.dot_position == p2.dot_position) 
		return true;
	return false;
}


GetTable::GetTable(){
	//初始化非终结符和终结符
	V = {"S","Program","Type","Block","Stmts","Decl","Stmt","ForAssignment","Assignment","Bool", "Rel", "LExpr","HExpr","Factor","Self_op","HLogic_op","LLogic_op","HMath_op","LMath_op","Judge_op","Bool_value","Array","Fora","Forb","Forc","HRel","LArray","M","N"};

	T = {"(" ,")","main","int","bool","return",";","{","}","if","else","while","for","Identifier","Num","[","]","true","false","==","!=",">=","<=",">","<","+","-","*","/","%","||","&&","++","--","!","-",";","=","ε"};

	std::unordered_set<std::string> empty_set;
	std::unordered_set<int> empty_set_int;

	//初始化first和follow集
	for(auto i : V) { 
		first.insert({i, empty_set});
		follow.insert({i, empty_set});
		index.insert({i,empty_set_int});
	}


	follow["Program"].insert("#");
}


void GetTable::get_grammer(std::ifstream &in) {
	std::string tmp;
	Grammer gtmp;
	gtmp.left = "S";
	gtmp.right = {"Program"};
	G.push_back(gtmp);

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
	
	for(int i = 0; i < static_cast<int>(G.size()); i++) 
		index[G[i].left].insert(i);	
}

bool GetTable::inVT(std::string s) {
	for(auto i : V) {
		if(i == s)
			return true;
	}

	return false;
}


void GetTable::get_first() {
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
					continue;
				}  
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
			if(t == static_cast<int>(g.right.size()) && first[g.left].find("ε") == first[g.left].end()) {
				first[g.left].insert("ε");
				change = true;
			}
		}
	}

	first.erase("S");
	
}

void GetTable::judge_first(std::vector<std::string> s, std::unordered_set<std::string> &result) {
	int count = 0;
	for(auto i : s) {
		if(!inVT(i)) {
			result.insert(i);
			break;
		}
		if(first[i].find("ε") == first[i].end()) {
			result.insert(first[i].begin(),first[i].end());
			break;
		} 

		result.insert(first[i].begin(), first[i].end());
		result.erase("ε");
		count++;
	
	}

	if(count == static_cast<int>(s.size()))
		result.insert("ε");
}

void GetTable::get_follow() {
	//与求first集类似，设置change
	bool change = true;
	while(change) {
		change = false;
		//循环每个文法
		std::vector<std::string>::iterator search_next; //向后扫描指针
		for(auto &g : G) {
			for(auto it = g.right.begin(); it != g.right.end(); it++) {
				if(!inVT(*it)) 
					continue;
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

	follow.erase("S");
}

void GetTable::get_closure(Status &p) {
	bool change = true;
	Status pptmp;
	while(change) {
		change = false;
		pptmp = p;
		for(auto &pro : pptmp) {
			if(pro.dot_position == static_cast<int>(G[pro.pro_num].right.size()))
				continue;
			std::string V = G[pro.pro_num].right[pro.dot_position];
			if(!inVT(V)) 
				continue;
			std::unordered_set<std::string> new_successor;

			//求新项目的后继符集
			if(pro.dot_position == static_cast<int>(G[pro.pro_num].right.size()) - 1)
				new_successor = pro.successors;
			else {
				std::vector<std::string> vtmp(G[pro.pro_num].right.begin()+pro.dot_position+1,G[pro.pro_num].right.end()); 
				judge_first(vtmp, new_successor);
				if(new_successor.find("ε") != new_successor.end()) {
					new_successor.insert(pro.successors.begin(),pro.successors.end());
					new_successor.erase("ε");
				}
			}
			Project ptmp;
			for(auto &i : index[V]) {
				ptmp.pro_num = i;
				ptmp.dot_position = 0;
				ptmp.successors = new_successor;
				if(p.find(ptmp) == p.end()) {
					p.insert(ptmp);
					change = true;
				}
				else {
					auto original = p.find(ptmp)->successors.size(); 
					ptmp.successors.insert(p.find(ptmp)->successors.begin(),p.find(ptmp)->successors.end());
					p.erase(*p.find(ptmp));
					p.insert(ptmp);

					if(original < ptmp.successors.size()) 
						change = true;
				}	
			}
		}
	}
}

bool GetTable::judge_repeat(Status s1, Status s2) {
	if(s1.size() == s2.size()) {
		for(auto pros : s1) {
			if(s2.find(pros) == s2.end())
				return false;	
			else {
				Project tmp = *(s2.find(pros));
				if(!(tmp == pros))
					return false;
			}
		}
		return true;
	}
	return false;
}


bool GetTable::judge_conflict(Status & s, std::unordered_set<std::string> &result) {
	bool flag = false;
	std::unordered_set<std::string> tmp;
	for(auto pro : s) {
		if(pro.dot_position == static_cast<int>(G[pro.pro_num].right.size())) 
			tmp.insert(pro.successors.begin(),pro.successors.end());
	}

	for(auto pro : s) {
		if(pro.dot_position < static_cast<int>(G[pro.pro_num].right.size())) {
			std::string next = G[pro.pro_num].right[pro.dot_position];
			if(tmp.find(next) != tmp.end()) {
				result.insert(next);
				flag = true;
			}
		}
	}

	return flag;

}
	

void GetTable::get_status() {
	//初始化第一个状态0
	int t = 0;
	Project ptmp;
	ptmp.dot_position = 0;
	ptmp.pro_num = 0;
	ptmp.successors = {"#"};
	Status tmp_status(10,hasher,eq);
	tmp_status.insert(ptmp);
	get_closure(tmp_status);

	statuses.insert({t,tmp_status});

	//获取状态集合
	bool change = true;
	std::unordered_set<int> record; //记录已经求过的状态
	std::unordered_map<int, Status> sstmp;
	std::unordered_set<std::string> conflict;

	while(change) {
		change = false;
		sstmp = statuses;
		
		for(auto &sta : sstmp) {
			if(record.find(sta.first) != record.end()) 
				continue;
			record.insert(sta.first);
			//judge_conflict(sta.second,conflict);
			/*
			 * 开始球状态转移，用一个set<string>记录被转移过的状态
			 */
			std::unordered_set<std::string> record_status;
			for(auto pros : sta.second) {
				if(G[pros.pro_num].right[0] == "ε" || pros.dot_position == static_cast<int>(G[pros.pro_num].right.size())) {
					for(auto sucess : pros.successors) {  
						if(action[sta.first].find(sucess) == action[sta.first].end())
						action[sta.first][sucess] = "r" + std::to_string(pros.pro_num);
					}
					continue;
				}
				std::string trans = G[pros.pro_num].right[pros.dot_position];
				if(record_status.find(trans) != record_status.end()) 
					continue;
				record_status.insert(trans);
				tmp_status.clear();
				ptmp.pro_num = pros.pro_num;
				ptmp.dot_position = pros.dot_position + 1;
				ptmp.successors = pros.successors;
				tmp_status.insert(ptmp);
				for(auto protmp : sta.second) {
					if(G[protmp.pro_num].right[protmp.dot_position] == trans && !(protmp == pros)) {
						ptmp.pro_num = protmp.pro_num;
						ptmp.dot_position = protmp.dot_position + 1;
						ptmp.successors = protmp.successors;
						tmp_status.insert(ptmp);
					}
				}
				get_closure(tmp_status);
				bool flag = true;
				for(auto s : sstmp) {
					if(judge_repeat(s.second,tmp_status)) {
						if(inVT(trans))
							goTo[sta.first][trans] = s.first;
						else
							action[sta.first][trans] = "s" + std::to_string(s.first);
						flag = false;
						break;
					}							
				}
				if(!flag) 
					continue;
				statuses.insert({++t, tmp_status});
				change = true;

				//填写goTo表和action表
				if(inVT(trans)) 
					goTo[sta.first][trans] = t;
				else 
					action[sta.first][trans] = "s" + std::to_string(t);
			}
		}
	}
	//添加acc
	action[goTo[0]["Program"]]["#"] = "acc";

}


void GetTable::print(std::ofstream &o1, std::ofstream &o2) {
	for(auto f1 : action) {
		for(auto f2 : f1.second)
			o1 << f1.first << " " << f2.first << " " << f2.second << std::endl;
	}

	for(auto f1 : goTo) {
		for(auto f2 : f1.second)
			o2 << f1.first << " " << f2.first << " " << f2.second << std::endl;
	}
}


int main() {
	GetTable test;
	std::ifstream gin("grammer.txt");
	std::ifstream lin("lexer.out");
	std::ofstream action("action.out");
	std::ofstream goTo("goto.out");
	test.get_grammer(gin);
	test.get_first();
	test.get_follow();
	test.get_status();
	test.print(action,goTo);



	gin.close();
	lin.close();
	action.close();
	goTo.close();
	return 0;
}

