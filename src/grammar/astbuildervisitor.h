#pragma once

#include "runtime/RalParserBaseVisitor.h"

namespace RaLang {

class Ast;
class AstExpression;
class AstStatement;
enum class AstTokenType;
enum class LoopType;
class SymbolTable;

class AstBuilderVisitor : public RalParserBaseVisitor {
public:
  AstBuilderVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast);

  std::any visitAlgorithm(RalParser::AlgorithmContext *ctx) override;
  std::any visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) override;
  std::any visitBinaryConditionalOperation(RalParser::BinaryConditionalOperationContext *ctx) override;
  std::any visitBinaryMultiplyOperation(RalParser::BinaryMultiplyOperationContext *ctx) override;
  std::any visitBinaryOperation(RalParser::BinaryOperationContext *ctx) override;
  std::any visitExpressionStatement(RalParser::ExpressionStatementContext *ctx) override;
  std::any visitFunctionAffectation(RalParser::FunctionAffectationContext *ctx) override;
  std::any visitFunctionCall(RalParser::FunctionCallContext *ctx) override;
  std::any visitIfStatement(RalParser::IfStatementContext *ctx) override;
  std::any visitInputStatement(RalParser::InputStatementContext *ctx) override;
  std::any visitInstructions(RalParser::InstructionsContext *ctx) override;
  std::any visitIntegerLiteral(RalParser::IntegerLiteralContext *ctx) override;
  std::any visitLogicalAnd(RalParser::LogicalAndContext *ctx) override;
  std::any visitLogicalNot(RalParser::LogicalNotContext *ctx) override;
  std::any visitLogicalOr(RalParser::LogicalOrContext *ctx) override;
  std::any visitLoopKStatement(RalParser::LoopKStatementContext *ctx) override;
  std::any visitLoopUntilStatement(RalParser::LoopUntilStatementContext *ctx) override;
  std::any visitLoopWhileStatement(RalParser::LoopWhileStatementContext *ctx) override;
  std::any visitModule(RalParser::ModuleContext *ctx) override;
  std::any visitNameExpression(RalParser::NameExpressionContext *ctx) override;
  std::any visitInParenExpression(RalParser::InParenExpressionContext *ctx) override;
  std::any visitPowerOperation(RalParser::PowerOperationContext *ctx) override;
  std::any visitPrintStatement(RalParser::PrintStatementContext *ctx) override;
  std::any visitRealLiteral(RalParser::RealLiteralContext *ctx) override;
  std::any visitReturnStatement(RalParser::ReturnStatementContext *ctx) override;
  std::any visitStringLiteral(RalParser::StringLiteralContext *ctx) override;
  std::any visitSwitchStatement(RalParser::SwitchStatementContext *ctx) override;
  std::any visitUnaryNegativeExpression(RalParser::UnaryNegativeExpressionContext *ctx) override;
  std::any visitVariableAffectation(RalParser::VariableAffectationContext *ctx) override;
  std::any visitVariableDeclaration(RalParser::VariableDeclarationContext *ctx) override;

protected:
  std::any aggregateResult(std::any /*aggregate*/, std::any nextResult) override;

private:
  std::shared_ptr<AstExpression> createVariableExpression(int line, const std::string &name);
  std::shared_ptr<AstExpression>
  createBinaryLogicalExpression(AstTokenType type, std::vector<RalParser::ExpressionContext *> expressions, int line);
  std::shared_ptr<AstExpression>
  createMathExpression(AstTokenType type, std::vector<RalParser::ExpressionContext *> expressions, int line);
  void addBinarySubExpressions(std::shared_ptr<AstExpression> &astExpr,
                               std::vector<RalParser::ExpressionContext *> expressions, int line);
  std::shared_ptr<AstExpression> createCallExpression(const std::string &name,
                                                      std::vector<RalParser::ExpressionContext *> callArgs, int line);
  std::vector<std::shared_ptr<AstStatement>>
  createStatements(const std::vector<RalParser::StatementContext *> &statementContexts);
  std::shared_ptr<AstStatement> createLoop(LoopType loopType, RalParser::ExpressionContext *expression,
                                           RalParser::InstructionsContext *loopContext);

private:
  SymbolTable &m_symbolTable;
  Ast &m_ast;
  std::string m_fileName;
};

extern std::string getAlgorithmName(RalParser::AlgorithmNameContext *ctx);

} // namespace RaLang
