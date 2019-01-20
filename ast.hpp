#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "driver.hpp"

namespace AST {

enum class BinOp {
    Add,
    Sub,
    Mul,
    Div
};

enum class CompOp {
    Less,
    LessEqual,
    Equals,
    Greater,
    GreaterEqual
};

class Visitor;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor & visitor, Driver & driver) = 0;
};

class Expr;
class Statement;
class IOStatement;
class IfStatement;
class Return;
class Block;
class Number;
class Identifier;
class BinaryExpr;
class Assign;
class Conditional;
class Output;

class Visitor {
public:
    virtual void visit(const Number & number, Driver & driver) = 0;
    virtual void visit(const BinaryExpr & expr, Driver & driver) = 0;
    virtual void visit(const Assign & assign, Driver & driver) = 0;
    virtual void visit(const Identifier & identifier, Driver & driver) = 0;
    virtual void visit(const Conditional & conditional, Driver & driver) = 0;
    virtual void visit(const Block & block, Driver & driver) = 0;
    virtual void visit(const IfStatement & if_stmt, Driver & driver) = 0;
    virtual void visit(const Return & ret, Driver & driver) = 0;
    virtual void visit(const Output & output, Driver & driver) = 0;
};

class Expr : public ASTNode {

};

class Statement : public ASTNode {

};

class Block : public Statement {
public:
    // TODO
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    std::vector<std::unique_ptr<Statement>> statements;
};

class Number : public Expr {
public:
    Number(int value) : value(value) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    int value;
};
    
class Identifier : public Expr {
public:
    Identifier(const std::string & name) : name(name) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    std::string name;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(BinOp op, std::unique_ptr<Expr> & lhs,
            std::unique_ptr<Expr> & rhs)
        : op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    BinOp op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
};

class Conditional : public Expr {
public:
    Conditional(CompOp op, std::unique_ptr<Expr> & lhs,
            std::unique_ptr<Expr> & rhs)
        : op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    CompOp op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
};

class IfStatement : public Statement {
public:
    IfStatement(std::unique_ptr<Conditional> & cond, std::unique_ptr<Block> & then_block)
            // std::unique_ptr<Block> & else_block)
        : cond(std::move(cond)),
          then_block(std::move(then_block)) { }
          // else_block(std::move(else_block)) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    std::unique_ptr<Conditional> cond;
    std::unique_ptr<Block> then_block;
    // std::unique_ptr<Block> else_block;
};

class Return : public Statement {
public:
    Return(std::unique_ptr<Expr> & expr)
        : expr(std::move(expr)) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    std::unique_ptr<Expr> expr;
};

class Assign : public Statement {
public:
    Assign(const std::string & identifier, std::unique_ptr<Expr> & value)
        : identifier(identifier),
          value(std::move(value)) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    std::string identifier;
    std::unique_ptr<Expr> value;
};

class IOStatement : public Statement {

};

class Output : public IOStatement {
public:
    Output(std::unique_ptr<Identifier> & identifier)
        : identifier(std::move(identifier)) { }
    virtual void accept(Visitor & visitor, Driver & driver) override {
        visitor.visit(*this, driver);
    }

    std::unique_ptr<Identifier> identifier;
};


#if 0
class PrintVisitor : public Visitor {
    virtual void visit(const Number & number) override {
        std::cout << number.value;
    }

    virtual void visit(const BinaryExpr & expr) override {
        std::cout << "(OpNo:" << static_cast<int>(expr.op) << " ";
        expr.lhs->accept(*this);
        std::cout << " ";
        expr.rhs->accept(*this);
        std::cout << ")";
    }

    virtual void visit(const Assign & assign) override {
        std::cout << "(assign " << assign.identifier << " ";
        assign.value->accept(*this);
        std::cout << ")";
    }
};
#endif


} // namespace AST
