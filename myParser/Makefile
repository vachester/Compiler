generate : Main
	./Main

Main : Main.o myParser.o
	g++ -o Main Main.o myParser.o

Main.o : Main.cpp myParser.h
	g++ -c Main.cpp

myParser.o : myParser.cpp myParser.h grammer.txt action.out goto.out /root/C++/Compiler/lexer.out
	g++ -c myParser.cpp

clean :
	rm Main Main.o myParser.o

