#include "grammar/typecheckvisitor.h"

#include "grammar/ast.h"
#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"
#include "utils.h"

#include <cassert>

namespace RaLang {

TypeCheckVisitor::TypeCheckVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast)
    : m_symbolTable(symbolTable), m_ast(ast), m_fileName(fileName) {}

void TypeCheckVisitor::visit() {
  for (auto &module : m_ast.getNodes()) {
    module->accept(this);
  }
}

void TypeCheckVisitor::visit(AstAlgorithm *algorithm) {
  std::string algName = algorithm->getName();
  AlgSymbol *algSymbol = resolveAlgorithm(m_symbolTable.getGlobals(), algName, algorithm->getLine());
  m_symbolTable.pushScope(algorithm->getScope());
  m_symbolTable.pushScope(algorithm->getLocalScope());
  // Create storage for return value
  if (algSymbol->getType()->getTypeKind() != TypeKind::Void) {
    VariableSymbol *returnValueSymbol = m_symbolTable.createVariableSymbol(RAL_RET_VALUE, algSymbol->getType());
    algorithm->getLocalScope()->define(std::unique_ptr<Symbol>(returnValueSymbol));
  }
  GeneratorBaseVisitor::visit(algorithm);
  m_symbolTable.popScope();
  m_symbolTable.popScope();
}

llvm::Value *TypeCheckVisitor::visit(AstAlgorithmCallExpression *algorithmCall) {
  int line = algorithmCall->getLine();
  std::string name = algorithmCall->getName();
  AlgSymbol *algSymbol = resolveAlgorithm(algorithmCall->getScope(), name, line);
  auto actualArgs = algorithmCall->getNodes();
  std::vector<Symbol *> formalParameters = algSymbol->getFormalParameters();

  if (actualArgs.size() != formalParameters.size()) {
    throw AlgArgMismatchException("Expected " + std::to_string(formalParameters.size()) + " but found " +
                                  std::to_string(actualArgs.size()) + " for call " + name + " at " +
                                  std::to_string(line));
  }

  for (unsigned i = 0; i != actualArgs.size(); i++) {
    auto astExpr = std::dynamic_pointer_cast<AstExpression>(actualArgs[i]);
    assert(astExpr);
    astExpr->accept(this);
    TypeKind expressionTypeKind = astExpr->getTypeKind();
    Type *formalParameterType = formalParameters[i]->getType();
    TypeKind formalParameterTypeKind = formalParameterType->getTypeKind();
    if (expressionTypeKind != formalParameterTypeKind) {
      std::shared_ptr<AstExpression> astPromotionExpression = promote(astExpr, formalParameterTypeKind);
      algorithmCall->replaceNode(i, astPromotionExpression);
    }
  }

  algorithmCall->setTypeKind(algSymbol->getType()->getTypeKind());
  return nullptr;
}

llvm::Value *TypeCheckVisitor::visit(AstBinaryConditionalExpression *expression) {
  promoteBinaryExpression(expression);
  expression->setTypeKind(TypeKind::Boolean);
  return nullptr;
}

llvm::Value *TypeCheckVisitor::visit(AstBinaryLogicalExpression *expression) {
  int line = expression->getLine();
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(leftAstExpr);
  auto rightAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[1]);
  assert(rightAstExpr);
  leftAstExpr->accept(this);
  rightAstExpr->accept(this);
  TypeKind l = leftAstExpr->getTypeKind();
  TypeKind r = rightAstExpr->getTypeKind();

  if (l != TypeKind::Boolean) {
    std::shared_ptr<AstExpression> astPromotionExpression = promote(leftAstExpr, TypeKind::Boolean);
    expression->replaceNode(0, astPromotionExpression);
  }
  if (r != TypeKind::Boolean) {
    std::shared_ptr<AstExpression> astPromotionExpression = promote(rightAstExpr, TypeKind::Boolean);
    expression->replaceNode(1, astPromotionExpression);
  }
  expression->setTypeKind(TypeKind::Boolean);
  return nullptr;
}

