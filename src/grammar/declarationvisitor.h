#pragma once
#include "logic/symboltable.h"
#include "runtime/RalParserBaseVisitor.h"

#include <llvm/IR/IRBuilder.h>

namespace llvm
{
class Module;
class LLVMContext;
}

namespace RaLang {

class SymbolTable;
class DeclarationVisitor : public RalParserBaseVisitor
{
public:
    DeclarationVisitor(SymbolTable& symbolTable, llvm::LLVMContext& llvmContext, llvm::IRBuilder<>& builder, llvm::Module& module);
    std::any visitFunction(RalParser::FunctionContext *ctx) override;

private:
    SymbolTable& m_symbolTable;
    llvm::LLVMContext& m_llvmContext;
    llvm::IRBuilder<>& m_builder;
    llvm::Module& m_module;
};

} // namespace RaLang
