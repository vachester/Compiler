#include "myParser.h"

Parser::Parser() {
	stack_node pNode;
	pNode.status = 0;
	tmp_count = 0;
	pNode.token.setTag(70);
	pNode.token.setLexeme("#");

	stk.push(pNode);
	run();
}


void Parser::getTable(std::ifstream &i1, std::ifstream &i2) {
	std::string tmp2,atmp3;
	int tmp1,gtmp3;
	while(i1 >> tmp1, i1 >> tmp2, i1 >> atmp3) {
		action[tmp1][tmp2] = atmp3;
	}

	while(i2 >> tmp1, i2 >> tmp2, i2 >> gtmp3) {
		goTo[tmp1][tmp2] = gtmp3;
	}	

}

void Parser::get_grammer(std::ifstream &in) {
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
}


inline
void Parser::gen_code(std::string str) {
	code.push_back(str);
}


void Parser::gen_math_code(std::vector<stack_node> p) {
	std::string left, right, op;	

	left = p[2].nodeInfo.name;
	op = p[1].nodeInfo.name;
	right = p[0].nodeInfo.name;

	code.push_back("t"+std::to_string(tmp_count++)+" = "+left + " "+op+" "+right);


}


void Parser::backpatch(std::unordered_set<int> l, int addr) {
	for(auto t : l)
		code[t] = code[t] + std::to_string(addr);
}


std::unordered_set<int> Parser::merge(std::unordered_set<int> s1, std::unordered_set<int> s2) {
	std::unordered_set<int> result = s1;
	result.insert(s2.begin(),s2.end());
	return result;
}

Symbol Parser::hasKey(std::string str) {
	Symbol result ;
	std::stack<STable *> stack_tmp = Utable;
	STable st_tmp;

	while(!stack_tmp.empty()) {
		st_tmp = *stack_tmp.top();
		if(st_tmp.find(str) != st_tmp.end())
			return st_tmp[str];
		stack_tmp.pop();
	}

	result.n = ERROR;
	return result;
}

