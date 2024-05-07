#pragma once

#include "grammar/ast.h"

#include <llvm/IR/IRBuilder.h>

namespace RaLang {

class SymbolTable;

class IrGenerator : public GeneratorVisitor {
public:
  IrGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable, llvm::LLVMContext &llvmContext,
              llvm::IRBuilder<> &builder, llvm::Module &module);

  // Generator interface
public:
  void visit(Ast &ast);
  void visit(AstAlgorithm *algorithm) override;
  llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) override;
  llvm::Value *visit(AstBinaryConditionalExpression *expression) override;
  llvm::Value *visit(AstMathExpression *expression) override;
  void visit(AstExpressionStatement *expressionStatement) override;
  llvm::Value *visit(AstFunctionAffectationExpression *expression) override;
  void visit(AstIfStatement *statement) override;
  void visit(AstInputStatement *statement) override;
  llvm::Value *visit(AstIntExpression *expression) override;
  void visit(AstModule *module) override;
  void visit(AstPrintStatement *statement) override;
  void visit(AstReturnStatement *returnStatement) override;
  llvm::Value *visit(AstUnaryExpression *expression) override;
  void visit(AstVariableDeclarationStatement *statement) override;
  llvm::Value *visit(AstVariableExpression *expression) override;
  llvm::Value *visit(AstVariableAffectationExpression *expression) override;

private:
  void addReturnStatement();

private:
  bool m_emitDebugInfo;
  std::string m_path;

  SymbolTable &m_symbolTable;
  llvm::LLVMContext &m_llvmContext;
  llvm::IRBuilder<> &m_builder;
  llvm::Module &m_module;
  bool m_has_return_statement;
};

} // namespace RaLang
