#include <memory>

namespace AST {

enum class BinOp {
    Add,
    Sub,
    Mul,
    Div
};

class Visitor;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor & visitor) = 0;
};

class Expr;
class Number;
class BinaryExpr;
class Assign;

class Visitor {
public:
    virtual void visit(const Number & number) = 0;
    virtual void visit(const BinaryExpr & expr) = 0;
    virtual void visit(const Assign & assign) = 0;
};

class Expr : public ASTNode {

};

class Number : public Expr {
public:
    Number(int value) : value(value) { }
    virtual void accept(Visitor & visitor) override {
        visitor.visit(*this);
    }

private:
    int value;

    friend class PrintVisitor;
};
    
class BinaryExpr : public Expr {
public:
    BinaryExpr(BinOp op, std::unique_ptr<Expr> & lhs,
            std::unique_ptr<Expr> & rhs)
        : op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) { }
    virtual void accept(Visitor & visitor) override {
        visitor.visit(*this);
    }

private:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    BinOp op;

    friend class PrintVisitor;
};

class Assign : public ASTNode {
public:
    Assign(const std::string & identifier, std::unique_ptr<Expr> & value)
        : identifier(identifier),
          value(std::move(value)) { }
    virtual void accept(Visitor & visitor) override {
        visitor.visit(*this);
    }

private:
    std::string identifier;
    std::unique_ptr<Expr> value;

    friend class PrintVisitor;

};

class PrintVisitor : public Visitor {
    virtual void visit(const Number & number) override {
        std::cout << "(" << number.value << ")";
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


} // namespace AST
