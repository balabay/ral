#pragma once

#include "grammar/generatorbasevisitor.h"

#include <llvm/IR/IRBuilder.h>

namespace RaLang {

class SymbolTable;

class IrDeclarationGenerator : public GeneratorBaseVisitor {
public:
  IrDeclarationGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable,
                         llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder, llvm::Module &module);

  void visit(AstAlgorithm *algorithm) override;
  using GeneratorBaseVisitor::visit;
  void initStandardAlgorithms();

private:
  void initPrint();
  void initInput();
  void initAlgorithm(const std::pair<std::string, std::string> &algName, int line);

private:
  bool m_emitDebugInfo;
  std::string m_path;

  SymbolTable &m_symbolTable;
  llvm::LLVMContext &m_llvmContext;
  llvm::IRBuilder<> &m_builder;
  llvm::Module &m_module;
};

} // namespace RaLang
