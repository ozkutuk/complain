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

    llvm::Value * value;
    std::unordered_map<std::string,llvm::Value *> symbols = {}; // allocs

};

namespace Codegen {

void write_result(llvm::Value * val);
void print_ir();
void init();

}
