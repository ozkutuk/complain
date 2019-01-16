#include "codegen.hpp"

#include <iostream>
#include <string>
#include <map>

#include "ast.hpp"
#include "driver.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Value.h"

static llvm::LLVMContext ctx;
// we do not support multiple translation units, so we can just initialize the module here
static std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("top", ctx);
static llvm::IRBuilder<> builder(ctx);

namespace Codegen {

void init() {
    llvm::FunctionType * mainType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    llvm::Function * mainFunction = llvm::Function::Create(mainType,
            llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock * mainBlock = llvm::BasicBlock::Create(ctx, "entry", mainFunction);

    // initial insert point is main block for now
    builder.SetInsertPoint(mainBlock);
}

void write_result(llvm::Value * val) {
    builder.CreateRet(val);
}

void print_ir() {
    module->print(llvm::errs(), nullptr);
}

}


static llvm::Value * LogErrorV(const std::string & message) {
    std::cerr << message << std::endl;
    return nullptr;
}

void CodegenVisitor::visit(const AST::Number & number, Driver & driver) {
    // std::cout << number.value << std::endl;
    value = llvm::ConstantInt::get(ctx, llvm::APInt(32, number.value, true));
}

void CodegenVisitor::visit(const AST::BinaryExpr & expr, Driver & driver) {
    llvm::Value * tmp = value; 
    expr.lhs->accept(*this, driver);
    llvm::Value * L = value;
    expr.rhs->accept(*this, driver);
    llvm::Value * R = value;
    value = tmp;

    if (!L || !R)
        value = nullptr;

    switch (expr.op) {
        case AST::BinOp::Add: {
            value = builder.CreateAdd(L, R, "addtmp");
            break;
        }
        case AST::BinOp::Sub: {
            value = builder.CreateSub(L, R, "subtmp");
            break;
        }
        case AST::BinOp::Mul: {
            value = builder.CreateMul(L, R, "multmp");
            break;
        }
        case AST::BinOp::Div: {
            value =  builder.CreateUDiv(L, R, "divtmp");
            break;
        }
        default: {
            value = LogErrorV("invalid binary operator");
            break;
        }
    }

}

void CodegenVisitor::visit(const AST::Assign & assign, Driver & driver) {
    // value = nullptr;
    assign.value->accept(*this, driver);


    auto it = driver.globals.find(assign.identifier);
    if (it != driver.globals.end()) {
        auto alloc = it->second;
        value = builder.CreateStore(value, alloc);
    }
    else {
        llvm::Value * alloc = builder.CreateAlloca(value->getType());
        driver.globals[assign.identifier] =  alloc;
        value = builder.CreateStore(value, alloc);
    }
    // llvm::Value * num = ConstantInt::get(int_type, aTable.value, true);
    // Value *alloc = new AllocaInst(int_type, aTable.variableName, entry);
    // StoreInst *ptr = new StoreInst(num,alloc,false,entry);
}

void CodegenVisitor::visit(const AST::Identifier & identifier, Driver & driver) {
    auto it = driver.globals.find(identifier.name);
    if (it != driver.globals.end()) {
        auto alloc = it->second;
        value = builder.CreateLoad(alloc);
    }
    else {
        value = LogErrorV("identifier referenced before initialization");
    }

    
}

