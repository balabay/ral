#pragma once

#include "grammar/ast.h"

#include <llvm/IR/IRBuilder.h>

namespace RaLang {

class SymbolTable;

class IrGenerator : public Generator
{
public:
    IrGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable,
                llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder,
                llvm::Module &module);

    // Generator interface
public:
    void visit(Ast& ast);
    llvm::Value *visit(AstModule *module) override;
    llvm::Value *visit(AstAlgorithm *algorithm) override;
    llvm::Value *visit(AstReturnStatement *returnStatement) override;

private:
  bool m_emitDebugInfo;
  std::string m_path;

  SymbolTable &m_symbolTable;
  llvm::LLVMContext &m_llvmContext;
  llvm::IRBuilder<> &m_builder;
  llvm::Module &m_module;
};

} // namespace RaLang
