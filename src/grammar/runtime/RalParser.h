
// Generated from /home/blex/projects/ral/src/grammar/RalParser.g4 by
// ANTLR 4.13.1

#pragma once

#include "antlr4-runtime.h"

class RalParser : public antlr4::Parser {
public:
  enum {
    Add = 1,
    Sub = 2,
    Mul = 3,
    Div = 4,
    Mod = 5,
    Gt = 6,
    Gte = 7,
    Lt = 8,
    Lte = 9,
    Eq = 10,
    Ne = 11,
    Equal = 12,
    OpeningParen = 13,
    ClosingParen = 14,
    OpeningBrace = 15,
    ClosingBrace = 16,
    OpeningBracket = 17,
    ClosingBracket = 18,
    Comma = 19,
    IntegerTypeName = 20,
    FloatingPointTypeName = 21,
    AlgorhitmHeader = 22,
    BeginAlgorhitmImplementation = 23,
    EndAlgorhitmImplementation = 24,
    TerminalInput = 25,
    TerminalOutput = 26,
    If = 27,
    Then = 28,
    Else = 29,
    EndOfIfOrSwitchStatement = 30,
    LoopBreakAndAlgorhitmReturn = 31,
    FunctionReturnValue = 32,
    LogicalAnd = 33,
    LogicalOr = 34,
    LogicalNot = 35,
    Id = 36,
    DecimalLiteral = 37,
    ZeroLiteral = 38,
    Comment = 39,
    WhiteSpace = 40,
    LineTerminator = 41
  };

  enum {
    RuleModule = 0,
    RuleAlgorithm = 1,
    RuleAlgorithmPrototype = 2,
    RuleAlgorithmBody = 3,
    RuleFormalParameters = 4,
    RuleInstructions = 5,
    RuleStatement = 6,
    RuleExpression = 7,
    RuleFunctionCall = 8,
    RuleArgs = 9,
    RuleLiteral = 10,
    RuleIntegerLiteral = 11,
    RuleType = 12,
    RuleVariableDeclaration = 13,
    RulePrintStatement = 14,
    RuleInputStatement = 15,
    RuleIfStatement = 16,
    RuleReturnStatement = 17
  };

  explicit RalParser(antlr4::TokenStream *input);

  RalParser(antlr4::TokenStream *input,
            const antlr4::atn::ParserATNSimulatorOptions &options);

  ~RalParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN &getATN() const override;

  const std::vector<std::string> &getRuleNames() const override;

  const antlr4::dfa::Vocabulary &getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  class ModuleContext;
  class AlgorithmContext;
  class AlgorithmPrototypeContext;
  class AlgorithmBodyContext;
  class FormalParametersContext;
  class InstructionsContext;
  class StatementContext;
  class ExpressionContext;
  class FunctionCallContext;
  class ArgsContext;
  class LiteralContext;
  class IntegerLiteralContext;
  class TypeContext;
  class VariableDeclarationContext;
  class PrintStatementContext;
  class InputStatementContext;
  class IfStatementContext;
  class ReturnStatementContext;

