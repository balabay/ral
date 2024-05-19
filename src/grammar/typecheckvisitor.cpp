#include "grammar/typecheckvisitor.h"

#include "grammar/ast.h"
#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"

#include <cassert>

namespace RaLang {

TypeCheckVisitor::TypeCheckVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast)
    : m_symbolTable(symbolTable), m_ast(ast), m_fileName(fileName) {}

void TypeCheckVisitor::visit() {
  for (auto &module : m_ast.getNodes()) {
    module->accept(this);
  }
}

llvm::Value *TypeCheckVisitor::visit(AstUnaryExpression *expression) {
  auto nodes = expression->getNodes();
  assert(nodes.size());
  auto astExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(astExpr);
  astExpr->accept(this);
  TypeKind expressionTypeKind = astExpr->getTypeKind();
  expression->setTypeKind(expressionTypeKind);
  return nullptr;
}

void TypeCheckVisitor::visit(AstVariableDeclarationStatement *statement) {
  const auto &nodes = statement->getNodes();
  if (nodes.size() == 0) {
    // not initialized variable
    return;
  }

  // build-in types are defined in global scope
  Type *resolvedType = resolveTypeNoException(m_symbolTable.getGlobals(), statement->getTypeName());
  if (!resolvedType) {
    // Some custom type, no type promotion for custom types
    // TODO: check type identity with initializer
    return;
  }
  TypeKind variableTypeKind = resolvedType->getTypeKind();

  auto expression = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(expression);
  expression->accept(this);
  TypeKind expressionTypeKind = expression->getTypeKind();

  if (variableTypeKind == expressionTypeKind) {
    return;
  }

  std::shared_ptr<AstExpression> astPromotionExpression = promote(expression, variableTypeKind);
  if (astPromotionExpression) {
    statement->replaceNode(0, astPromotionExpression);
  }
}

std::shared_ptr<AstExpression> TypeCheckVisitor::promote(std::shared_ptr<AstExpression> astExpr, TypeKind type) {
  if (astExpr->getTypeKind() == TypeKind::Int) {
    if (type == TypeKind::Real) {
      return std::dynamic_pointer_cast<AstExpression>(AstTypePromotionExpression::create(type, astExpr));
    }
  }

  if (astExpr->getTypeKind() == TypeKind::Real) {
    if (type == TypeKind::Int) {
      return std::dynamic_pointer_cast<AstExpression>(AstTypePromotionExpression::create(type, astExpr));
    }
  }

  if (type == TypeKind::Real || type == TypeKind::Int) {
    throw TypePromotionException(
        "Cannot promote type from " + std::to_string(static_cast<int>(astExpr->getTypeKind())) + " to " +
        std::to_string(static_cast<int>(type)) + " at line " + std::to_string(astExpr->getLine()));
  }
  return nullptr;
}

} // namespace RaLang
