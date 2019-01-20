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
#include "codegen.hpp"

class Driver;
}

%param { Driver & driver }

%code {

#include "driver.hpp"

yy::parser::symbol_type yylex(Driver & driver);

void yy::parser::error(const std::string & message) {
    std::cout << "Error: " << message << std::endl;
}

}

%type <std::unique_ptr<AST::Expr>> expr term factor;
%type <std::unique_ptr<AST::Assign>> assign;
%type <std::unique_ptr<AST::Block>> statements;
%type <std::unique_ptr<AST::Statement>> statement;
%type <std::unique_ptr<AST::IfStatement>> if_stmt;
%type <std::unique_ptr<AST::Return>> return_stmt;
%type <std::unique_ptr<AST::IOStatement>> io_stmt;
%type <std::unique_ptr<AST::Conditional>> conditional;
%type <AST::CompOp> comparison;

%token <int> NUMBER
%token <std::string> IDENTIFIER ERROR
%token MINUS PLUS MUL DIV 
%token LESSTHAN LESSEQUAL EQUALS GREATERTHAN GREATEREQUAL
%token ASSIGN LPAREN RPAREN SEMICOLON RETURN INPUT OUTPUT IF ENDIF
%token END 0


%start program

%%

program: statements END     { CodegenVisitor codegen; $1->accept(codegen, driver); Codegen::print_ir(); } 
       ;

statements: statements statement SEMICOLON { 
          #if 0
          if(!$$) {
            std::cout << "A" << std::endl;
            $$ = std::make_unique<AST::Block>();
            } else {
            std::cout << "B" << std::endl;
            }
            #endif
            $$ = std::move($1);
            $$->statements.push_back(std::move($2)); }

          | %empty                         { // std::cout << "C" << std::endl;
                                             $$ = std::make_unique<AST::Block>();
                                            }
          ;

statement: assign      { $$ = std::move($1); }
         | return_stmt { $$ = std::move($1); }
         | io_stmt     { $$ = std::move($1); }
         | if_stmt     { $$ = std::move($1); }
         ;

if_stmt: IF conditional statements ENDIF { $$ = std::make_unique<AST::IfStatement>($2, $3); } // TODO else
       ;

conditional: expr comparison expr { $$ = std::make_unique<AST::Conditional>($2, $1, $3); }
           ;

comparison: LESSTHAN     { $$ = AST::CompOp::Less; }
          | LESSEQUAL    { $$ = AST::CompOp::LessEqual; }
          | EQUALS       { $$ = AST::CompOp::Equals; }
          | GREATERTHAN  { $$ = AST::CompOp::Greater; }
          | GREATEREQUAL { $$ = AST::CompOp::GreaterEqual; }
          ;

return_stmt: RETURN expr    { $$ = std::make_unique<AST::Return>($2); }
           ;

assign: IDENTIFIER ASSIGN expr { 
                                 $$ = std::make_unique<AST::Assign>($1, $3);
                                 CodegenVisitor codegen; 
                                 $$->accept(codegen, driver);
                                }
      ;

io_stmt: INPUT IDENTIFIER      { std::cerr << "input statements are not implemented" << std::endl; $$ = nullptr; }
       | OUTPUT IDENTIFIER     { auto tmp_id = std::make_unique<AST::Identifier>($2);
                                 $$ = std::make_unique<AST::Output>(tmp_id);
                               }
       ;

expr: expr MINUS term          { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Sub, $1, $3); }
    | expr PLUS term           { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Add, $1, $3); }
    | term                     { $$ = std::move($1); }
    ;

term: term MUL factor          { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Mul, $1, $3); }
    | term DIV factor          { $$ = std::make_unique<AST::BinaryExpr>(AST::BinOp::Div, $1, $3); }
    | factor                   { $$ = std::move($1); }
    ;

factor: IDENTIFIER             { $$ = std::make_unique<AST::Identifier>($1); } 
      | NUMBER                 { $$ = std::make_unique<AST::Number>($1); }
      | LPAREN expr RPAREN     { $$ = std::move($2); }
      ;


%%

void yyerror(const std::string & message) {
    std::cerr << message << std::endl;
}
