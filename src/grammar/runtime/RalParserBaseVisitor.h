
// Generated from /home/blex/projects/ral/src/grammar/RalParser.g4 by
// ANTLR 4.13.1

#pragma once

#include "RalParserVisitor.h"
#include "antlr4-runtime.h"

/**
 * This class provides an empty implementation of RalParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the
 * available methods.
 */
class RalParserBaseVisitor : public RalParserVisitor {
public:
  virtual std::any visitModule(RalParser::ModuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAlgorithm(RalParser::AlgorithmContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitAlgorithmPrototype(RalParser::AlgorithmPrototypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitFormalParameters(RalParser::FormalParametersContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitInstructions(RalParser::InstructionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(RalParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionAffectation(
      RalParser::FunctionAffectationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryNegativeExpression(
      RalParser::UnaryNegativeExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitInParenExpression(RalParser::InParenExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitBinaryOperation(RalParser::BinaryOperationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitNameExpression(RalParser::NameExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitLiteralExpression(RalParser::LiteralExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBinaryMultiplyOperation(
      RalParser::BinaryMultiplyOperationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBinaryConditionalOperation(
      RalParser::BinaryConditionalOperationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionCallExpression(
      RalParser::FunctionCallExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableAffectation(
      RalParser::VariableAffectationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitFunctionCall(RalParser::FunctionCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgs(RalParser::ArgsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(RalParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitIntegerLiteral(RalParser::IntegerLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitType(RalParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableDeclaration(
      RalParser::VariableDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitPrintStatement(RalParser::PrintStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitInputStatement(RalParser::InputStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitIfStatement(RalParser::IfStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any
  visitReturnStatement(RalParser::ReturnStatementContext *ctx) override {
    return visitChildren(ctx);
  }
};
