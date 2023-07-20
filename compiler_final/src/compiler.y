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
	#include "../include/ExpressionTree.hpp"
// #define YYSTYPE double
	int yylex();
	void yyerror(char*);
        int i;	
%}

%union {
	struct expr * tree;
	char * varName;
	struct varExpr * varexpr;
	int intVal;
	bool boolVal;
	struct VariableDeclaration * declNode;
	struct Declare * decl;
	int type;
	struct statement * stmt;
	struct strExp * exp;
	struct Main * mainBlock;
	struct Glist * glist;
	struct var_list * varList;
	struct arg_list * argList;
	struct arg * Arg;
	struct func * Func;
	struct Lid_list * lidList;
	struct FunctionDef * fdef;
	struct exprList * exprlist;
	struct FunctionCall * functionCall;
}

%start code

%token BEG END
%token T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <varName> VAR STRING
%token <intVal> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
%token MAIN RETURN

%type <tree> expr ret_stmt para
%type <varexpr> var_expr 
%type <declNode> Gid Lid
%type <glist> Glist Lid_list
%type <type> ret_type func_ret_type arg_type type
%type <decl> Gdecl_list Gdecl Ldecl_sec Ldecl Ldecl_list
%type <stmt> assignment_stmt conditional_stmt write_stmt read_stmt func_stmt
%type <stmt> statement stmt_list
%type <exp> strExp 
%type <mainBlock> MainBlock 
%type <varList> var_list
%type <argList> arg_list arg_list1 FargList
%type <Arg> arg
%type <Func> func
%type <varName> func_name
%type <fdef> FunctionDef Fdef_sec
%type <exprlist> param_list1 param_list
%type <functionCall> FunctionCall

