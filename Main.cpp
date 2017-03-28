#include <iostream>
#include "myLexer/myLexer.h"

int main() {

	//读入测试文件
	std::ifstream in("test.c");
	std::ofstream out("test.out");

	//设置不省略空白符
	in >> std::noskipws;

	Lexer lex;

	//输出词法单元
	Word w;
	w = lex.scan(in);
	while(!in.eof() && w.getTag() != Tag::ERROR) {
		int line = lex.getLine();
		out << line << "\t" << "< " << w.getTag() << ", " << w.getLexeme() << " >" << std::endl;
		w = lex.scan(in);
	}

	if(w.getTag() == Tag::ERROR)
		out << "ERROR!" << std::endl;

	in.close();
	out.close();
	return 0;
}
