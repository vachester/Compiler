generate : Main
	./Main

Main : Main.o myParser.o myLexer.o
	g++ -o Main Main.o myParser.o myLexer.o

Main.o : Main.cpp myParser.h
	g++ -c Main.cpp

myParser.o : myParser.cpp myParser.h grammer.txt action.out goto.out /root/C++/Compiler/test.txt
	g++ -c myParser.cpp

Lexer.o : /root/C++/Compiler/myLexer/myLexer.cpp
	g++ -c /root/C++/Compiler/myLexer/myLexer.cpp

clean :
	rm Main Main.o myParser.o myLexer.o

