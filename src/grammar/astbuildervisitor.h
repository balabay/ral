#pragma once

#include "runtime/RalParserBaseVisitor.h"

namespace RaLang {

class Ast;
class SymbolTable;

class AstBuilderVisitor : public RalParserBaseVisitor {
public:
  AstBuilderVisitor(const std::string &fileName, SymbolTable &symbolTable,
                    Ast &ast);

  std::any visitModule(RalParser::ModuleContext *ctx) override;
  std::any visitAlgorithm(RalParser::AlgorithmContext *ctx) override;
  std::any visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) override;

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

} // namespace RaLang
