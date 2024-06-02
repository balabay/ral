#include "generatorbasevisitor.h"

namespace RaLang {

GeneratorBaseVisitor::GeneratorBaseVisitor() {}

void GeneratorBaseVisitor::visit(Ast &ast) {
  for (auto &node : ast.getNodes()) {
    node->accept(this);
  }
}

void GeneratorBaseVisitor::visit(AstAlgorithm *algorithm) { visitNodes(algorithm); }

llvm::Value *GeneratorBaseVisitor::visit(AstAlgorithmCallExpression *algorithmCall) {
  return visitNodes(algorithmCall);
}

llvm::Value *GeneratorBaseVisitor::visit(AstBinaryConditionalExpression *expression) { return visitNodes(expression); }

llvm::Value *GeneratorBaseVisitor::visit(AstBinaryLogicalExpression *expression) { return visitNodes(expression); }

llvm::Value *GeneratorBaseVisitor::visit(AstMathExpression *expression) { return visitNodes(expression); }

void GeneratorBaseVisitor::visit(AstExpressionStatement *expressionStatement) { visitNodes(expressionStatement); }

llvm::Value *GeneratorBaseVisitor::visit(AstFunctionAffectationExpression *expression) {
  return visitNodes(expression);
}

void GeneratorBaseVisitor::visit(AstIfStatement *statement) {
  statement->ifCondition()->accept(this);
  for (auto st : statement->thenBlock()) {
    st->accept(this);
  }
  if (statement->hasElse()) {
    for (auto st : statement->elseBlock()) {
      st->accept(this);
    }
  }
}

void GeneratorBaseVisitor::visit(AstInputStatement *statement) { visitNodes(statement); }

void GeneratorBaseVisitor::visit(AstLoopKStatement *statement) {
  statement->getLoopCount()->accept(this);
  visitNodes(statement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstNumberLiteralExpression *expression) { return visitNodes(expression); }

void GeneratorBaseVisitor::visit(AstModule *module) { visitNodes(module); }

void GeneratorBaseVisitor::visit(AstPrintStatement *statement) {
  visitNodes(statement);
  for (auto &formatSpecifier : statement->getFormatSpecifiers()) {
    if (formatSpecifier.first) {
      formatSpecifier.first->accept(this);
      if (formatSpecifier.second) {
        formatSpecifier.second->accept(this);
      }
    }
  }
}

void GeneratorBaseVisitor::visit(AstReturnStatement *returnStatement) { visitNodes(returnStatement); }

llvm::Value *GeneratorBaseVisitor::visit(AstStringLiteralExpression *expression) { return visitNodes(expression); }

llvm::Value *GeneratorBaseVisitor::visit(AstTypePromotionExpression *expression) { return visitNodes(expression); }

llvm::Value *GeneratorBaseVisitor::visit(AstUnaryExpression *expression) { return visitNodes(expression); }

void GeneratorBaseVisitor::visit(AstVariableDeclarationStatement *statement) { visitNodes(statement); }

llvm::Value *GeneratorBaseVisitor::visit(AstVariableExpression *expression) { return visitNodes(expression); }

llvm::Value *GeneratorBaseVisitor::visit(AstVariableAffectationExpression *expression) {
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
