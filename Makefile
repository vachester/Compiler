generate : Main
	./Main

Main : Main.o myLexer.o
	g++ -o Main Main.o myLexer.o

Main.o : Main.cpp myLexer/myLexer.h myLexer/tag.h test.c
	g++ -c Main.cpp

myLexer.o : myLexer/myLexer.cpp myLexer/myLexer.h myLexer/tag.h test.c
	g++ -c myLexer/myLexer.cpp

clean :
	rm Main Main.o myLexer.o 
