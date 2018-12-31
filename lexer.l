%option noyywrap nodefault nounput noinput

%{
#include "parser.hpp"
#include <iostream>

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type yylex()

#undef yyterminate
#define yyterminate yy::parser::make_END()
%}


%%

"+"                  { return yy::parser::make_PLUS(); }
"-"                  { return yy::parser::make_MINUS(); }
"*"                  { return yy::parser::make_MUL(); }
"/"                  { return yy::parser::make_DIV(); }
"("                  { return yy::parser::make_LPAREN(); }
")"                  { return yy::parser::make_RPAREN(); }
":="                 { return yy::parser::make_ASSIGN(); }
[a-zA-Z][a-zA-Z0-9]* { return yy::parser::make_IDENTIFIER(yytext); }
[0-9]+               { return yy::parser::make_NUMBER(std::stoi(yytext)); }
\n                   { return yy::parser::make_EOL(); }
[ \t]                { /* ignore */ }
.                    { return yy::parser::make_ERROR(yytext); }

<<EOF>>              { return yy::parser::make_END(); }

%%
