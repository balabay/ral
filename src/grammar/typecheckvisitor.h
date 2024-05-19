#pragma once

#include "grammar/generatorbasevisitor.h"

namespace RaLang {

class Ast;
class SymbolTable;

class TypeCheckVisitor : public GeneratorBaseVisitor {
public:
  TypeCheckVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast);

  void visit();

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

} // namespace RaLang
