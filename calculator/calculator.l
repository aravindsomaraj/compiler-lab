%{
#include "y.tab.h"
extern int lineno;
%} 
  
/* Rules Section*/
%%
[ \t]* ;
([0-9])*[.]?[0-9]+    {yylval.value = strtof(yytext,NULL);return NUMBER;}  

"\n" {lineno++;return (int)*yytext;}
. {return (int)*yytext;};
%%
  
int yywrap(void){}
