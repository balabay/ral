
// Generated from /home/blex/projects/ral/src/grammar/RalLexer.g4 by
// ANTLR 4.13.1

#pragma once

#include "antlr4-runtime.h"

class RalLexer : public antlr4::Lexer {
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

  explicit RalLexer(antlr4::CharStream *input);

  ~RalLexer() override;

  std::string getGrammarFileName() const override;

  const std::vector<std::string> &getRuleNames() const override;

  const std::vector<std::string> &getChannelNames() const override;

  const std::vector<std::string> &getModeNames() const override;

  const antlr4::dfa::Vocabulary &getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN &getATN() const override;

  // By default the static state used to implement the lexer is lazily
  // initialized during the first call to the constructor. You can call this
  // function if you wish to initialize the static state ahead of time.
  static void initialize();

private:
  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.
};
