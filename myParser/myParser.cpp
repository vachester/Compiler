#include "myParser.h"

Parser::Parser() {
	Vstack.push("#");
	status_stack.push(0);
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
	gtmp.right = {"Bool"};
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

bool Parser::stack_parser(std::ifstream &in) {
	std::string token, topV, act;
	int top_status, next_status, line;
	bool flag;
	std::stack<std::string> stmp;
	std::stack<int> itmp;
	while(in >> line, in >> token) {
		itmp.push(line);
		stmp.push(token);
	}

	line = itmp.top();
	itmp.push(line + 1);	
	stmp.push("#");

	while(!itmp.empty()) {
		line = itmp.top();
		token = stmp.top();
		itmp.pop();
		stmp.pop();
		line_stack.push(line);
		token_stack.push(token);
	}

	while(!token_stack.empty()) {
		flag = false;
		token = token_stack.top();
		line = line_stack.top();
		topV = Vstack.top();		
		top_status = status_stack.top();
		if(action[top_status].find(token) == action[top_status].end()) 
			return false; //这里要错误处理
		act = action[top_status][token];

		if(act[0] == 's') {
			//采用移进动作
			next_status = std::stoi(act.substr(1));
			Vstack.push(token);
			status_stack.push(next_status);	
			token_stack.pop();
			line_stack.pop();
		} else if(act == "acc") {
			flag = true;
			token_stack.pop();
			line_stack.pop();
		}
		else {
			//归约动作
			next_status = std::stoi(act.substr(1));
			parser_result.push_back(next_status);
			parser_line.push_back(line);
			for(int i = 0; i < static_cast<int>(G[next_status].right.size());i++) {
				Vstack.pop();
				status_stack.pop();
			}		

			Vstack.push(G[next_status].left);
			topV = Vstack.top();
			top_status = status_stack.top();
			if(goTo[top_status].find(topV) == goTo[top_status].end()) 
				return false;
			next_status = goTo[top_status][topV];
			status_stack.push(next_status);
		}
	}
	return flag;
}



void Parser::print(std::ofstream &out) {
	for(int i = 0; i < static_cast<int>(parser_result.size());i++) {
		out << G[parser_result[i]].left << " ->";
		for(auto r : G[parser_result[i]].right) 
			out << " " << r;
		out << std::endl;
	}	
}


void Parser::run() {

	std::ifstream gin("grammer.txt");
	std::ifstream lin("/root/C++/Compiler/lexer.out");
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