llvm::Value *TypeCheckVisitor::visit(AstFunctionAffectationExpression *expression) {
  int line = expression->getLine();
  // Check that current function is not void
  AlgSymbol *alg = getCurrentAlg(expression->getScope());
  assert(alg);
  if (alg->getType()->getTypeKind() == TypeKind::Void) {
    throw TypeException("Algorithm " + alg->getName() + " does not return value, cannot assign to it at line " +
                        std::to_string(line));
  }

  std::string name = RAL_RET_VALUE;
  auto variableSymbol = expression->getScope()->resolve(name);
  assert(variableSymbol);

  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 1);
  auto astExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(astExpr);
  astExpr->accept(this);
  TypeKind expressionTypeKind = astExpr->getTypeKind();
  TypeKind algReturnType = variableSymbol->getType()->getTypeKind();
  if (algReturnType != expressionTypeKind) {
    std::shared_ptr<AstExpression> astPromotionExpression = promote(astExpr, algReturnType);
    expression->replaceNode(0, astPromotionExpression);
  }
  expression->setTypeKind(algReturnType);
  return nullptr;
}

void TypeCheckVisitor::visit(AstIfStatement *statement) {
  GeneratorBaseVisitor::visit(statement);
  auto astIfCondition = statement->ifCondition();
  TypeKind expressionTypeKind = astIfCondition->getTypeKind();
  if ((expressionTypeKind != TypeKind::Int) && (expressionTypeKind != TypeKind::Boolean)) {
    std::shared_ptr<AstExpression> astPromotionExpression = promote(astIfCondition, TypeKind::Boolean);
    statement->replaceIfCondition(astPromotionExpression);
  }
}

void TypeCheckVisitor::visit(AstLoopStatement *statement) {
  GeneratorBaseVisitor::visit(statement);
  auto astLoop = statement->getLoopExpression();
  TypeKind expressionTypeKind = astLoop->getTypeKind();
  switch (statement->getLoopType()) {
  case LoopType::K: {
    if (expressionTypeKind != TypeKind::Int) {
      std::shared_ptr<AstExpression> astPromotionExpression = promote(astLoop, TypeKind::Int);
      statement->replaceLoopExpression(astPromotionExpression);
    }
    break;
  }
  case LoopType::Until:
  case LoopType::While: {
    if (expressionTypeKind != TypeKind::Boolean) {
      std::shared_ptr<AstExpression> astPromotionExpression = promote(astLoop, TypeKind::Boolean);
      statement->replaceLoopExpression(astPromotionExpression);
    }
    break;
  }
  default:
    throw NotImplementedException("Unknown loop type at " + std::to_string(statement->getLine()) + " " +
                                  std::to_string(static_cast<int>(statement->getLoopType())));
  }
}

llvm::Value *TypeCheckVisitor::visit(AstMathExpression *expression) {
  TypeKind t = promoteBinaryExpression(expression);
  expression->setTypeKind(t);
  return nullptr;
}

void TypeCheckVisitor::visit(AstPrintStatement *statement) {
  GeneratorBaseVisitor::visit(statement);
  // Promote format specifiers to Int
  for (auto &formatSpecifier : statement->getFormatSpecifiers()) {
    if (formatSpecifier.first) {
      TypeKind formatExprType = formatSpecifier.first->getTypeKind();
      if ((formatExprType != TypeKind::Int)) {
        std::shared_ptr<AstExpression> astPromotionExpression = promote(formatSpecifier.first, TypeKind::Int);
        statement->replaceFormat(formatSpecifier.first, astPromotionExpression);
      }
      if (formatSpecifier.second) {
        TypeKind precisionExprType = formatSpecifier.second->getTypeKind();
        if ((precisionExprType != TypeKind::Int)) {
          std::shared_ptr<AstExpression> astPromotionExpression = promote(formatSpecifier.second, TypeKind::Int);
          statement->replaceFormat(formatSpecifier.second, astPromotionExpression);
        }
      }
    }
  }
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

  auto astExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(astExpr);
  astExpr->accept(this);
  TypeKind expressionTypeKind = astExpr->getTypeKind();

  if (variableTypeKind == expressionTypeKind) {
    return;
  }

  std::shared_ptr<AstExpression> astPromotionExpression = promote(astExpr, variableTypeKind);
  statement->replaceNode(0, astPromotionExpression);
}

