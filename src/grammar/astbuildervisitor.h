#pragma once

#include "runtime/RalParserBaseVisitor.h"

namespace RaLang {

class Ast;
class SymbolTable;

class AstBuilderVisitor : public RalParserBaseVisitor {
public:
  AstBuilderVisitor(const std::string &fileName, SymbolTable &symbolTable,
                    Ast &ast);

  std::any visitAlgorithm(RalParser::AlgorithmContext *ctx) override;
  std::any visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) override;
  std::any visitInstructions(RalParser::InstructionsContext *ctx) override;
  std::any visitModule(RalParser::ModuleContext *ctx) override;
  std::any
  visitReturnStatement(RalParser::ReturnStatementContext *ctx) override;
  std::any visitStatement(RalParser::StatementContext *ctx) override;
  std::any visitFunctionCall(RalParser::FunctionCallContext *ctx) override;

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

} // namespace RaLang