  class ModuleContext : public antlr4::ParserRuleContext {
  public:
    ModuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<AlgorithmContext *> algorithm();
    AlgorithmContext *algorithm(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ModuleContext *module();

  class AlgorithmContext : public antlr4::ParserRuleContext {
  public:
    AlgorithmContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AlgorhitmHeader();
    AlgorithmPrototypeContext *algorithmPrototype();
    AlgorithmBodyContext *algorithmBody();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AlgorithmContext *algorithm();

  class AlgorithmPrototypeContext : public antlr4::ParserRuleContext {
  public:
    AlgorithmPrototypeContext(antlr4::ParserRuleContext *parent,
                              size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    TypeContext *type();
    antlr4::tree::TerminalNode *OpeningParen();
    antlr4::tree::TerminalNode *ClosingParen();
    FormalParametersContext *formalParameters();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AlgorithmPrototypeContext *algorithmPrototype();

  class AlgorithmBodyContext : public antlr4::ParserRuleContext {
  public:
    AlgorithmBodyContext(antlr4::ParserRuleContext *parent,
                         size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BeginAlgorhitmImplementation();
    InstructionsContext *instructions();
    antlr4::tree::TerminalNode *EndAlgorhitmImplementation();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AlgorithmBodyContext *algorithmBody();

  class FormalParametersContext : public antlr4::ParserRuleContext {
  public:
    FormalParametersContext(antlr4::ParserRuleContext *parent,
                            size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TypeContext *> type();
    TypeContext *type(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Id();
    antlr4::tree::TerminalNode *Id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode *Comma(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  FormalParametersContext *formalParameters();

  class InstructionsContext : public antlr4::ParserRuleContext {
  public:
    InstructionsContext(antlr4::ParserRuleContext *parent,
                        size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StatementContext *> statement();
    StatementContext *statement(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  InstructionsContext *instructions();

  class StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VariableDeclarationContext *variableDeclaration();
    IfStatementContext *ifStatement();
    PrintStatementContext *printStatement();
    InputStatementContext *inputStatement();
    ExpressionContext *expression();
    ReturnStatementContext *returnStatement();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  StatementContext *statement();

  class ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);

    ExpressionContext() = default;
    void copyFrom(ExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;
  };

  class FunctionAffectationContext : public ExpressionContext {
  public:
    FunctionAffectationContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *FunctionReturnValue();
    antlr4::tree::TerminalNode *Equal();
    ExpressionContext *expression();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class UnaryNegativeExpressionContext : public ExpressionContext {
  public:
    UnaryNegativeExpressionContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *Sub();
    ExpressionContext *expression();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class InParenExpressionContext : public ExpressionContext {
  public:
    InParenExpressionContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *OpeningParen();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *ClosingParen();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class BinaryOperationContext : public ExpressionContext {
  public:
    BinaryOperationContext(ExpressionContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext *expression(size_t i);
    antlr4::tree::TerminalNode *Add();
    antlr4::tree::TerminalNode *Sub();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class NameExpressionContext : public ExpressionContext {
  public:
    NameExpressionContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *Id();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class LiteralExpressionContext : public ExpressionContext {
  public:
    LiteralExpressionContext(ExpressionContext *ctx);

    LiteralContext *literal();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class BinaryMultiplyOperationContext : public ExpressionContext {
  public:
    BinaryMultiplyOperationContext(ExpressionContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext *expression(size_t i);
    antlr4::tree::TerminalNode *Mul();
    antlr4::tree::TerminalNode *Div();
    antlr4::tree::TerminalNode *Mod();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class BinaryConditionalOperationContext : public ExpressionContext {
  public:
    BinaryConditionalOperationContext(ExpressionContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext *expression(size_t i);
    antlr4::tree::TerminalNode *Gt();
    antlr4::tree::TerminalNode *Gte();
    antlr4::tree::TerminalNode *Lt();
    antlr4::tree::TerminalNode *Lte();
    antlr4::tree::TerminalNode *Eq();
    antlr4::tree::TerminalNode *Ne();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class FunctionCallExpressionContext : public ExpressionContext {
  public:
    FunctionCallExpressionContext(ExpressionContext *ctx);

    FunctionCallContext *functionCall();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class VariableAffectationContext : public ExpressionContext {
  public:
    VariableAffectationContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Equal();
    ExpressionContext *expression();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionContext *expression();
  ExpressionContext *expression(int precedence);
  class FunctionCallContext : public antlr4::ParserRuleContext {
  public:
    FunctionCallContext(antlr4::ParserRuleContext *parent,
                        size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *OpeningParen();
    antlr4::tree::TerminalNode *ClosingParen();
    ArgsContext *args();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  FunctionCallContext *functionCall();

  class ArgsContext : public antlr4::ParserRuleContext {
  public:
    ArgsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext *expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode *Comma(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ArgsContext *args();

  class LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  LiteralContext *literal();

  class IntegerLiteralContext : public antlr4::ParserRuleContext {
  public:
    IntegerLiteralContext(antlr4::ParserRuleContext *parent,
                          size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DecimalLiteral();
    antlr4::tree::TerminalNode *ZeroLiteral();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  IntegerLiteralContext *integerLiteral();

  class TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IntegerTypeName();
    antlr4::tree::TerminalNode *FloatingPointTypeName();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TypeContext *type();

  class VariableDeclarationContext : public antlr4::ParserRuleContext {
  public:
    VariableDeclarationContext(antlr4::ParserRuleContext *parent,
                               size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    std::vector<antlr4::tree::TerminalNode *> Id();
    antlr4::tree::TerminalNode *Id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Equal();
    antlr4::tree::TerminalNode *Equal(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext *expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode *Comma(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  VariableDeclarationContext *variableDeclaration();

  class PrintStatementContext : public antlr4::ParserRuleContext {
  public:
    PrintStatementContext(antlr4::ParserRuleContext *parent,
                          size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TerminalOutput();
    std::vector<ExpressionContext *> expression();
    ExpressionContext *expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode *Comma(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PrintStatementContext *printStatement();

  class InputStatementContext : public antlr4::ParserRuleContext {
  public:
    InputStatementContext(antlr4::ParserRuleContext *parent,
                          size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TerminalInput();
    std::vector<antlr4::tree::TerminalNode *> Id();
    antlr4::tree::TerminalNode *Id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode *Comma(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  InputStatementContext *inputStatement();

  class IfStatementContext : public antlr4::ParserRuleContext {
  public:
    IfStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *If();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *Then();
    InstructionsContext *instructions();
    antlr4::tree::TerminalNode *EndOfIfOrSwitchStatement();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  IfStatementContext *ifStatement();

  class ReturnStatementContext : public antlr4::ParserRuleContext {
  public:
    ReturnStatementContext(antlr4::ParserRuleContext *parent,
                           size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LoopBreakAndAlgorhitmReturn();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ReturnStatementContext *returnStatement();

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex,
               size_t predicateIndex) override;

  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily
  // initialized during the first call to the constructor. You can call this
  // function if you wish to initialize the static state ahead of time.
  static void initialize();

private:
};
