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
	void yyerror( char* );
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
	struct strExp * strExp;
	struct Main * mainBlock;
}

%start code

%token BEG END
%token T_INT T_BOOL
%token WRITE
%token DECL ENDDECL
%token <varName> VAR 
%token <intVal> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE
%token T F 
%token MAIN RETURN

%type <tree> expr ret_stmt
%type <varexpr> var_expr 
%type <declNode> Gid Glist
%type <type> ret_type func_ret_type
%type <decl> Gdecl_list Gdecl 
%type <stmt> assign_stmt cond_stmt write_stmt 
%type <stmt> statement stmt_list
%type <strExp> strExp 
%type <mainBlock> MainBlock


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT
%%
	code 	: 	Prog		{ }
	 	 	;
	Prog	:	Gdecl_sec MainBlock 			{addMainBlock ($2); printOverallPrg(); start(); printGlobalSymbolTable(); return 0;  }
			;
		
	Gdecl_sec	:	DECL Gdecl_list ENDDECL		{ Declare_block ($2);  }
				;
		
	Gdecl_list	:	{$$ = NULL; } 	 					
				| 	Gdecl Gdecl_list  	{$$ = makeDeclarationList ($1,$2); }
				;

	Gdecl 	:	ret_type Glist ';' {$$ = makeDeclaration ((Datatype)$1,$2); }
			;
		
	ret_type	:	T_INT		{ $$ = INT; }
				|	T_BOOL 		{ $$ = BOOL; }
				;
		
	Glist 	:	Gid 			{ $$ = $1; }
			|	Gid ',' Glist 	{ $$ = makeDeclList ($1,$3); }
			;
	
	Gid	:	VAR					{ $$ = DeclareNewNode ($1); }
		|	VAR '[' NUM ']'		{ $$ = DeclareNewNode ($1,$3); }
		;
				
	func_ret_type:	T_INT		{ $$ = INT; }
		|	T_BOOL				{ $$ = BOOL; }
		;
			
	ret_stmt:	RETURN expr ';'	{ $$ = $2; }
		;
			
	MainBlock: 	func_ret_type main '('')''{' BEG stmt_list ret_stmt END  '}'		{ $$ = makeMainBlock ((Datatype)$1,NULL,$7,$8); }
					  
		;
		
	main	:	MAIN					{ }
		;

	stmt_list:	/* NULL */			{ $$ = NULL; }
		|	statement stmt_list		{ $$ = createStatementList($1,$2); }
		|	error ';' 				{ $$= NULL; printf ("Error in statement.\n"); exit (1);}
		;

	statement:	assign_stmt  ';'	{ $$ = $1; }   
		|	write_stmt ';'			{ $$ = $1; }
		|	cond_stmt 				{ $$ = $1; }  
		;

	write_stmt:	WRITE '(' expr ')' 			{ $$ = createWrite ($3); }
		 | WRITE '(''"' strExp '"'')'     { $$ = createWrite ($4); }

		;
	
	assign_stmt:	var_expr '=' expr 					{ $$ = createAssignment($1,$3); }
		;

	cond_stmt:	IF expr THEN stmt_list ENDIF 											{ $$ = create_IF ($2,$4); }
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 								{ $$ = create_IFELSE ($2,$4,$6); }
		|	WHILE expr DO stmt_list ENDWHILE ';'										{ $$ = create_WHILE ($2,$4); }
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
		|	LOGICAL_NOT expr			{ $$ = addNonTerminalExpr($2 ,'!', $2); } // added both the params as $2 to avoid making a new function.
		|	expr LOGICAL_AND expr		{ $$ = addNonTerminalExpr($1 ,'&', $3); }
		|	expr LOGICAL_OR expr		{ $$ = addNonTerminalExpr($1 ,'|', $3); }
		;

	strExp 	:  	VAR                 { $$ = StringExp ($1); }
                | 	VAR strExp   		{ $$ = StringExp ($1,$2);}
                ;
	
	var_expr:	VAR					{ $$ = createVal($1); }
		|	VAR '[' expr ']'		{ $$ = createVal($1,$3); }
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