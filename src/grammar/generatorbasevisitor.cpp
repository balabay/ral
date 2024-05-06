#include "generatorbasevisitor.h"

namespace RaLang {

GeneratorBaseVisitor::GeneratorBaseVisitor() {}

void GeneratorBaseVisitor::visit(Ast &ast) {
  for (auto &node : ast.getNodes()) {
    node->accept(this);
  }
}

llvm::Value *GeneratorBaseVisitor::visit(AstAlgorithm *algorithm) {
  return visitNodes(algorithm);
}

llvm::Value *
GeneratorBaseVisitor::visit(AstAlgorithmCallExpression *algorithmCall) {
  return visitNodes(algorithmCall);
}

llvm::Value *
GeneratorBaseVisitor::visit(AstBinaryConditionalExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *GeneratorBaseVisitor::visit(AstMathExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *
GeneratorBaseVisitor::visit(AstExpressionStatement *expressionStatement) {
  return visitNodes(expressionStatement);
}

llvm::Value *
GeneratorBaseVisitor::visit(AstFunctionAffectationExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *GeneratorBaseVisitor::visit(AstIfStatement *statement) {
  // TODO: visit condition and branches
  return visitNodes(statement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstInputStatement *statement) {
  return visitNodes(statement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstIntExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *GeneratorBaseVisitor::visit(AstModule *module) {
  return visitNodes(module);
}

llvm::Value *GeneratorBaseVisitor::visit(AstPrintStatement *statement) {
  return visitNodes(statement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstReturnStatement *returnStatement) {
  return visitNodes(returnStatement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstUnaryExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *
GeneratorBaseVisitor::visit(AstVariableDeclarationStatement *statement) {
  return visitNodes(statement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstVariableExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *
GeneratorBaseVisitor::visit(AstVariableAffectationExpression *expression) {
  return visitNodes(expression);
}

llvm::Value *GeneratorBaseVisitor::visitNodes(AstNode *n) {
  llvm::Value *result = nullptr;
  for (auto node : n->getNodes()) {
    result = node->accept(this);
  }
  return result;
}

} // namespace RaLang
