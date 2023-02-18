/* Directives */
%{
using namespace std;

#include "mylexer.hpp"

#undef YY_DECL
#define YY_DECL int MyLexer::produceToken(Token ** tok)
%}

NPAT 0|[1-9][0-9]*
%%

{NPAT} { *tok = new NumToken(yytext); return NUM; }
[+] { *tok = new Token(); return PLUS; } 
[\n ] { }
.    { return ERROR; }
%%
/* added to the end of the generated file */
