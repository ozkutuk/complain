%option noyywrap nodefault nounput noinput

%{
#include "parser.hpp"
#include <iostream>

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type yylex(Driver & driver)

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
";"                  { return yy::parser::make_SEMICOLON(); }
":="                 { return yy::parser::make_ASSIGN(); }
"<="                 { return yy::parser::make_LESSEQUAL(); }
"<"                  { return yy::parser::make_LESSTHAN(); }
"=="                 { return yy::parser::make_EQUALS(); }
">="                 { return yy::parser::make_GREATEREQUAL(); }
">"                  { return yy::parser::make_GREATERTHAN(); }
"if"                 { return yy::parser::make_IF(); }
"endi"               { return yy::parser::make_ENDIF(); }
"return"             { return yy::parser::make_RETURN(); }
"input"              { return yy::parser::make_INPUT(); }
"output"             { return yy::parser::make_OUTPUT(); }
[a-zA-Z][a-zA-Z0-9]* { return yy::parser::make_IDENTIFIER(yytext); }
[0-9]+               { return yy::parser::make_NUMBER(std::stoi(yytext)); }
[ \t\n]              { /* ignore */ }
.                    { return yy::parser::make_ERROR(yytext); }

<<EOF>>              { return yy::parser::make_END(); }

%%
