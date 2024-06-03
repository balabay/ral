#pragma once

#include "grammar/ast.h"
namespace RaLang {

class GeneratorBaseVisitor : public GeneratorVisitor {
public:
  GeneratorBaseVisitor();

  // GeneratorVisitor interface
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
  void visit(AstLoopKStatement *statement) override;
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

protected:
  llvm::Value *visitNodes(AstNode *n);
};

} // namespace RaLang
