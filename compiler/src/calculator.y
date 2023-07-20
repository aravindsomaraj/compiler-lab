%{
// #define	YYSTYPE	double 	/* data type of yacc stack */
#include<stdio.h>
#include "../include/ExpressionTree.h"

SymbolTable SymbolTablesym = {0,NULL,NULL};
Instruct *codeStart = NULL;
Instruct *codeEnd = NULL;
%}


%left	'+' '-'	  /* left associative, same precedence */
%left	'*' '/'	  /* left assoc., higher precedence */
%union {
    struct expr * tree;
    double value;
	char* IDname;
	struct symbol* syms;
	struct Instruct * ins;
	struct exprList * expList;
	int fName;
}

%start code
%token INTEGER
%token <fName> FNAME
%token <IDname> ID
%token <value> NUMBER
%token ENDDECL BEGINDECL
%type <tree> expr 	
%type <syms> IDs 
%type <expList> exprs
%type <ins> declaration function assignment

%%
code 	: statements	{ CodeExec(codeStart);printCode();return 0;}
		;
statements 	: statement statements
			| statement 
			;

statement	: BEGINDECL declarationBlock ENDDECL    
			| assignment ';'    					{addIns($1);}
			| function ';'   						{addIns($1);}
			;

expr:	  NUMBER	{ $$ = TerminalExp($1); }
	|	'-' NUMBER  { $$ = TerminalExp(-1*$2); }
	| 		ID		{ $$ = TerminalVar($1);	}
	| expr '+' expr { $$ = AddNonTerminalExp($1 ,'+', $3); }
	| expr '-' expr	{ $$ = AddNonTerminalExp($1 ,'-', $3); }
	| expr '*' expr	{ $$ = AddNonTerminalExp($1 ,'*', $3); }
	| expr '/' expr	{ $$ = AddNonTerminalExp($1 ,'/', $3); }
	| '(' expr ')'	{ $$ = $2; }
	;

declarationBlock 	: declarationBlock declaration ';' 	{addIns($2);}
					| declaration ';' 					{addIns($1);}
					;

declaration	: DATATYPE IDs {$$ = Declaration($2,INT);}
			;

DATATYPE 	: INTEGER
			;

IDs : ID ',' IDs 	{$$ = Symbol($1,$3);}
	| ID 			{$$ = Symbol($1,NULL);}
	;

exprs	: expr ',' exprs 	{$$ = addExp ($1,$3);  }
		| expr 				{$$ = addExp ($1,NULL);}
		;

assignment	: ID '=' expr { $$ = returnAssignment($1,$3); }
			;

function	: FNAME '('exprs')' {$$ = returnFunction($1,$3); }
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