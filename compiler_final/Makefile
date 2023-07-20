all: bin main

bin: 
	mkdir bin

main: bin/y.tab.c bin/lex.yy.c  src/ExpressionTree.cpp
	g++ bin/y.tab.c bin/lex.yy.c src/ExpressionTree.cpp -o bin/compiler

bin/lex.yy.c: src/compiler.l 
	lex src/compiler.l
	mv lex.yy.c bin/

bin/y.tab.c: src/compiler.y include/ExpressionTree.hpp
	yacc -d src/compiler.y
	mv y.tab.h bin/
	mv y.tab.c bin/

y.tab.h: compiler.y
	yacc -d compiler.y

clean: 
	rm -r bin

run: all
	./bin/compiler 

test: all
	@./bin/compiler < ./tests/bubble.sil
