%{
// #define YYSTYPE NODPTR   // Optional: define the type of yylval
#include "../bin/y.tab.h"     // Include the parser header file
#include "../include/ExpressionTree.h"   // Include any necessary external headers
extern int lineno;           // Declare an external variable
int wflag;
%}

/* %option noyywrap */
digit	[0-9]
letter	[a-zA-Z]

%%
[ \t]* ;   						//ignoring whitespace and semicolon		
main	            { return MAIN;	}	
					
return 				{ return RETURN;	}
begin				{ return BEG; 	}	
end					{ return END;	}	
read				{ return READ;	}
write				{	}	
integer				{ return T_INT;	}	
boolean				{ return T_BOOL;}	
decl				{ return DECL;	}
enddecl				{ return ENDDECL;	}
if					{	}
then				{	}	
else				{	}	
endif				{	}
do					{	}
while				{	}
endwhile			{	}
for 				{	}		
"=="				{	}	
"<="				{	}
">="				{	}
"!="				{	}	
AND					{	}	
OR					{	}
NOT					{	}
true				{ yylval.boolean=1;return T; }	
false			    { yylval.boolean=0;return F; }

[0-9]+                      { yylval.value = strtol(yytext,NULL,10);return NUM;} // Match integers
[0-9]*[.][0-9]+             { yylval.value = strtof(yytext,NULL);return NUM;}   // Match floating-point numbers

{letter}({letter}|{digit})*		{ yylval.IDname = strndup(yytext,yyleng); return VAR;}
"[\n]"                          { lineno++;return (int)*yytext;}  // Match newline character 
.								{ return (int)*yytext;}

%%
int yywrap(void){return 1;}  // End of file encountered, terminate lexer

		
// [-+*/=()><;,\{\}\[\]\%]         {	}
			
// ["]                             {	}