%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT
%%
	code 	:	Prog								{ 	}
	Prog	:	Gdecl_sec Fdef_sec MainBlock   		{ addFdef_sec($2); addMainBlock ($3); print_program() ; print_new_code_code(); return 0; }
			;
		
	Gdecl_sec	:	DECL Gdecl_list ENDDECL			{ Declare_block ($2); }
				;
		
	Gdecl_list	: 						{$$ = NULL; }
				| 	Gdecl Gdecl_list	{$$ = create_decl_list ($1,$2);}
				;
		
	Gdecl 	:	ret_type Glist ';'		{$$ = create_decl ((Datatype)$1,$2);}
			;
		
	ret_type	:	T_INT				{$$ = INT; }
				|	T_BOOL 				{$$ = BOOL; }
				;
		
	Glist 	:	Gid						{ $$ = create_global_decl_list($1); }
			| 	func 					{ $$ = create_global_decl_list($1); }
			|	Gid ',' Glist 			{ $$ = create_global_decl_list ($1,$3); }
			|	func ',' Glist			{ $$ = create_global_decl_list ($1,$3); }
			;
	
	Gid	:	VAR							{ $$ = DeclareNewNode ($1); }
		|	Gid '[' NUM ']'				{ $$ = DeclareNewNode ($1,$3); }

		;
		
	func:	VAR '(' arg_list ')'		{ $$ = createFunction($1,$3); }
		;
			
	arg_list:							{$$ = NULL;}
			|	arg_list1				{$$ = $1;}
			;
		
	arg_list1	:	arg ';' arg_list1 	{$$ = create_func_parameters($1,$3);}
				|	arg					{$$ = create_func_parameters($1);}
				;
		
	arg 	:	arg_type var_list		{ $$ = parameters((Datatype)$1,$2); }
		;
		
	arg_type:	T_INT					{ $$ = INT; }
		|		T_BOOL 		 			{ $$ = BOOL; }
		;

	var_list:	VAR 		 			{ $$ = create_variable_list($1); }
		|	VAR ',' var_list 			{ $$ = create_variable_list($1,$3); }
		;
		
	Fdef_sec:							{ $$ = NULL; }
		|	 Fdef_sec FunctionDef 				{ $$ = createFdefSection($2,$1); }
		;
		
	FunctionDef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{ $$ = FunctionDefinition((Datatype)$1,$2,$4,$7,$9,$10); }
		;
		
	func_ret_type:	T_INT		{ $$ = INT; }
		|	T_BOOL				{ $$ = BOOL; }
		;
			
	func_name:	VAR				{ $$ = $1; }
		;
		
	FargList:	arg_list		{ $$ = $1; }
		;

	ret_stmt:	RETURN expr ';'	{ $$ = $2; }
		;
			
	MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ $$ = makeMainBlock ((Datatype)$1,$6,$8,$9); }
					  
		;
		
	main	:	MAIN						{  }
		;
		
	Ldecl_sec	:							{$$ = NULL; }
				|DECL Ldecl_list ENDDECL	{$$ = $2; }
				;

	Ldecl_list:						{$$ = NULL;}
		|	Ldecl Ldecl_list		{$$ = create_decl_list ($1,$2);}
		;

	Ldecl	:	type Lid_list ';'	{$$ = create_decl ((Datatype)$1,$2);}
		;

	type	:	T_INT				{ $$ = INT; }
		|	T_BOOL					{ $$ = BOOL; }
		;

	Lid_list:	Lid					{$$ = create_global_decl_list($1);}
		|	Lid ',' Lid_list		{$$ = create_global_decl_list($1,$3);}
		;
		
	Lid	:	VAR						{ $$ = DeclareNewNode ($1); }
		;

	stmt_list:	/* NULL */			{ $$ = NULL; }
		|	statement stmt_list		{ $$ = createStatementList($1,$2); }
		|	error ';' 				{ $$= NULL; printf ("Error in statement.\n"); exit (1); }
		;

	statement:	assignment_stmt  ';'		{ $$ = $1; }
		|	read_stmt ';'					{ $$ = $1; }
		|	write_stmt ';'					{ $$ = $1; }
		|	conditional_stmt 				{ $$ = $1; }
		|	func_stmt ';'					{ $$ = $1; }
		;

	read_stmt:	READ '(' var_expr ')' {	$$ = createRead($3); }
		;

	write_stmt:	WRITE '(' expr ')' 			{ $$ = createWrite ($3); }
		| WRITE '('strExp')'				{ $$ = createWrite ($3); }
		;
	
	assignment_stmt:	var_expr '=' expr 					{ $$ = createAssignment($1,$3); }
		;

	conditional_stmt:	IF expr THEN stmt_list ENDIF 											{ $$ = create_IF ($2,$4); }
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 										{ $$ = create_IFELSE ($2,$4,$6); }
		|	WHILE expr DO stmt_list ENDWHILE ';'												{ $$ = create_WHILE ($2,$4); }
		|   FOR '(' assignment_stmt  ';'  expr ';'  assignment_stmt ')' '{' stmt_list '}'		{ $$ = create_FOR($3,$5,$7,$10); }
		;
	
	func_stmt:	FunctionCall 																{ $$ = makeFuncStmt($1); }
		;
		
	FunctionCall:	VAR '(' param_list ')'													{ $$ = makefunc_call($1,$3);}
		;
		
	param_list:																			{ $$ = NULL; }
		|	param_list1																	{ $$ = $1; }
		;
		
	param_list1:	para				{ $$ = makeExprList($1);}
		|	para ',' param_list1		{ $$ = makeExprList($1,$3);}
		;

	para	:	expr					{ $$ = $1;}
		;

	expr	:	NUM 					{ $$ = TerminalExp($1);	}
		|	'-' NUM						{ $$ = TerminalExp(-$2); }
		|	var_expr					{ $$ = TerminalExp($1); }
		|	T							{ $$ = TerminalExp(true); }
		|	F							{ $$ = TerminalExp(false); }
		|	'(' expr ')'				{ $$ = $2; }
		|	expr '+' expr 				{ $$ = addNonTerminalExpr($1 ,'+', $3); }
		|	expr '-' expr	 			{ $$ = addNonTerminalExpr($1 ,'-', $3); }
		|	expr '*' expr				{ $$ = addNonTerminalExpr($1 ,'*', $3); }
		|	expr '/' expr 				{ $$ = addNonTerminalExpr($1 ,'/', $3); }
		|	expr '%' expr 				{ $$ = addNonTerminalExpr($1 ,'%', $3); }
		|	expr '<' expr				{ $$ = addNonTerminalExpr($1 ,'<', $3); }
		|	expr '>' expr				{ $$ = addNonTerminalExpr($1 ,'>', $3); }
		|	expr GREATERTHANOREQUAL expr{ $$ = addNonTerminalExpr($1 ,'g', $3); }
		|	expr LESSTHANOREQUAL expr	{ $$ = addNonTerminalExpr($1 ,'l', $3); }
		|	expr NOTEQUAL expr			{ $$ = addNonTerminalExpr($1 ,'n', $3);	}
		|	expr EQUALEQUAL expr		{ $$ = addNonTerminalExpr($1 ,'e', $3); }
		|	LOGICAL_NOT expr			{ $$ = addNonTerminalExpr($2 ,'!', $2); } 
		|	expr LOGICAL_AND expr		{ $$ = addNonTerminalExpr($1 ,'&', $3); }
		|	expr LOGICAL_OR expr		{ $$ = addNonTerminalExpr($1 ,'|', $3); }
		|	FunctionCall				{ $$ = TerminalExp($1); }
		;

	strExp :  STRING                 	{ $$ = StringExp ($1); }
        	;
	
	var_expr:	VAR						{ $$ = createVal($1); }
		|	var_expr '[' expr ']'		{ $$ = createVal($1,$3); }
		;
%%

#include <stdio.h>
#include <ctype.h>
char	*progname;	/* for error messages */
int	lineno = 1;
void warning(char *s, char *t);

int main(int argc,char *argv[])	
{
	progname = argv[0];
	PrgBeginMessage();				// Function to print a start message indicating that the program has started
	yyparse();
	PrgEndMessage();				// Function to print an end message indicating that the program has ended
	return 0;	
}


void yyerror(char *s)	/* called for yacc syntax error */
{
	warning(s, (char *) 0);
}

void warning(char *s, char *t)	/* print warning message */
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
	return ;
}