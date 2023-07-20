%{
#include<stdio.h>
#include "ExpressionTree.h"
%}

%left	'+' '-'	  /* left associative, same precedence */
%left	'*' '/'	  /* left assoc., higher precedence */

%union {
    struct node * tree;
    double value;
}

%token <value> NUMBER
%type <tree> expr 

%%   
list:	/* Parser: Productions */ 
	| list '\n'
	| list expr '\n'    { printf("\t%.8g\n", Solve($2) ); }
	;
expr:	  NUMBER	{ $$ = AddLeaf($1); }
	| expr '+' expr	{ $$ = AddnonTerminal($1 ,'+', $3); }
	| expr '-' expr	{ $$ = AddnonTerminal($1 ,'-', $3); }
	| expr '*' expr	{ $$ = AddnonTerminal($1 ,'*', $3); }
	| expr '/' expr	{ $$ = AddnonTerminal($1 ,'/', $3); }
	| '(' expr ')'	{ $$ = $2; }
	;
%%
	/* end of grammar */

#include <stdio.h>
#include <ctype.h>
char	*progname;	/* for error messages */
int	lineno = 1;

main(argc, argv)	
	char *argv[];
{
	progname = argv[0];
	yyparse();
}


yyerror(s)	/* called for yacc syntax error */
	char *s;
{
	warning(s, (char *) 0);
}

warning(s, t)	/* print warning message */
	char *s, *t;
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}