llvm::Value *TypeCheckVisitor::visit(AstVariableExpression *expression) {
  std::string name = expression->getName();
  Symbol *variableSymbol = expression->getScope()->resolve(name);
  assert(variableSymbol);
  Type *variableType = variableSymbol->getType();
  TypeKind variableTypeKind = variableType->getTypeKind();
  expression->setTypeKind(variableTypeKind);
  return nullptr;
}

llvm::Value *TypeCheckVisitor::visit(AstVariableAffectationExpression *expression) {
  std::string name = expression->getName();
  Symbol *variableSymbol = expression->getScope()->resolve(name);
  assert(variableSymbol);
  Type *variableType = variableSymbol->getType();
  TypeKind variableTypeKind = variableType->getTypeKind();

  auto nodes = expression->getNodes();
  assert(nodes.size());
  auto astExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(astExpr);
  astExpr->accept(this);
  TypeKind expressionTypeKind = astExpr->getTypeKind();

  if (variableTypeKind != expressionTypeKind) {
    std::shared_ptr<AstExpression> astPromotionExpression = promote(astExpr, variableTypeKind);
    expression->replaceNode(0, astPromotionExpression);
  }
  return nullptr;
}

std::shared_ptr<AstExpression> TypeCheckVisitor::promote(std::shared_ptr<AstExpression> astExpr, TypeKind type) {
  bool canPromote = false;
  switch (astExpr->getTypeKind()) {
  case TypeKind::Boolean:
  case TypeKind::Int:
  case TypeKind::Real: {
    switch (type) {
    case TypeKind::Boolean:
    case TypeKind::Int:
    case TypeKind::Real: {
      canPromote = true;
      break;
    }
    default: {
    }
    }
    break;
  }
  default: {
  }
  }

  if (canPromote) {
    return std::dynamic_pointer_cast<AstExpression>(AstTypePromotionExpression::create(type, astExpr));
  }

  throw TypePromotionException("Cannot promote type from " + typeKindToString(astExpr->getTypeKind()) + " to " +
                               typeKindToString(type) + " at line " + std::to_string(astExpr->getLine()));
}

TypeKind TypeCheckVisitor::promoteBinaryExpression(AstExpression *expression) {
  int line = expression->getLine();
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(leftAstExpr);
  auto rightAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[1]);
  assert(rightAstExpr);
  leftAstExpr->accept(this);
  rightAstExpr->accept(this);
  TypeKind l = leftAstExpr->getTypeKind();
  TypeKind r = rightAstExpr->getTypeKind();
  if (l == r) {
    if ((l != TypeKind::Boolean) && (l != TypeKind::Int) && (l != TypeKind::Real)) {
      throw TypeException("Cannot process types " + typeKindToString(l) + " " + typeKindToString(r) + " line " +
                          std::to_string(line));
    }
    return l;
  } else {
    TypeKind leftPromoted = l;
    TypeKind rightPromoted = r;
    if ((l == TypeKind::Real) || (r == TypeKind::Real)) {
      leftPromoted = rightPromoted = TypeKind::Real;
    } else if ((l == TypeKind::Int) || (r == TypeKind::Int)) {
      leftPromoted = rightPromoted = TypeKind::Int;
    } else {
      throw TypeException("Cannot process types " + typeKindToString(l) + " " + typeKindToString(r) + " line " +
                          std::to_string(line));
    }

    if (leftPromoted != l) {
      std::shared_ptr<AstExpression> astPromotionExpression = promote(leftAstExpr, leftPromoted);
      expression->replaceNode(0, astPromotionExpression);
    }
    if (rightPromoted != r) {
      std::shared_ptr<AstExpression> astPromotionExpression = promote(rightAstExpr, rightPromoted);
      expression->replaceNode(1, astPromotionExpression);
    }
    return leftPromoted;
  }
}

} // namespace RaLang
