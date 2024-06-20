#pragma once

#include "grammar/ast.h"

#include <llvm/IR/IRBuilder.h>

namespace RaLang {

class SymbolTable;
class DebugInfoBase;

class IrGenerator : public GeneratorVisitor {
public:
  IrGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable, llvm::LLVMContext &llvmContext,
              llvm::IRBuilder<> &builder, llvm::Module &module);
  ~IrGenerator();
  // Generator interface
public:
  void visit(Ast &ast);
  void visit(AstAlgorithm *algorithm) override;
  llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) override;
  llvm::Value *visit(AstBinaryConditionalExpression *expression) override;
  llvm::Value *visit(AstBinaryLogicalExpression *expression) override;
  void visit(AstExpressionStatement *expressionStatement) override;
  llvm::Value *visit(AstFunctionAffectationExpression *expression) override;
  void visit(AstIfStatement *statement) override;
  void visit(AstInputStatement *statement) override;
  void visit(AstLoopStatement *statement) override;
  llvm::Value *visit(AstMathExpression *expression) override;
  void visit(AstModule *module) override;
  llvm::Value *visit(AstNumberLiteralExpression *expression) override;
  void visit(AstPrintStatement *statement) override;
  void visit(AstReturnStatement *returnStatement) override;
  llvm::Value *visit(AstStringLiteralExpression *expression) override;
  llvm::Value *visit(AstTypePromotionExpression *expression) override;
  void visit(AstVariableDeclarationStatement *statement) override;
  llvm::Value *visit(AstVariableExpression *expression) override;
  llvm::Value *visit(AstVariableAffectationExpression *expression) override;

private:
  void addReturnStatement(Scope *scope);
  llvm::Value *compareIntExpressions(AstTokenType t, llvm::Value *leftExprValue, llvm::Value *rightExprValue);
  llvm::Value *compareRealExpressions(AstTokenType t, llvm::Value *leftExprValue, llvm::Value *rightExprValue);
  llvm::Value *mathIntExpressions(AstTokenType t, llvm::Value *leftExprValue, llvm::Value *rightExprValue);
  llvm::Value *mathRealExpressions(AstTokenType t, llvm::Value *leftExprValue, llvm::Value *rightExprValue);
  void loopK(AstLoopStatement *statement);
  void loopUntil(AstLoopStatement *statement);
  void loopFor(AstLoopStatement *statement);

private:
  std::unique_ptr<DebugInfoBase> m_debugInfo;

  std::string m_path;

  SymbolTable &m_symbolTable;
  llvm::LLVMContext &m_llvmContext;
  llvm::IRBuilder<> &m_builder;
  llvm::Module &m_module;
  bool m_has_return_statement;
};

} // namespace RaLang
