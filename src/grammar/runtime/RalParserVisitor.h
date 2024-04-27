
// Generated from /home/blex/projects/ral/src/grammar/RalParser.g4 by
// ANTLR 4.13.1

#pragma once

#include "RalParser.h"
#include "antlr4-runtime.h"

/**
 * This class defines an abstract visitor for a parse tree
 * produced by RalParser.
 */
class RalParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:
  /**
   * Visit parse trees produced by RalParser.
   */
  virtual std::any visitModule(RalParser::ModuleContext *context) = 0;

  virtual std::any visitAlgorithm(RalParser::AlgorithmContext *context) = 0;

  virtual std::any
  visitAlgorithmPrototype(RalParser::AlgorithmPrototypeContext *context) = 0;

  virtual std::any
  visitAlgorithmBody(RalParser::AlgorithmBodyContext *context) = 0;

  virtual std::any
  visitFormalParameters(RalParser::FormalParametersContext *context) = 0;

  virtual std::any
  visitInstructions(RalParser::InstructionsContext *context) = 0;

  virtual std::any visitStatement(RalParser::StatementContext *context) = 0;

  virtual std::any
  visitFunctionAffectation(RalParser::FunctionAffectationContext *context) = 0;

  virtual std::any visitUnaryNegativeExpression(
      RalParser::UnaryNegativeExpressionContext *context) = 0;

  virtual std::any
  visitInParenExpression(RalParser::InParenExpressionContext *context) = 0;

  virtual std::any
  visitBinaryOperation(RalParser::BinaryOperationContext *context) = 0;

  virtual std::any
  visitNameExpression(RalParser::NameExpressionContext *context) = 0;

  virtual std::any
  visitLiteralExpression(RalParser::LiteralExpressionContext *context) = 0;

  virtual std::any visitBinaryMultiplyOperation(
      RalParser::BinaryMultiplyOperationContext *context) = 0;

  virtual std::any visitBinaryConditionalOperation(
      RalParser::BinaryConditionalOperationContext *context) = 0;

  virtual std::any visitFunctionCallExpression(
      RalParser::FunctionCallExpressionContext *context) = 0;

  virtual std::any
  visitVariableAffectation(RalParser::VariableAffectationContext *context) = 0;

  virtual std::any
  visitFunctionCall(RalParser::FunctionCallContext *context) = 0;

  virtual std::any visitArgs(RalParser::ArgsContext *context) = 0;

  virtual std::any visitLiteral(RalParser::LiteralContext *context) = 0;

  virtual std::any
  visitIntegerLiteral(RalParser::IntegerLiteralContext *context) = 0;

  virtual std::any visitType(RalParser::TypeContext *context) = 0;

  virtual std::any
  visitVariableDeclaration(RalParser::VariableDeclarationContext *context) = 0;

  virtual std::any
  visitPrintStatement(RalParser::PrintStatementContext *context) = 0;

  virtual std::any
  visitInputStatement(RalParser::InputStatementContext *context) = 0;

  virtual std::any visitIfStatement(RalParser::IfStatementContext *context) = 0;

  virtual std::any
  visitReturnStatement(RalParser::ReturnStatementContext *context) = 0;
};
