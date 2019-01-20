#pragma once

#include <unordered_map>
#include <string>

#include "ast.hpp"
#include "driver.hpp"

#include "llvm/IR/Value.h"

class CodegenVisitor : public AST::Visitor {
public:
    virtual void visit(const AST::Number & number, Driver & driver) override;
    virtual void visit(const AST::BinaryExpr & expr, Driver & driver) override;
    virtual void visit(const AST::Assign & assign, Driver & driver) override;
    virtual void visit(const AST::Identifier & identifier, Driver & driver) override;
    virtual void visit(const AST::Comparison & comparison, Driver & driver) override;
    virtual void visit(const AST::Logical & logical, Driver & driver) override;
    virtual void visit(const AST::IfStatement & if_stmt, Driver & driver) override;
    virtual void visit(const AST::Block & block, Driver & driver) override;
    virtual void visit(const AST::Return & ret, Driver & driver) override;
    virtual void visit(const AST::Output & output, Driver & driver) override;

    llvm::Value * value;
};

namespace Codegen {

void print_ir();
void init();

}
