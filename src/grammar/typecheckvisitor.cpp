#include "grammar/typecheckvisitor.h"

namespace RaLang {

TypeCheckVisitor::TypeCheckVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast)
    : m_symbolTable(symbolTable), m_ast(ast), m_fileName(fileName) {}

void TypeCheckVisitor::visit() {
  for (auto &module : m_ast.getNodes()) {
    module->accept(this);
  }
}

} // namespace RaLang
