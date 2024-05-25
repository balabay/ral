#pragma once

#include "grammar/generatorbasevisitor.h"

namespace RaLang {

class Ast;
class SymbolTable;

class TypeCheckVisitor : public GeneratorBaseVisitor {
public:
  TypeCheckVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast);

  void visit();
  llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) override;
  llvm::Value *visit(AstBinaryConditionalExpression *expression) override;
  llvm::Value *visit(AstMathExpression *expression) override;
  llvm::Value *visit(AstUnaryExpression *expression) override;
  void visit(AstVariableDeclarationStatement *statement) override;
  llvm::Value *visit(AstVariableExpression *expression) override;

private:
  std::shared_ptr<AstExpression> promote(std::shared_ptr<AstExpression> astExpr, TypeKind type);
  TypeKind promoteBinaryExpression(AstExpression *expression);

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

} // namespace RaLang
