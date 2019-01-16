#pragma once

#include "ast.hpp"
#include "llvm/IR/Value.h"

class CodegenVisitor : public AST::Visitor {
public:
    virtual void visit(const AST::Number & number) override;
    virtual void visit(const AST::BinaryExpr & expr) override;
    virtual void visit(const AST::Assign & assign) override;

    llvm::Value * value;
};

namespace Codegen {

void write_result(llvm::Value * val);
void print_ir();
void init();

}