bool Parser::stack_parser(std::ifstream &in) {

	in >> std::noskipws;
	Lexer mylex;

	Word w;
	w = mylex.scan(in);

	STable *stable;
	Symbol sym_tmp;
	stack_node stk_top;

	std::string left = "", right = "";

	while(!stk.empty()) {
		stk_top = stk.top();
		int top_status = stk_top.status;
		int next_status;
		std::string act; //将要执行的动作
		std::string T;
		if(w.getTag() == 50) 
			T = "Num";
		else if(w.getTag() == 60) 
			T = "Identifier";
		else
			T = w.getLexeme();

		if(action[top_status].find(T) == action[top_status].end()) 
			return false;
		act = action[top_status][T];
		stack_node tmp;
		if(act[0] == 's') {
			//规定作用域
			if(w.getLexeme() == "{") {
				offset_record[Utable.size()] = 0;
				stable = new STable();
				Utable.push(stable);
			}
			else if(w.getLexeme() == "}") {
				Utable.pop();
				if(!Utable.empty())
					stable = Utable.top();
			}
			//移进动作
			next_status = std::stoi(act.substr(1));
			tmp.status = next_status;
			tmp.token = w;
			tmp.nodeInfo.name = w.getLexeme();
			if(w.getTag() == 50)
				tmp.nodeInfo.val = std::stoi(w.getLexeme());
			stk.push(tmp);
			w = mylex.scan(in);
		} else if(act[0] == 'r') {
			std::vector<stack_node> right_tmp;
			next_status = std::stoi(act.substr(1));
			if(G[next_status].right[0] != "ε") {
				for(int i = 0; i < static_cast<int>(G[next_status].right.size());i++) {
					right_tmp.push_back(stk.top());
					stk.pop();
				}		
			}
			stk_top = stk.top();
			top_status = stk_top.status;
			if(goTo[top_status].find(G[next_status].left) == goTo[top_status].end())
				return false;
			tmp.status = goTo[top_status][G[next_status].left];
			tmp.token.setLexeme(G[next_status].left);
			tmp.token.setTag(next_status);

			//std::cout << next_status << " ";

			switch(next_status) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					tmp.nodeInfo.t = INT;
					tmp.nodeInfo.width = 4;
					break;
				case 3:
					tmp.nodeInfo.t = BOOL;
					tmp.nodeInfo.width = 1;
					break;
				case 4:
					backpatch(right_tmp[4].nodeInfo.nextlist, code.size());
					break;
				case 5:
					tmp.nodeInfo = right_tmp[1].nodeInfo;
					sym_tmp = hasKey(tmp.nodeInfo.name);
					if(stable->find(tmp.nodeInfo.name) == stable->end()) {
						//创建符号表并且初始化为0
						Symbol s_tmp;
						int size_tmp = 1;
						if(tmp.nodeInfo.array_offset > 0) {
							size_tmp *= tmp.nodeInfo.array_offset;
							//向符号表里添加数据
							size_tmp *= right_tmp[2].nodeInfo.width;
							s_tmp.t = ARRAY;
							s_tmp.offset = offset_record[Utable.size()-1];
							s_tmp.n = tmp.nodeInfo.array_offset;
							offset_record[Utable.size()-1] += size_tmp;

							stable->insert({tmp.nodeInfo.name,s_tmp});
						} else {
							size_tmp = right_tmp[2].nodeInfo.width;
							//向符号表添加数据
							s_tmp.t = tmp.nodeInfo.t;
							s_tmp.offset = offset_record[Utable.size()-1];
							offset_record[Utable.size()-1] += size_tmp;

							stable->insert({tmp.nodeInfo.name,s_tmp});
						}
					} else 
						std::cout <<"hh" ;
						;//重复声明,error;

					break;
				case 6:
					backpatch(right_tmp[2].nodeInfo.nextlist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.nextlist = right_tmp[0].nodeInfo.nextlist;
					break;
				case 7:
					tmp.nodeInfo.nextlist = right_tmp[0].nodeInfo.nextlist;
					break;
				case 8:
					tmp.nodeInfo = right_tmp[3].nodeInfo;
					//符号表中不存在，则创建
					sym_tmp = hasKey(tmp.nodeInfo.name);
					if(stable->find(tmp.nodeInfo.name) == stable->end() && stk_top.token.getLexeme() == "Type") 
						tmp.nodeInfo.array_offset = right_tmp[1].nodeInfo.val;
					else {
						if(sym_tmp.t == ARRAY) {
							std::string code_tmp;
							code_tmp = "t" + std::to_string(tmp_count++) + " = " + std::to_string(sym_tmp.offset);
							gen_code(code_tmp);
							code_tmp= "t" + std::to_string(tmp_count++) + " = ";
							code_tmp = code_tmp + "t"+std::to_string(tmp_count-2)+"["+ std::to_string(right_tmp[1].nodeInfo.val)+"]";
							gen_code(code_tmp);
							tmp.nodeInfo.name = "t"+std::to_string(tmp_count - 1);
						}
						else
							;//普通变量引用数组
					}
					break;
				case 9:
					tmp.nodeInfo = right_tmp[3].nodeInfo;
					sym_tmp = hasKey(tmp.nodeInfo.name);
					if(sym_tmp.n >= 0) {
						if(sym_tmp.t == ARRAY){
							Symbol sym_tmpp = hasKey(right_tmp[1].nodeInfo.name);
							if(sym_tmpp.n >= 0) {
								std::string code_tmp;
								code_tmp = "t" + std::to_string(tmp_count++) + " = " + std::to_string(sym_tmp.offset);
								gen_code(code_tmp);
								code_tmp = "t" + std::to_string(tmp_count++) + " = ";
								code_tmp = code_tmp + "t"+std::to_string(tmp_count-2)+"["+right_tmp[1].nodeInfo.name+"]";
								gen_code(code_tmp);
								tmp.nodeInfo.name = "t"+std::to_string(tmp_count-1);
							}
							else
								;//error
						}		
						else
							;//普通变量引用数组
					} else
						;//声明数组用变量
					break;
				case 10:
					tmp.nodeInfo.name = right_tmp[0].nodeInfo.name;
					break;
				case 11:
					tmp.nodeInfo.name = "1";
					tmp.nodeInfo.truelist.insert(code.size());
					break;
				case 12:
					tmp.nodeInfo.name = "0";
					tmp.nodeInfo.falselist.insert(code.size());
					break;
				case 13:
					//计算左右边的值
					if(right_tmp[2].nodeInfo.other == "") 
						left = right_tmp[2].nodeInfo.name;
					else 
						left = right_tmp[2].nodeInfo.name +"["+right_tmp[2].nodeInfo.other+"]";

					right = right_tmp[0].nodeInfo.name;

					gen_code(left+" = "+right);

					break;
				case 14:
					tmp.nodeInfo = right_tmp[1].nodeInfo;
					if(tmp.nodeInfo.other != "") {
						std::string code_tmp;
						code_tmp="t"+std::to_string(tmp_count++)+" = ";
						left = tmp.nodeInfo.name+"["+tmp.nodeInfo.other+"]";
						code_tmp += left;
						gen_code(code_tmp);
						right = "t"+std::to_string(tmp_count-1);
						code_tmp = right +" = "+right+" "+right_tmp[0].nodeInfo.name + " 1";
						gen_code(code_tmp);
						gen_code(left+" = "+ right);
					} else 
						gen_code(tmp.nodeInfo.name + " = "+tmp.nodeInfo.name+" "+right_tmp[0].nodeInfo.name+" 1");
					break;
				case 15:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					if(tmp.nodeInfo.other != "") {
						std::string code_tmp;
						code_tmp="t"+std::to_string(tmp_count++)+" = ";
						left = tmp.nodeInfo.name+"["+tmp.nodeInfo.other+"]";
						code_tmp += left;
						gen_code(code_tmp);
						right = "t"+std::to_string(tmp_count-1);
						code_tmp = right +" = "+right+" "+right_tmp[1].nodeInfo.name + " 1";
						gen_code(code_tmp);
						gen_code(left+" = "+ right);
					} else 
						gen_code(tmp.nodeInfo.name + " = "+tmp.nodeInfo.name+" "+right_tmp[1].nodeInfo.name+" 1");
					break;
				case 16:
					backpatch(right_tmp[3].nodeInfo.falselist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.falselist = right_tmp[0].nodeInfo.falselist;
					tmp.nodeInfo.truelist = merge(right_tmp[0].nodeInfo.truelist,right_tmp[3].nodeInfo.truelist);
					break;
				case 17:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 18:
					backpatch(right_tmp[3].nodeInfo.truelist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.truelist = right_tmp[0].nodeInfo.truelist;
					tmp.nodeInfo.falselist = merge(right_tmp[0].nodeInfo.falselist,right_tmp[3].nodeInfo.falselist);
					break;
				case 19:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 20:
					tmp.nodeInfo.truelist.insert(code.size());
					tmp.nodeInfo.falselist.insert(code.size()+1);
					gen_code("if "+right_tmp[2].nodeInfo.name+" "+right_tmp[1].nodeInfo.name+" "+right_tmp[0].nodeInfo.name+" goto ");
					gen_code("goto ");
					break;
				case 21:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 22:
					gen_math_code(right_tmp);
					tmp.nodeInfo.name = "t"+std::to_string(tmp_count-1);
					break;
				case 23:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 24:
					gen_math_code(right_tmp);
					tmp.nodeInfo.name = "t"+std::to_string(tmp_count-1);
					break;
				case 25:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 26:
					tmp.nodeInfo.truelist = right_tmp[1].nodeInfo.falselist;
					tmp.nodeInfo.falselist = right_tmp[1].nodeInfo.truelist;
					break;
				case 27:
					tmp.nodeInfo = right_tmp[1].nodeInfo;
					break;
				case 28:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 29:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					tmp.nodeInfo.name = right_tmp[0].token.getLexeme();
					break;
				case 30:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 31:
					tmp.nodeInfo.name = "+";
					break;
				case 32:
					tmp.nodeInfo.name = "-";
					break;
				case 33:
					break;
				case 34:
					break;
				case 35:
					tmp.nodeInfo.name = "+";
					break;
				case 36:
					tmp.nodeInfo.name = "-";
					break;
				case 37:
					tmp.nodeInfo.name = "*";
					break;
				case 38:
					tmp.nodeInfo.name = "/";
					break;
				case 39:
					tmp.nodeInfo.name = "%";
					break;
				case 40:
					tmp.nodeInfo.name = "==";
					break;
				case 41:
					tmp.nodeInfo.name = "!=";
					break;
				case 42:
					tmp.nodeInfo.name = ">=";
					break;
				case 43:
					tmp.nodeInfo.name = "<=";
					break;
				case 44:
					tmp.nodeInfo.name = ">";
					break;;
				case 45:
					tmp.nodeInfo.name = "<";
					break;
				case 46:
					break;
				case 47:
					backpatch(right_tmp[3].nodeInfo.truelist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.nextlist = merge(right_tmp[3].nodeInfo.falselist,right_tmp[0].nodeInfo.nextlist);
					break;
				case 48:
					backpatch(right_tmp[7].nodeInfo.truelist,right_tmp[5].nodeInfo.quad);
					backpatch(right_tmp[7].nodeInfo.falselist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.nextlist = merge(right_tmp[4].nodeInfo.nextlist,merge(right_tmp[3].nodeInfo.nextlist,right_tmp[0].nodeInfo.nextlist));
					break;
				case 49:
					break;
				case 50:
					backpatch(right_tmp[0].nodeInfo.nextlist,right_tmp[5].nodeInfo.quad);
					backpatch(right_tmp[3].nodeInfo.truelist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.nextlist = right_tmp[3].nodeInfo.falselist;
					gen_code("goto "+std::to_string(right_tmp[5].nodeInfo.quad));
					break;
				case 51:
					backpatch(right_tmp[0].nodeInfo.nextlist,right_tmp[6].nodeInfo.quad);
					backpatch(right_tmp[5].nodeInfo.truelist,right_tmp[1].nodeInfo.quad);
					tmp.nodeInfo.nextlist = right_tmp[5].nodeInfo.falselist;
					for(auto cd : right_tmp[3].nodeInfo.code)
						gen_code(cd);
					gen_code("goto "+std::to_string(right_tmp[6].nodeInfo.quad));
					break;
				case 52:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 53:
					break;
				case 54:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					break;
				case 55:
					break;
				case 56:
					tmp.nodeInfo = right_tmp[1].nodeInfo;
					break;
				case 57:
					tmp.nodeInfo = right_tmp[3].nodeInfo;
					//符号表中不存在，则创建
					sym_tmp = hasKey(tmp.nodeInfo.name);

					if(sym_tmp.t == ARRAY) {
						std::string code_tmp;
						code_tmp = "t" + std::to_string(tmp_count++) + " = " + std::to_string(sym_tmp.offset);
						gen_code(code_tmp);
						tmp.nodeInfo.other = std::to_string(right_tmp[1].nodeInfo.val);
						tmp.nodeInfo.name = "t"+std::to_string(tmp_count-1);
					}
					else
						;//普通变量引用数组
					break;
				case 58:
					tmp.nodeInfo = right_tmp[3].nodeInfo;
					sym_tmp = hasKey(tmp.nodeInfo.name);
					if(sym_tmp.t == ARRAY){
						Symbol sym_tmpp = hasKey(right_tmp[1].nodeInfo.name);
						if(sym_tmpp.n >= 0) {
							std::string code_tmp;
							code_tmp = "t" + std::to_string(tmp_count++) + " = " + std::to_string(sym_tmp.offset);
							gen_code(code_tmp);
							tmp.nodeInfo.other = right_tmp[1].nodeInfo.name;
							tmp.nodeInfo.name = "t"+std::to_string(tmp_count-1);
						}
						else
							;//error
					} else
						;//声明数组用变量
					break;
				case 59:
					tmp.nodeInfo.name = right_tmp[0].nodeInfo.name;
					break;
				case 60:
					tmp.nodeInfo.quad = code.size();
					break;
				case 61:
					tmp.nodeInfo.nextlist.insert(code.size());
					gen_code("goto ");
					break;
				case 62:
					//计算左右边的值
					if(right_tmp[2].nodeInfo.other == "") 
						left = right_tmp[2].nodeInfo.name;
					else 
						left = right_tmp[2].nodeInfo.name +"["+right_tmp[2].nodeInfo.other+"]";

					right = right_tmp[0].nodeInfo.name;

					tmp.nodeInfo.code.push_back(left+" = "+right);

					break;
				case 63:
					tmp.nodeInfo = right_tmp[1].nodeInfo;
					if(tmp.nodeInfo.other != "") {
						std::string code_tmp;
						code_tmp="t"+std::to_string(tmp_count++)+" = ";
						left = tmp.nodeInfo.name+"["+tmp.nodeInfo.other+"]";
						code_tmp += left;
						tmp.nodeInfo.code.push_back(code_tmp);
						right = "t"+std::to_string(tmp_count-1);
						code_tmp = right +" = "+right+" "+right_tmp[0].nodeInfo.name + " 1";
						tmp.nodeInfo.code.push_back(code_tmp);
						tmp.nodeInfo.code.push_back(left+" = "+right);
					} else 
						tmp.nodeInfo.code.push_back(tmp.nodeInfo.name + " = "+tmp.nodeInfo.name+" "+right_tmp[0].nodeInfo.name+" 1");
					break;
				case 64:
					tmp.nodeInfo = right_tmp[0].nodeInfo;
					if(tmp.nodeInfo.other != "") {
						std::string code_tmp;
						code_tmp="t"+std::to_string(tmp_count++)+" = ";
						left = tmp.nodeInfo.name+"["+tmp.nodeInfo.other+"]";
						code_tmp += left;
						tmp.nodeInfo.code.push_back(code_tmp);
						right = "t"+std::to_string(tmp_count-1);
						code_tmp = right +" = "+right+" "+right_tmp[1].nodeInfo.name + " 1";
						tmp.nodeInfo.code.push_back(code_tmp);
						tmp.nodeInfo.code.push_back(left+" = "+right);
					} else 
						tmp.nodeInfo.code.push_back(tmp.nodeInfo.name + " = "+tmp.nodeInfo.name+" "+right_tmp[1].nodeInfo.name+" 1");
					break;
				case 65:
					break;
				case 66:
					break;
				default: 
					;
			}
			stk.push(tmp);
		} else 
			return true;
		if(in.eof()) {
			w.setLexeme("#");
			w.setTag(0);
		}
	}
	return false;
}



void Parser::print(std::ofstream &out) {
	for(int i = 0; i < static_cast<int>(code.size());i++) {
		out << i << " : " << code[i] << std::endl;
	}	
}


void Parser::run() {

	std::ifstream gin("grammer.txt");
	std::ifstream lin("/root/C++/Compiler/test.txt");
	std::ifstream action_in("action.out");
	std::ifstream goto_in("goto.out");
	std::ofstream parser_out("/root/C++/Compiler/parser.out");




	getTable(action_in,goto_in);
	get_grammer(gin);	
	if(stack_parser(lin))
		print(parser_out);
	gin.close();
	lin.close();
	action_in.close();
	goto_in.close();

}
