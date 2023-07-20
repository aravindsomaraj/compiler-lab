digit [0-9]
letter [A-Za-z_] 
%{
// #define YYSTYPE double   // Optional: define the type of yylval
#include "../bin/y.tab.h"     // Include the parser header file
#include "../include/ExpressionTree.h"   // Include any necessary external headers
extern int lineno;           // Declare an external variable
%} 
  
/* Rules Section*/
%%
[ \t]* ;                    // Ignore whitespace and semicolons
[0-9]+                      {yylval.value = strtol(yytext,NULL,10);return NUMBER;} // Match integers
[0-9]*[.][0-9]+             {yylval.value = strtof(yytext,NULL);return NUMBER;}   // Match floating-point numbers
"begindecl"                 {return BEGINDECL;}      // Match "begindecl" keyword
"enddecl"                   {return ENDDECL;}        // Match "enddecl" keyword
"integer"                   {return INTEGER;}        // Match "integer" keyword
"print"                     {yylval.fName = PRINT;return FNAME;}  // Match "print" keyword
{letter}({letter}|{digit})* {yylval.IDname = strndup(yytext,yyleng); return ID;}  // Match identifiers
"[\n]"                      {lineno++;return (int)*yytext;}  // Match newline character
.                           {return (int)*yytext;};   // Match any other character
%%
  
int yywrap(void){return 1;}  // End of file encountered, terminate lexer

 