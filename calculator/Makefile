all: main

main: y.tab.c lex.yy.c  ExpressionTree.c
	gcc y.tab.c lex.yy.c ExpressionTree.c

lex.yy.c: calculator.l 
	lex calculator.l

y.tab.c: calculator.y ExpressionTree.h
	yacc -d calculator.y

y.tab.h: calculator.y
	yacc -d calculator.y

clean: 
	rm -rf a.out lex.yy.c y.tab.c y.tab.h

run: all
	./a.out
