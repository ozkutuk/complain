%language "c++"
%defines
%define api.token.constructor
%define api.value.type variant

%code requires {
#include <vector>
#include <iostream>
#include <string>
#include <memory>

#include "ast.hpp"
}

%code {

yy::parser::symbol_type yylex();

void yy::parser::error(const std::string & message) {
    std::cout << "Error: " << message << std::endl;
}

}

%type <std::unique_ptr<AST::Expr>> expr term factor;
%type <std::unique_ptr<AST::Assign>> assign;

%token <int> NUMBER
%token <std::string> IDENTIFIER ERROR
%token EOL ASSIGN MINUS PLUS MUL DIV LPAREN RPAREN
%token END 0


%start program

%%

program: program assign EOL    { AST::PrintVisitor printer; $2->accept(printer); std::cout << std::endl; } 
       | program ERROR EOL     { std::cerr << "Error: " << $2 << std::endl; }
       | %empty
       ;

assign: IDENTIFIER ASSIGN expr { $$ = std::make_unique<AST::Assign>($1, $3); }
      ;

expr: expr MINUS term          { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Sub, $1, $3); }
    | expr PLUS term           { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Add, $1, $3); }
    | term                     { $$ = std::move($1); }
    ;

term: term MUL factor          { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Mul, $1, $3); }
    | term DIV factor          { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Div, $1, $3); }
    | factor                   { $$ = std::move($1); }
    ;

factor: IDENTIFIER             { /* TODO find val from symbol table */ $$ = std::make_unique<AST::Number>(0); } // temp
      | NUMBER                 { $$ = std::make_unique<AST::Number>($1); }
      | LPAREN expr RPAREN     { $$ = std::move($2); }
      ;


%%

void yyerror(const std::string & message) {
    std::cerr << message << std::endl;
}
