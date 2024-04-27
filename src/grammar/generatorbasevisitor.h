#pragma once

#include "grammar/ast.h"
namespace RaLang {

class GeneratorBaseVisitor : public GeneratorVisitor {
public:
  GeneratorBaseVisitor();

  // GeneratorVisitor interface
public:
  void visit(Ast &ast);
  llvm::Value *visit(AstAlgorithm *algorithm) override;
  llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) override;
  llvm::Value *visit(AstExpressionStatement *expressionStatement) override;
  llvm::Value *visit(AstFunctionAffectationExpression *expression) override;
  llvm::Value *visit(AstInputStatement *statement) override;
  llvm::Value *visit(AstIntExpression *expression) override;
  llvm::Value *visit(AstModule *module) override;
  llvm::Value *visit(AstPrintStatement *statement) override;
  llvm::Value *visit(AstReturnStatement *returnStatement) override;
  llvm::Value *visit(AstVariableDeclarationStatement *statement) override;
  llvm::Value *visit(AstVariableExpression *expression) override;
  llvm::Value *visit(AstVariableAffectationExpression *expression) override;

protected:
  llvm::Value *visitNodes(AstNode *n);
};

} // namespace RaLang
