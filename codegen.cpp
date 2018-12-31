#include "codegen.hpp"

#include <iostream>
#include <string>
#include <map>

#include "ast.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Value.h"

using namespace llvm;

static LLVMContext ctx;
static IRBuilder<> builder(ctx);
static std::map<std::string, Value *> locals;

static Value * LogErrorV(const std::string & message) {
    std::cerr << message << std::endl;
    return nullptr;
}

void CodegenVisitor::visit(const AST::Number & number) {
    value = ConstantFP::get(ctx, APFloat(static_cast<double>(number.value)));
}

void CodegenVisitor::visit(const AST::BinaryExpr & expr) {
    Value * tmp = value; 
    expr.lhs->accept(*this);
    Value * L = value;
    expr.lhs->accept(*this);
    Value * R = value;
    value = tmp;

    if (!L || !R)
        value = nullptr;

    switch (expr.op) {
        case AST::BinOp::Add: {
            value = builder.CreateFAdd(L, R, "addtmp");
        }
        case AST::BinOp::Sub: {
            value = builder.CreateFSub(L, R, "subtmp");
        }
        case AST::BinOp::Mul: {
            value = builder.CreateFMul(L, R, "multmp");
        }
        case AST::BinOp::Div: {
            value =  builder.CreateFDiv(L, R, "divtmp");
        }
        default: {
            value = LogErrorV("invalid binary operator");
        }
    }

}

void CodegenVisitor::visit(const AST::Assign & assign) {
    // TODO
    value = nullptr;
}

