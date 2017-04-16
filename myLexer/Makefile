generate : Main
	./Main

Main : Main.o myLexer.o
	g++ -o Main Main.o myLexer.o

Main.o : Main.cpp tag.h myLexer.h
	g++ -c Main.cpp

myLexer.o : myLexer.h myLexer.cpp tag.h /root/C++/Compiler/test.c
	g++ -c myLexer.cpp

clean :
	rm Main Main.o myLexer.o
