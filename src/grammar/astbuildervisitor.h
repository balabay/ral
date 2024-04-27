#pragma once

#include "runtime/RalParserBaseVisitor.h"

namespace RaLang {

class Ast;
class SymbolTable;
class AstExpression;

class AstBuilderVisitor : public RalParserBaseVisitor {
public:
  AstBuilderVisitor(const std::string &fileName, SymbolTable &symbolTable,
                    Ast &ast);

  std::any visitAlgorithm(RalParser::AlgorithmContext *ctx) override;
  std::any visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) override;
  std::any visitFunctionCall(RalParser::FunctionCallContext *ctx) override;
  std::any visitInputStatement(RalParser::InputStatementContext *ctx) override;
  std::any visitInstructions(RalParser::InstructionsContext *ctx) override;
  std::any visitIntegerLiteral(RalParser::IntegerLiteralContext *ctx) override;
  std::any visitModule(RalParser::ModuleContext *ctx) override;
  std::any visitNameExpression(RalParser::NameExpressionContext *ctx) override;
  std::any visitPrintStatement(RalParser::PrintStatementContext *ctx) override;
  std::any
  visitReturnStatement(RalParser::ReturnStatementContext *ctx) override;
  std::any visitStatement(RalParser::StatementContext *ctx) override;
  std::any
  visitVariableDeclaration(RalParser::VariableDeclarationContext *ctx) override;

private:
  std::shared_ptr<AstExpression> getVariableByName(const std::string &name,
                                                   int line);

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

} // namespace RaLang
