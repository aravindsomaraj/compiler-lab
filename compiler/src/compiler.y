/*
 *   This file is part of SIL Compiler.
 *
 *  SIL Compiler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SIL Compiler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SIL Compiler.  If not, see <http://www.gnu.org/licenses/>.
 */

%{	
#include <stdio.h>
#include "../include/ExpressionTree.h"
// #define YYSTYPE double
int yylex();
void yyerror( char* );
    int i;	

SymbolTable SymbolTablesym = {0,NULL,NULL};
Instruct *codeStart = NULL;
Instruct *codeEnd = NULL;
%}

%union {
    struct expr * tree;
    double value;
	int boolean;
	char* IDname;
	int type;
	struct symbol* syms;
	struct Instruct * ins;
	struct exprList * expList;
	int fName;
}

%start code

%token BEG END
%token T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <IDname> VAR 
%token <boolean> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token <boolean> T F 
%token MAIN RETURN

%type <tree> expr 	
/* %type <syms> var_list  */
%type <syms> Lid_list
%type <type> func_ret_type
%type <type> type
%type <IDname> Lid
%type <IDname> var_expr

/*%type <expList> exprs*/

%type <ins> /*Gdecl Fdef*/ Ldecl assign_stmt read_stmt write_stmt func_stmt /* cond_stmt */



%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT
%%

code	:	Prog	{ CodeExec(codeStart);printCode();return 0;}
Prog	:	/*Gdecl_sec Fdef_sec*/ MainBlock
		;
/*		
Gdecl_sec:	
		 | DECL Gdecl_list ENDDECL		{	}   //Mandatory, Remove Null val
		 ;
		
Gdecl_list: 
		| 	Gdecl Gdecl_list
		;
		
Gdecl 	:	ret_type Glist ';'  {	}
		;
		
ret_type:	T_INT				{	}
		|	T_BOOL 				{	}
		;
		
Glist 	:	Gid
		| 	func 
		|	Gid ',' Glist 
		|	func ',' Glist
		;
	
Gid	:	VAR						{	}
		|	Gid '[' NUM ']'		{	}

		;
		
func 	:	VAR '(' arg_list ')' 	{	}
		;
			
arg_list:	
		|	arg_list1
		;
		
arg_list1:	arg_list1 ';' arg
		|	arg
		;
		
arg 	:	arg_type var_list	
		;
		
arg_type:	T_INT		 		{ 	}
		|	T_BOOL 		 		{	}
		;

var_list:	VAR 		 		{	}
		|	VAR ',' var_list 	{	}
		;
		
Fdef_sec:	
		|	Fdef_sec Fdef
		;
		
Fdef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{	}
		;
*/		
func_ret_type:	T_INT		{ $$ = INT;	}
		|	T_BOOL			{ $$ = BOOL;}
		;
			
/* func_name:	VAR				{	}
		;
		
FargList:	arg_list		{	}
		; */

ret_stmt:	
		| RETURN expr ';'	{	} 	// remove empty case
		;
			
MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ }
					  
		;
		
main	:	MAIN	
		;
		
Ldecl_sec:	
		| DECL Ldecl_list ENDDECL	/*{	}   Mandatory, Remove Null val*/
		;

Ldecl_list:		
		|	Ldecl_list Ldecl	{addIns($2); }
		;

Ldecl	:	type Lid_list ';'	{$$ = Declaration($2,$1);}	
		;

type	:	T_INT			{ $$ = INT;	}
		|	T_BOOL			{ $$ = BOOL;}
		;

Lid_list:	Lid					{$$ = Symbol($1,NULL);}
		|	Lid ',' Lid_list	{$$ = Symbol($1,$3);}
		;
		
Lid	:	VAR						{$$ = $1;	}
	;

stmt_list:	/* NULL */			//{	}
		|	statement stmt_list	{	}
		|	error ';' 			{	}
		;

statement:	assign_stmt  ';'	{ addIns($1);	}
		|	read_stmt ';'		{ addIns($1);	}
		|	write_stmt ';'		{ addIns($1);	}
		|	cond_stmt 			{	}
		|	func_stmt ';'		{ addIns($1);	}
		;

read_stmt:	READ '(' var_expr ')' 			{ $$ = returnRead($3);	}
		;

write_stmt:	WRITE '(' expr ')' 	{	}
		  | WRITE '(''"' str_expr '"'')'     	{	}
		  ;
	
assign_stmt:	var_expr '=' expr 			{ $$ = returnAssignment($1,$3);	}
		;

cond_stmt:	IF expr THEN stmt_list ENDIF 					{	}
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{	}
		|	WHILE expr DO stmt_list ENDWHILE ';'			{	}
		|   FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'       {	}
		;
	
func_stmt:	func_call 				{	}
		 ;
		
func_call:	VAR '(' param_list ')'	{	}
		 ;
		
param_list:				
		  |	param_list1		
		  ;
		
param_list1:	para			
		|	para ',' param_list1	
		;

para	:	expr				{	}
		;

expr	:	NUM 				{ $$ = TerminalExp($1);	}
		|	'-' NUM				{ $$ = TerminalExp(-1*$2);	}
		|	var_expr			{ $$ = TerminalVar($1);	}
		|	T					{ $$ = TerminalBool($1);	}
		|	F					{ $$ = TerminalBool($1);	}
		|	'(' expr ')'		{	}

		|	expr '+' expr 		{ $$ = AddNonTerminalExp($1 ,'+', $3);	}
		|	expr '-' expr	 	{ $$ = AddNonTerminalExp($1 ,'-', $3);	}
		|	expr '*' expr 		{ $$ = AddNonTerminalExp($1 ,'*', $3);	}
		|	expr '/' expr 		{ $$ = AddNonTerminalExp($1 ,'/', $3);	}
		|	expr '%' expr 		{ $$ = AddNonTerminalExp($1 ,'%', $3);	}
		|	expr '<' expr		{ $$ = AddRelationalOp($1, GREATER, $3);	}
		|	expr '>' expr		{ $$ = AddRelationalOp($1, LESSER, $3);	}
		|	expr GREATERTHANOREQUAL expr	{ $$ = AddRelationalOp($1, GREATERTHANOREQUAL, $3);	}
		|	expr LESSTHANOREQUAL expr		{ $$ = AddRelationalOp($1, LESSTHANOREQUAL, $3);	}
		|	expr NOTEQUAL expr				{ $$ = AddRelationalOp($1, NOTEQUAL, $3);	}
		|	expr EQUALEQUAL expr			{ $$ = AddRelationalOp($1, EQUALEQUAL, $3);	}
		|	LOGICAL_NOT expr				{ $$ = AddLogicalOp(NULL,LOGICAL_NOT,$2);	}
		|	expr LOGICAL_AND expr			{ $$ = AddLogicalOp($1,LOGICAL_AND,$3);	}
		|	expr LOGICAL_OR expr			{ $$ = AddLogicalOp($1,LOGICAL_OR,$3);	}
		|	func_call						{   }

		;
str_expr: VAR            					{	}
        | str_expr VAR   					{	}
        ;
	
var_expr:	VAR								{ $$ = $1;	}
		/*|	var_expr '[' expr ']'			{	}*/
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