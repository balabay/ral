#pragma once

#include "grammar/generatorbasevisitor.h"

namespace RaLang {

class Ast;
class SymbolTable;

class TypeCheckVisitor : public GeneratorBaseVisitor {
public:
  TypeCheckVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast);

  void visit();
  llvm::Value *visit(AstUnaryExpression *expression) override;
  void visit(AstVariableDeclarationStatement *statement) override;

private:
  std::shared_ptr<AstExpression> promote(std::shared_ptr<AstExpression> astExpr, TypeKind type);

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

} // namespace RaLang
