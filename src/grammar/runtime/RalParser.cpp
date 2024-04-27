
// Generated from /home/blex/projects/ral/src/grammar/RalParser.g4 by
// ANTLR 4.13.1

#include "RalParserVisitor.h"

#include "RalParser.h"

using namespace antlrcpp;

using namespace antlr4;

namespace {

struct RalParserStaticData final {
  RalParserStaticData(std::vector<std::string> ruleNames,
                      std::vector<std::string> literalNames,
                      std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  RalParserStaticData(const RalParserStaticData &) = delete;
  RalParserStaticData(RalParserStaticData &&) = delete;
  RalParserStaticData &operator=(const RalParserStaticData &) = delete;
  RalParserStaticData &operator=(RalParserStaticData &&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ralparserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
    RalParserStaticData *ralparserParserStaticData = nullptr;

void ralparserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ralparserParserStaticData != nullptr) {
    return;
  }
#else
  assert(ralparserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<RalParserStaticData>(
      std::vector<std::string>{
          "module", "algorithm", "algorithmPrototype", "algorithmBody",
          "formalParameters", "instructions", "statement", "expression",
          "functionCall", "args", "literal", "integerLiteral", "type",
          "variableDeclaration", "printStatement", "inputStatement",
          "ifStatement", "returnStatement"},
      std::vector<std::string>{"",
                               "'+'",
                               "'-'",
                               "'*'",
                               "'/'",
                               "'%'",
                               "'>'",
                               "'>='",
                               "'<'",
                               "'<='",
                               "'='",
                               "'!='",
                               "':='",
                               "'('",
                               "')'",
                               "'{'",
                               "'}'",
                               "'['",
                               "']'",
                               "','",
                               "'\\u0446\\u0435\\u043B'",
                               "'\\u0432\\u0435\\u0449'",
                               "'\\u0430\\u043B\\u0433'",
                               "'\\u043D\\u0430\\u0447'",
                               "'\\u043A\\u043E\\u043D'",
                               "'\\u0432\\u0432\\u043E\\u0434'",
                               "'\\u0432\\u044B\\u0432\\u043E\\u0434'",
                               "'\\u0435\\u0441\\u043B\\u0438'",
                               "'\\u0442\\u043E'",
                               "'\\u0438\\u043D\\u0430\\u0447\\u0435'",
                               "'\\u0432\\u0441\\u0451'",
                               "'\\u0432\\u044B\\u0445\\u043E\\u0434'",
                               "'\\u0437\\u043D\\u0430\\u0447'",
                               "'\\u0438'",
                               "'\\u0438\\u043B\\u0438'",
                               "'\\u043D\\u0435'",
                               "",
                               "",
                               "'0'"},
      std::vector<std::string>{"",
                               "Add",
                               "Sub",
                               "Mul",
                               "Div",
                               "Mod",
                               "Gt",
                               "Gte",
                               "Lt",
                               "Lte",
                               "Eq",
                               "Ne",
                               "Equal",
                               "OpeningParen",
                               "ClosingParen",
                               "OpeningBrace",
                               "ClosingBrace",
                               "OpeningBracket",
                               "ClosingBracket",
                               "Comma",
                               "IntegerTypeName",
                               "FloatingPointTypeName",
                               "AlgorhitmHeader",
                               "BeginAlgorhitmImplementation",
                               "EndAlgorhitmImplementation",
                               "TerminalInput",
                               "TerminalOutput",
                               "If",
                               "Then",
                               "Else",
                               "EndOfIfOrSwitchStatement",
                               "LoopBreakAndAlgorhitmReturn",
                               "FunctionReturnValue",
                               "LogicalAnd",
                               "LogicalOr",
                               "LogicalNot",
                               "Id",
                               "DecimalLiteral",
                               "ZeroLiteral",
                               "Comment",
                               "WhiteSpace",
                               "LineTerminator"});
  static const int32_t serializedATNSegment[] = {
      4,   1,   41,  181, 2,   0,   7,   0,   2,   1,   7,   1,   2,   2,   7,
      2,   2,   3,   7,   3,   2,   4,   7,   4,   2,   5,   7,   5,   2,   6,
      7,   6,   2,   7,   7,   7,   2,   8,   7,   8,   2,   9,   7,   9,   2,
      10,  7,   10,  2,   11,  7,   11,  2,   12,  7,   12,  2,   13,  7,   13,
      2,   14,  7,   14,  2,   15,  7,   15,  2,   16,  7,   16,  2,   17,  7,
      17,  1,   0,   5,   0,   38,  8,   0,   10,  0,   12,  0,   41,  9,   0,
      1,   0,   1,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   3,
      2,   50,  8,   2,   1,   2,   1,   2,   1,   2,   3,   2,   55,  8,   2,
      1,   2,   3,   2,   58,  8,   2,   1,   3,   1,   3,   1,   3,   1,   3,
      1,   4,   1,   4,   1,   4,   1,   4,   1,   4,   1,   4,   5,   4,   70,
      8,   4,   10,  4,   12,  4,   73,  9,   4,   1,   5,   5,   5,   76,  8,
      5,   10,  5,   12,  5,   79,  9,   5,   1,   6,   1,   6,   1,   6,   1,
      6,   1,   6,   1,   6,   3,   6,   87,  8,   6,   1,   7,   1,   7,   1,
      7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,
      1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   3,   7,   105,
      8,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1,
      7,   1,   7,   1,   7,   5,   7,   116, 8,   7,   10,  7,   12,  7,   119,
      9,   7,   1,   8,   1,   8,   1,   8,   3,   8,   124, 8,   8,   1,   8,
      1,   8,   1,   9,   1,   9,   1,   9,   5,   9,   131, 8,   9,   10,  9,
      12,  9,   134, 9,   9,   1,   10,  1,   10,  1,   11,  1,   11,  1,   12,
      1,   12,  1,   13,  1,   13,  1,   13,  1,   13,  1,   13,  1,   13,  1,
      13,  1,   13,  5,   13,  150, 8,   13,  10,  13,  12,  13,  153, 9,   13,
      1,   14,  1,   14,  1,   14,  1,   14,  5,   14,  159, 8,   14,  10,  14,
      12,  14,  162, 9,   14,  1,   15,  1,   15,  1,   15,  1,   15,  5,   15,
      168, 8,   15,  10,  15,  12,  15,  171, 9,   15,  1,   16,  1,   16,  1,
      16,  1,   16,  1,   16,  1,   16,  1,   17,  1,   17,  1,   17,  0,   1,
      14,  18,  0,   2,   4,   6,   8,   10,  12,  14,  16,  18,  20,  22,  24,
      26,  28,  30,  32,  34,  0,   5,   1,   0,   3,   5,   1,   0,   1,   2,
      1,   0,   6,   11,  1,   0,   37,  38,  1,   0,   20,  21,  187, 0,   39,
      1,   0,   0,   0,   2,   44,  1,   0,   0,   0,   4,   49,  1,   0,   0,
      0,   6,   59,  1,   0,   0,   0,   8,   63,  1,   0,   0,   0,   10,  77,
      1,   0,   0,   0,   12,  86,  1,   0,   0,   0,   14,  104, 1,   0,   0,
      0,   16,  120, 1,   0,   0,   0,   18,  127, 1,   0,   0,   0,   20,  135,
      1,   0,   0,   0,   22,  137, 1,   0,   0,   0,   24,  139, 1,   0,   0,
      0,   26,  141, 1,   0,   0,   0,   28,  154, 1,   0,   0,   0,   30,  163,
      1,   0,   0,   0,   32,  172, 1,   0,   0,   0,   34,  178, 1,   0,   0,
      0,   36,  38,  3,   2,   1,   0,   37,  36,  1,   0,   0,   0,   38,  41,
      1,   0,   0,   0,   39,  37,  1,   0,   0,   0,   39,  40,  1,   0,   0,
      0,   40,  42,  1,   0,   0,   0,   41,  39,  1,   0,   0,   0,   42,  43,
      5,   0,   0,   1,   43,  1,   1,   0,   0,   0,   44,  45,  5,   22,  0,
      0,   45,  46,  3,   4,   2,   0,   46,  47,  3,   6,   3,   0,   47,  3,
      1,   0,   0,   0,   48,  50,  3,   24,  12,  0,   49,  48,  1,   0,   0,
      0,   49,  50,  1,   0,   0,   0,   50,  51,  1,   0,   0,   0,   51,  57,
      5,   36,  0,   0,   52,  54,  5,   13,  0,   0,   53,  55,  3,   8,   4,
      0,   54,  53,  1,   0,   0,   0,   54,  55,  1,   0,   0,   0,   55,  56,
      1,   0,   0,   0,   56,  58,  5,   14,  0,   0,   57,  52,  1,   0,   0,
      0,   57,  58,  1,   0,   0,   0,   58,  5,   1,   0,   0,   0,   59,  60,
      5,   23,  0,   0,   60,  61,  3,   10,  5,   0,   61,  62,  5,   24,  0,
      0,   62,  7,   1,   0,   0,   0,   63,  64,  3,   24,  12,  0,   64,  71,
      5,   36,  0,   0,   65,  66,  5,   19,  0,   0,   66,  67,  3,   24,  12,
      0,   67,  68,  5,   36,  0,   0,   68,  70,  1,   0,   0,   0,   69,  65,
      1,   0,   0,   0,   70,  73,  1,   0,   0,   0,   71,  69,  1,   0,   0,
      0,   71,  72,  1,   0,   0,   0,   72,  9,   1,   0,   0,   0,   73,  71,
      1,   0,   0,   0,   74,  76,  3,   12,  6,   0,   75,  74,  1,   0,   0,
      0,   76,  79,  1,   0,   0,   0,   77,  75,  1,   0,   0,   0,   77,  78,
      1,   0,   0,   0,   78,  11,  1,   0,   0,   0,   79,  77,  1,   0,   0,
      0,   80,  87,  3,   26,  13,  0,   81,  87,  3,   32,  16,  0,   82,  87,
      3,   28,  14,  0,   83,  87,  3,   30,  15,  0,   84,  87,  3,   14,  7,
      0,   85,  87,  3,   34,  17,  0,   86,  80,  1,   0,   0,   0,   86,  81,
      1,   0,   0,   0,   86,  82,  1,   0,   0,   0,   86,  83,  1,   0,   0,
      0,   86,  84,  1,   0,   0,   0,   86,  85,  1,   0,   0,   0,   87,  13,
      1,   0,   0,   0,   88,  89,  6,   7,   -1,  0,   89,  90,  5,   13,  0,
      0,   90,  91,  3,   14,  7,   0,   91,  92,  5,   14,  0,   0,   92,  105,
      1,   0,   0,   0,   93,  94,  5,   2,   0,   0,   94,  105, 3,   14,  7,
      9,   95,  105, 3,   16,  8,   0,   96,  105, 5,   36,  0,   0,   97,  98,
      5,   36,  0,   0,   98,  99,  5,   12,  0,   0,   99,  105, 3,   14,  7,
      3,   100, 101, 5,   32,  0,   0,   101, 102, 5,   12,  0,   0,   102, 105,
      3,   14,  7,   2,   103, 105, 3,   20,  10,  0,   104, 88,  1,   0,   0,
      0,   104, 93,  1,   0,   0,   0,   104, 95,  1,   0,   0,   0,   104, 96,
      1,   0,   0,   0,   104, 97,  1,   0,   0,   0,   104, 100, 1,   0,   0,
      0,   104, 103, 1,   0,   0,   0,   105, 117, 1,   0,   0,   0,   106, 107,
      10,  6,   0,   0,   107, 108, 7,   0,   0,   0,   108, 116, 3,   14,  7,
      7,   109, 110, 10,  5,   0,   0,   110, 111, 7,   1,   0,   0,   111, 116,
      3,   14,  7,   6,   112, 113, 10,  4,   0,   0,   113, 114, 7,   2,   0,
      0,   114, 116, 3,   14,  7,   5,   115, 106, 1,   0,   0,   0,   115, 109,
      1,   0,   0,   0,   115, 112, 1,   0,   0,   0,   116, 119, 1,   0,   0,
      0,   117, 115, 1,   0,   0,   0,   117, 118, 1,   0,   0,   0,   118, 15,
      1,   0,   0,   0,   119, 117, 1,   0,   0,   0,   120, 121, 5,   36,  0,
      0,   121, 123, 5,   13,  0,   0,   122, 124, 3,   18,  9,   0,   123, 122,
      1,   0,   0,   0,   123, 124, 1,   0,   0,   0,   124, 125, 1,   0,   0,
      0,   125, 126, 5,   14,  0,   0,   126, 17,  1,   0,   0,   0,   127, 132,
      3,   14,  7,   0,   128, 129, 5,   19,  0,   0,   129, 131, 3,   14,  7,
      0,   130, 128, 1,   0,   0,   0,   131, 134, 1,   0,   0,   0,   132, 130,
      1,   0,   0,   0,   132, 133, 1,   0,   0,   0,   133, 19,  1,   0,   0,
      0,   134, 132, 1,   0,   0,   0,   135, 136, 3,   22,  11,  0,   136, 21,
      1,   0,   0,   0,   137, 138, 7,   3,   0,   0,   138, 23,  1,   0,   0,
      0,   139, 140, 7,   4,   0,   0,   140, 25,  1,   0,   0,   0,   141, 142,
      3,   24,  12,  0,   142, 143, 5,   36,  0,   0,   143, 144, 5,   12,  0,
      0,   144, 151, 3,   14,  7,   0,   145, 146, 5,   19,  0,   0,   146, 147,
      5,   36,  0,   0,   147, 148, 5,   12,  0,   0,   148, 150, 3,   14,  7,
      0,   149, 145, 1,   0,   0,   0,   150, 153, 1,   0,   0,   0,   151, 149,
      1,   0,   0,   0,   151, 152, 1,   0,   0,   0,   152, 27,  1,   0,   0,
      0,   153, 151, 1,   0,   0,   0,   154, 155, 5,   26,  0,   0,   155, 160,
      3,   14,  7,   0,   156, 157, 5,   19,  0,   0,   157, 159, 3,   14,  7,
      0,   158, 156, 1,   0,   0,   0,   159, 162, 1,   0,   0,   0,   160, 158,
      1,   0,   0,   0,   160, 161, 1,   0,   0,   0,   161, 29,  1,   0,   0,
      0,   162, 160, 1,   0,   0,   0,   163, 164, 5,   25,  0,   0,   164, 169,
      5,   36,  0,   0,   165, 166, 5,   19,  0,   0,   166, 168, 5,   36,  0,
      0,   167, 165, 1,   0,   0,   0,   168, 171, 1,   0,   0,   0,   169, 167,
      1,   0,   0,   0,   169, 170, 1,   0,   0,   0,   170, 31,  1,   0,   0,
      0,   171, 169, 1,   0,   0,   0,   172, 173, 5,   27,  0,   0,   173, 174,
      3,   14,  7,   0,   174, 175, 5,   28,  0,   0,   175, 176, 3,   10,  5,
      0,   176, 177, 5,   30,  0,   0,   177, 33,  1,   0,   0,   0,   178, 179,
      5,   31,  0,   0,   179, 35,  1,   0,   0,   0,   15,  39,  49,  54,  57,
      71,  77,  86,  104, 115, 117, 123, 132, 151, 160, 169};
  staticData->serializedATN = antlr4::atn::SerializedATNView(
      serializedATNSegment,
      sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) {
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i),
                                           i);
  }
  ralparserParserStaticData = staticData.release();
}

} // namespace

RalParser::RalParser(TokenStream *input)
    : RalParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

RalParser::RalParser(TokenStream *input,
                     const antlr4::atn::ParserATNSimulatorOptions &options)
    : Parser(input) {
  RalParser::initialize();
  _interpreter = new atn::ParserATNSimulator(
      this, *ralparserParserStaticData->atn,
      ralparserParserStaticData->decisionToDFA,
      ralparserParserStaticData->sharedContextCache, options);
}

RalParser::~RalParser() { delete _interpreter; }

const atn::ATN &RalParser::getATN() const {
  return *ralparserParserStaticData->atn;
}

std::string RalParser::getGrammarFileName() const { return "RalParser.g4"; }

const std::vector<std::string> &RalParser::getRuleNames() const {
  return ralparserParserStaticData->ruleNames;
}

const dfa::Vocabulary &RalParser::getVocabulary() const {
  return ralparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView RalParser::getSerializedATN() const {
  return ralparserParserStaticData->serializedATN;
}

//----------------- ModuleContext
//------------------------------------------------------------------

RalParser::ModuleContext::ModuleContext(ParserRuleContext *parent,
                                        size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::ModuleContext::EOF() {
  return getToken(RalParser::EOF, 0);
}

std::vector<RalParser::AlgorithmContext *>
RalParser::ModuleContext::algorithm() {
  return getRuleContexts<RalParser::AlgorithmContext>();
}

RalParser::AlgorithmContext *RalParser::ModuleContext::algorithm(size_t i) {
  return getRuleContext<RalParser::AlgorithmContext>(i);
}

size_t RalParser::ModuleContext::getRuleIndex() const {
  return RalParser::RuleModule;
}

std::any RalParser::ModuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitModule(this);
  else
    return visitor->visitChildren(this);
}

RalParser::ModuleContext *RalParser::module() {
  ModuleContext *_localctx =
      _tracker.createInstance<ModuleContext>(_ctx, getState());
  enterRule(_localctx, 0, RalParser::RuleModule);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RalParser::AlgorhitmHeader) {
      setState(36);
      algorithm();
      setState(41);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(42);
    match(RalParser::EOF);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AlgorithmContext
//------------------------------------------------------------------

RalParser::AlgorithmContext::AlgorithmContext(ParserRuleContext *parent,
                                              size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::AlgorithmContext::AlgorhitmHeader() {
  return getToken(RalParser::AlgorhitmHeader, 0);
}

RalParser::AlgorithmPrototypeContext *
RalParser::AlgorithmContext::algorithmPrototype() {
  return getRuleContext<RalParser::AlgorithmPrototypeContext>(0);
}

RalParser::AlgorithmBodyContext *RalParser::AlgorithmContext::algorithmBody() {
  return getRuleContext<RalParser::AlgorithmBodyContext>(0);
}

size_t RalParser::AlgorithmContext::getRuleIndex() const {
  return RalParser::RuleAlgorithm;
}

std::any RalParser::AlgorithmContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitAlgorithm(this);
  else
    return visitor->visitChildren(this);
}

RalParser::AlgorithmContext *RalParser::algorithm() {
  AlgorithmContext *_localctx =
      _tracker.createInstance<AlgorithmContext>(_ctx, getState());
  enterRule(_localctx, 2, RalParser::RuleAlgorithm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(44);
    match(RalParser::AlgorhitmHeader);
    setState(45);
    algorithmPrototype();
    setState(46);
    algorithmBody();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AlgorithmPrototypeContext
//------------------------------------------------------------------

RalParser::AlgorithmPrototypeContext::AlgorithmPrototypeContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::AlgorithmPrototypeContext::Id() {
  return getToken(RalParser::Id, 0);
}

RalParser::TypeContext *RalParser::AlgorithmPrototypeContext::type() {
  return getRuleContext<RalParser::TypeContext>(0);
}

tree::TerminalNode *RalParser::AlgorithmPrototypeContext::OpeningParen() {
  return getToken(RalParser::OpeningParen, 0);
}

tree::TerminalNode *RalParser::AlgorithmPrototypeContext::ClosingParen() {
  return getToken(RalParser::ClosingParen, 0);
}

RalParser::FormalParametersContext *
RalParser::AlgorithmPrototypeContext::formalParameters() {
  return getRuleContext<RalParser::FormalParametersContext>(0);
}

size_t RalParser::AlgorithmPrototypeContext::getRuleIndex() const {
  return RalParser::RuleAlgorithmPrototype;
}

std::any
RalParser::AlgorithmPrototypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitAlgorithmPrototype(this);
  else
    return visitor->visitChildren(this);
}

RalParser::AlgorithmPrototypeContext *RalParser::algorithmPrototype() {
  AlgorithmPrototypeContext *_localctx =
      _tracker.createInstance<AlgorithmPrototypeContext>(_ctx, getState());
  enterRule(_localctx, 4, RalParser::RuleAlgorithmPrototype);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(49);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == RalParser::IntegerTypeName

        || _la == RalParser::FloatingPointTypeName) {
      setState(48);
      type();
    }
    setState(51);
    match(RalParser::Id);
    setState(57);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == RalParser::OpeningParen) {
      setState(52);
      match(RalParser::OpeningParen);
      setState(54);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == RalParser::IntegerTypeName

          || _la == RalParser::FloatingPointTypeName) {
        setState(53);
        formalParameters();
      }
      setState(56);
      match(RalParser::ClosingParen);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AlgorithmBodyContext
//------------------------------------------------------------------

RalParser::AlgorithmBodyContext::AlgorithmBodyContext(ParserRuleContext *parent,
                                                      size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *
RalParser::AlgorithmBodyContext::BeginAlgorhitmImplementation() {
  return getToken(RalParser::BeginAlgorhitmImplementation, 0);
}

RalParser::InstructionsContext *
RalParser::AlgorithmBodyContext::instructions() {
  return getRuleContext<RalParser::InstructionsContext>(0);
}

tree::TerminalNode *
RalParser::AlgorithmBodyContext::EndAlgorhitmImplementation() {
  return getToken(RalParser::EndAlgorhitmImplementation, 0);
}

size_t RalParser::AlgorithmBodyContext::getRuleIndex() const {
  return RalParser::RuleAlgorithmBody;
}

std::any
RalParser::AlgorithmBodyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitAlgorithmBody(this);
  else
    return visitor->visitChildren(this);
}

RalParser::AlgorithmBodyContext *RalParser::algorithmBody() {
  AlgorithmBodyContext *_localctx =
      _tracker.createInstance<AlgorithmBodyContext>(_ctx, getState());
  enterRule(_localctx, 6, RalParser::RuleAlgorithmBody);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(59);
    match(RalParser::BeginAlgorhitmImplementation);
    setState(60);
    instructions();
    setState(61);
    match(RalParser::EndAlgorhitmImplementation);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParametersContext
//------------------------------------------------------------------

RalParser::FormalParametersContext::FormalParametersContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<RalParser::TypeContext *>
RalParser::FormalParametersContext::type() {
  return getRuleContexts<RalParser::TypeContext>();
}

RalParser::TypeContext *RalParser::FormalParametersContext::type(size_t i) {
  return getRuleContext<RalParser::TypeContext>(i);
}

std::vector<tree::TerminalNode *> RalParser::FormalParametersContext::Id() {
  return getTokens(RalParser::Id);
}

tree::TerminalNode *RalParser::FormalParametersContext::Id(size_t i) {
  return getToken(RalParser::Id, i);
}

std::vector<tree::TerminalNode *> RalParser::FormalParametersContext::Comma() {
  return getTokens(RalParser::Comma);
}

tree::TerminalNode *RalParser::FormalParametersContext::Comma(size_t i) {
  return getToken(RalParser::Comma, i);
}

size_t RalParser::FormalParametersContext::getRuleIndex() const {
  return RalParser::RuleFormalParameters;
}

std::any
RalParser::FormalParametersContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitFormalParameters(this);
  else
    return visitor->visitChildren(this);
}

RalParser::FormalParametersContext *RalParser::formalParameters() {
  FormalParametersContext *_localctx =
      _tracker.createInstance<FormalParametersContext>(_ctx, getState());
  enterRule(_localctx, 8, RalParser::RuleFormalParameters);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(63);
    type();
    setState(64);
    match(RalParser::Id);
    setState(71);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RalParser::Comma) {
      setState(65);
      match(RalParser::Comma);
      setState(66);
      type();
      setState(67);
      match(RalParser::Id);
      setState(73);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InstructionsContext
//------------------------------------------------------------------

RalParser::InstructionsContext::InstructionsContext(ParserRuleContext *parent,
                                                    size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<RalParser::StatementContext *>
RalParser::InstructionsContext::statement() {
  return getRuleContexts<RalParser::StatementContext>();
}

RalParser::StatementContext *
RalParser::InstructionsContext::statement(size_t i) {
  return getRuleContext<RalParser::StatementContext>(i);
}

size_t RalParser::InstructionsContext::getRuleIndex() const {
  return RalParser::RuleInstructions;
}

std::any
RalParser::InstructionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitInstructions(this);
  else
    return visitor->visitChildren(this);
}

RalParser::InstructionsContext *RalParser::instructions() {
  InstructionsContext *_localctx =
      _tracker.createInstance<InstructionsContext>(_ctx, getState());
  enterRule(_localctx, 10, RalParser::RuleInstructions);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(77);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 487716823044) != 0)) {
      setState(74);
      statement();
      setState(79);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext
//------------------------------------------------------------------

RalParser::StatementContext::StatementContext(ParserRuleContext *parent,
                                              size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

RalParser::VariableDeclarationContext *
RalParser::StatementContext::variableDeclaration() {
  return getRuleContext<RalParser::VariableDeclarationContext>(0);
}

RalParser::IfStatementContext *RalParser::StatementContext::ifStatement() {
  return getRuleContext<RalParser::IfStatementContext>(0);
}

RalParser::PrintStatementContext *
RalParser::StatementContext::printStatement() {
  return getRuleContext<RalParser::PrintStatementContext>(0);
}

RalParser::InputStatementContext *
RalParser::StatementContext::inputStatement() {
  return getRuleContext<RalParser::InputStatementContext>(0);
}

RalParser::ExpressionContext *RalParser::StatementContext::expression() {
  return getRuleContext<RalParser::ExpressionContext>(0);
}

RalParser::ReturnStatementContext *
RalParser::StatementContext::returnStatement() {
  return getRuleContext<RalParser::ReturnStatementContext>(0);
}

size_t RalParser::StatementContext::getRuleIndex() const {
  return RalParser::RuleStatement;
}

std::any RalParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

RalParser::StatementContext *RalParser::statement() {
  StatementContext *_localctx =
      _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 12, RalParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(86);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
    case RalParser::IntegerTypeName:
    case RalParser::FloatingPointTypeName: {
      enterOuterAlt(_localctx, 1);
      setState(80);
      variableDeclaration();
      break;
    }

    case RalParser::If: {
      enterOuterAlt(_localctx, 2);
      setState(81);
      ifStatement();
      break;
    }

    case RalParser::TerminalOutput: {
      enterOuterAlt(_localctx, 3);
      setState(82);
      printStatement();
      break;
    }

    case RalParser::TerminalInput: {
      enterOuterAlt(_localctx, 4);
      setState(83);
      inputStatement();
      break;
    }

    case RalParser::Sub:
    case RalParser::OpeningParen:
    case RalParser::FunctionReturnValue:
    case RalParser::Id:
    case RalParser::DecimalLiteral:
    case RalParser::ZeroLiteral: {
      enterOuterAlt(_localctx, 5);
      setState(84);
      expression(0);
      break;
    }

    case RalParser::LoopBreakAndAlgorhitmReturn: {
      enterOuterAlt(_localctx, 6);
      setState(85);
      returnStatement();
      break;
    }

    default:
      throw NoViableAltException(this);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext
//------------------------------------------------------------------

RalParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent,
                                                size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t RalParser::ExpressionContext::getRuleIndex() const {
  return RalParser::RuleExpression;
}

void RalParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FunctionAffectationContext
//------------------------------------------------------------------

tree::TerminalNode *
RalParser::FunctionAffectationContext::FunctionReturnValue() {
  return getToken(RalParser::FunctionReturnValue, 0);
}

tree::TerminalNode *RalParser::FunctionAffectationContext::Equal() {
  return getToken(RalParser::Equal, 0);
}

RalParser::ExpressionContext *
RalParser::FunctionAffectationContext::expression() {
  return getRuleContext<RalParser::ExpressionContext>(0);
}

RalParser::FunctionAffectationContext::FunctionAffectationContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any
RalParser::FunctionAffectationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitFunctionAffectation(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryNegativeExpressionContext
//------------------------------------------------------------------

tree::TerminalNode *RalParser::UnaryNegativeExpressionContext::Sub() {
  return getToken(RalParser::Sub, 0);
}

RalParser::ExpressionContext *
RalParser::UnaryNegativeExpressionContext::expression() {
  return getRuleContext<RalParser::ExpressionContext>(0);
}

RalParser::UnaryNegativeExpressionContext::UnaryNegativeExpressionContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any RalParser::UnaryNegativeExpressionContext::accept(
    tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitUnaryNegativeExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InParenExpressionContext
//------------------------------------------------------------------

tree::TerminalNode *RalParser::InParenExpressionContext::OpeningParen() {
  return getToken(RalParser::OpeningParen, 0);
}

RalParser::ExpressionContext *
RalParser::InParenExpressionContext::expression() {
  return getRuleContext<RalParser::ExpressionContext>(0);
}

tree::TerminalNode *RalParser::InParenExpressionContext::ClosingParen() {
  return getToken(RalParser::ClosingParen, 0);
}

RalParser::InParenExpressionContext::InParenExpressionContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any
RalParser::InParenExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitInParenExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryOperationContext
//------------------------------------------------------------------

std::vector<RalParser::ExpressionContext *>
RalParser::BinaryOperationContext::expression() {
  return getRuleContexts<RalParser::ExpressionContext>();
}

RalParser::ExpressionContext *
RalParser::BinaryOperationContext::expression(size_t i) {
  return getRuleContext<RalParser::ExpressionContext>(i);
}

tree::TerminalNode *RalParser::BinaryOperationContext::Add() {
  return getToken(RalParser::Add, 0);
}

tree::TerminalNode *RalParser::BinaryOperationContext::Sub() {
  return getToken(RalParser::Sub, 0);
}

RalParser::BinaryOperationContext::BinaryOperationContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any
RalParser::BinaryOperationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitBinaryOperation(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NameExpressionContext
//------------------------------------------------------------------

tree::TerminalNode *RalParser::NameExpressionContext::Id() {
  return getToken(RalParser::Id, 0);
}

RalParser::NameExpressionContext::NameExpressionContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any
RalParser::NameExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitNameExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LiteralExpressionContext
//------------------------------------------------------------------

RalParser::LiteralContext *RalParser::LiteralExpressionContext::literal() {
  return getRuleContext<RalParser::LiteralContext>(0);
}

RalParser::LiteralExpressionContext::LiteralExpressionContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any
RalParser::LiteralExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitLiteralExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryMultiplyOperationContext
//------------------------------------------------------------------

std::vector<RalParser::ExpressionContext *>
RalParser::BinaryMultiplyOperationContext::expression() {
  return getRuleContexts<RalParser::ExpressionContext>();
}

RalParser::ExpressionContext *
RalParser::BinaryMultiplyOperationContext::expression(size_t i) {
  return getRuleContext<RalParser::ExpressionContext>(i);
}

tree::TerminalNode *RalParser::BinaryMultiplyOperationContext::Mul() {
  return getToken(RalParser::Mul, 0);
}

tree::TerminalNode *RalParser::BinaryMultiplyOperationContext::Div() {
  return getToken(RalParser::Div, 0);
}

tree::TerminalNode *RalParser::BinaryMultiplyOperationContext::Mod() {
  return getToken(RalParser::Mod, 0);
}

RalParser::BinaryMultiplyOperationContext::BinaryMultiplyOperationContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any RalParser::BinaryMultiplyOperationContext::accept(
    tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitBinaryMultiplyOperation(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryConditionalOperationContext
//------------------------------------------------------------------

std::vector<RalParser::ExpressionContext *>
RalParser::BinaryConditionalOperationContext::expression() {
  return getRuleContexts<RalParser::ExpressionContext>();
}

RalParser::ExpressionContext *
RalParser::BinaryConditionalOperationContext::expression(size_t i) {
  return getRuleContext<RalParser::ExpressionContext>(i);
}

tree::TerminalNode *RalParser::BinaryConditionalOperationContext::Gt() {
  return getToken(RalParser::Gt, 0);
}

tree::TerminalNode *RalParser::BinaryConditionalOperationContext::Gte() {
  return getToken(RalParser::Gte, 0);
}

tree::TerminalNode *RalParser::BinaryConditionalOperationContext::Lt() {
  return getToken(RalParser::Lt, 0);
}

tree::TerminalNode *RalParser::BinaryConditionalOperationContext::Lte() {
  return getToken(RalParser::Lte, 0);
}

tree::TerminalNode *RalParser::BinaryConditionalOperationContext::Eq() {
  return getToken(RalParser::Eq, 0);
}

tree::TerminalNode *RalParser::BinaryConditionalOperationContext::Ne() {
  return getToken(RalParser::Ne, 0);
}

RalParser::BinaryConditionalOperationContext::BinaryConditionalOperationContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any RalParser::BinaryConditionalOperationContext::accept(
    tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitBinaryConditionalOperation(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FunctionCallExpressionContext
//------------------------------------------------------------------

RalParser::FunctionCallContext *
RalParser::FunctionCallExpressionContext::functionCall() {
  return getRuleContext<RalParser::FunctionCallContext>(0);
}

RalParser::FunctionCallExpressionContext::FunctionCallExpressionContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any RalParser::FunctionCallExpressionContext::accept(
    tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitFunctionCallExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VariableAffectationContext
//------------------------------------------------------------------

tree::TerminalNode *RalParser::VariableAffectationContext::Id() {
  return getToken(RalParser::Id, 0);
}

tree::TerminalNode *RalParser::VariableAffectationContext::Equal() {
  return getToken(RalParser::Equal, 0);
}

RalParser::ExpressionContext *
RalParser::VariableAffectationContext::expression() {
  return getRuleContext<RalParser::ExpressionContext>(0);
}

RalParser::VariableAffectationContext::VariableAffectationContext(
    ExpressionContext *ctx) {
  copyFrom(ctx);
}

std::any
RalParser::VariableAffectationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitVariableAffectation(this);
  else
    return visitor->visitChildren(this);
}

RalParser::ExpressionContext *RalParser::expression() { return expression(0); }

RalParser::ExpressionContext *RalParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  RalParser::ExpressionContext *_localctx =
      _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  RalParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by
                         // generated code.
  size_t startState = 14;
  enterRecursionRule(_localctx, 14, RalParser::RuleExpression, precedence);

  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(104);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(
        _input, 7, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<InParenExpressionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(89);
      match(RalParser::OpeningParen);
      setState(90);
      expression(0);
      setState(91);
      match(RalParser::ClosingParen);
      break;
    }

    case 2: {
      _localctx =
          _tracker.createInstance<UnaryNegativeExpressionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(93);
      match(RalParser::Sub);
      setState(94);
      expression(9);
      break;
    }

    case 3: {
      _localctx =
          _tracker.createInstance<FunctionCallExpressionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(95);
      functionCall();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<NameExpressionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(96);
      match(RalParser::Id);
      break;
    }

    case 5: {
      _localctx =
          _tracker.createInstance<VariableAffectationContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(97);
      match(RalParser::Id);
      setState(98);
      match(RalParser::Equal);
      setState(99);
      expression(3);
      break;
    }

    case 6: {
      _localctx =
          _tracker.createInstance<FunctionAffectationContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(100);
      match(RalParser::FunctionReturnValue);
      setState(101);
      match(RalParser::Equal);
      setState(102);
      expression(2);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<LiteralExpressionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(103);
      literal();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(117);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9,
                                                                     _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(115);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(
            _input, 8, _ctx)) {
        case 1: {
          auto newContext =
              _tracker.createInstance<BinaryMultiplyOperationContext>(
                  _tracker.createInstance<ExpressionContext>(parentContext,
                                                             parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(106);

          if (!(precpred(_ctx, 6)))
            throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(107);
          _la = _input->LA(1);
          if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 56) != 0))) {
            _errHandler->recoverInline(this);
          } else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(108);
          expression(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOperationContext>(
              _tracker.createInstance<ExpressionContext>(parentContext,
                                                         parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(109);

          if (!(precpred(_ctx, 5)))
            throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(110);
          _la = _input->LA(1);
          if (!(_la == RalParser::Add

                || _la == RalParser::Sub)) {
            _errHandler->recoverInline(this);
          } else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(111);
          expression(6);
          break;
        }

        case 3: {
          auto newContext =
              _tracker.createInstance<BinaryConditionalOperationContext>(
                  _tracker.createInstance<ExpressionContext>(parentContext,
                                                             parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(112);

          if (!(precpred(_ctx, 4)))
            throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(113);
          _la = _input->LA(1);
          if (!((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 4032) != 0))) {
            _errHandler->recoverInline(this);
          } else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(114);
          expression(5);
          break;
        }

        default:
          break;
        }
      }
      setState(119);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input,
                                                                       9, _ctx);
    }
  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- FunctionCallContext
//------------------------------------------------------------------

RalParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent,
                                                    size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::FunctionCallContext::Id() {
  return getToken(RalParser::Id, 0);
}

tree::TerminalNode *RalParser::FunctionCallContext::OpeningParen() {
  return getToken(RalParser::OpeningParen, 0);
}

tree::TerminalNode *RalParser::FunctionCallContext::ClosingParen() {
  return getToken(RalParser::ClosingParen, 0);
}

RalParser::ArgsContext *RalParser::FunctionCallContext::args() {
  return getRuleContext<RalParser::ArgsContext>(0);
}

size_t RalParser::FunctionCallContext::getRuleIndex() const {
  return RalParser::RuleFunctionCall;
}

std::any
RalParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

RalParser::FunctionCallContext *RalParser::functionCall() {
  FunctionCallContext *_localctx =
      _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 16, RalParser::RuleFunctionCall);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(120);
    match(RalParser::Id);
    setState(121);
    match(RalParser::OpeningParen);
    setState(123);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~0x3fULL) == 0) && ((1ULL << _la) & 485331312644) != 0)) {
      setState(122);
      args();
    }
    setState(125);
    match(RalParser::ClosingParen);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgsContext
//------------------------------------------------------------------

RalParser::ArgsContext::ArgsContext(ParserRuleContext *parent,
                                    size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

std::vector<RalParser::ExpressionContext *>
RalParser::ArgsContext::expression() {
  return getRuleContexts<RalParser::ExpressionContext>();
}

RalParser::ExpressionContext *RalParser::ArgsContext::expression(size_t i) {
  return getRuleContext<RalParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> RalParser::ArgsContext::Comma() {
  return getTokens(RalParser::Comma);
}

tree::TerminalNode *RalParser::ArgsContext::Comma(size_t i) {
  return getToken(RalParser::Comma, i);
}

size_t RalParser::ArgsContext::getRuleIndex() const {
  return RalParser::RuleArgs;
}

std::any RalParser::ArgsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitArgs(this);
  else
    return visitor->visitChildren(this);
}

RalParser::ArgsContext *RalParser::args() {
  ArgsContext *_localctx =
      _tracker.createInstance<ArgsContext>(_ctx, getState());
  enterRule(_localctx, 18, RalParser::RuleArgs);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127);
    expression(0);
    setState(132);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RalParser::Comma) {
      setState(128);
      match(RalParser::Comma);
      setState(129);
      expression(0);
      setState(134);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext
//------------------------------------------------------------------

RalParser::LiteralContext::LiteralContext(ParserRuleContext *parent,
                                          size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

RalParser::IntegerLiteralContext *RalParser::LiteralContext::integerLiteral() {
  return getRuleContext<RalParser::IntegerLiteralContext>(0);
}

size_t RalParser::LiteralContext::getRuleIndex() const {
  return RalParser::RuleLiteral;
}

std::any RalParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

RalParser::LiteralContext *RalParser::literal() {
  LiteralContext *_localctx =
      _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 20, RalParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(135);
    integerLiteral();

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerLiteralContext
//------------------------------------------------------------------

RalParser::IntegerLiteralContext::IntegerLiteralContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::IntegerLiteralContext::DecimalLiteral() {
  return getToken(RalParser::DecimalLiteral, 0);
}

tree::TerminalNode *RalParser::IntegerLiteralContext::ZeroLiteral() {
  return getToken(RalParser::ZeroLiteral, 0);
}

size_t RalParser::IntegerLiteralContext::getRuleIndex() const {
  return RalParser::RuleIntegerLiteral;
}

std::any
RalParser::IntegerLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitIntegerLiteral(this);
  else
    return visitor->visitChildren(this);
}

RalParser::IntegerLiteralContext *RalParser::integerLiteral() {
  IntegerLiteralContext *_localctx =
      _tracker.createInstance<IntegerLiteralContext>(_ctx, getState());
  enterRule(_localctx, 22, RalParser::RuleIntegerLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(137);
    _la = _input->LA(1);
    if (!(_la == RalParser::DecimalLiteral

          || _la == RalParser::ZeroLiteral)) {
      _errHandler->recoverInline(this);
    } else {
      _errHandler->reportMatch(this);
      consume();
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext
//------------------------------------------------------------------

RalParser::TypeContext::TypeContext(ParserRuleContext *parent,
                                    size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::TypeContext::IntegerTypeName() {
  return getToken(RalParser::IntegerTypeName, 0);
}

tree::TerminalNode *RalParser::TypeContext::FloatingPointTypeName() {
  return getToken(RalParser::FloatingPointTypeName, 0);
}

size_t RalParser::TypeContext::getRuleIndex() const {
  return RalParser::RuleType;
}

std::any RalParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

RalParser::TypeContext *RalParser::type() {
  TypeContext *_localctx =
      _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 24, RalParser::RuleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139);
    _la = _input->LA(1);
    if (!(_la == RalParser::IntegerTypeName

          || _la == RalParser::FloatingPointTypeName)) {
      _errHandler->recoverInline(this);
    } else {
      _errHandler->reportMatch(this);
      consume();
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableDeclarationContext
//------------------------------------------------------------------

RalParser::VariableDeclarationContext::VariableDeclarationContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

RalParser::TypeContext *RalParser::VariableDeclarationContext::type() {
  return getRuleContext<RalParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> RalParser::VariableDeclarationContext::Id() {
  return getTokens(RalParser::Id);
}

tree::TerminalNode *RalParser::VariableDeclarationContext::Id(size_t i) {
  return getToken(RalParser::Id, i);
}

std::vector<tree::TerminalNode *>
RalParser::VariableDeclarationContext::Equal() {
  return getTokens(RalParser::Equal);
}

tree::TerminalNode *RalParser::VariableDeclarationContext::Equal(size_t i) {
  return getToken(RalParser::Equal, i);
}

std::vector<RalParser::ExpressionContext *>
RalParser::VariableDeclarationContext::expression() {
  return getRuleContexts<RalParser::ExpressionContext>();
}

RalParser::ExpressionContext *
RalParser::VariableDeclarationContext::expression(size_t i) {
  return getRuleContext<RalParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *>
RalParser::VariableDeclarationContext::Comma() {
  return getTokens(RalParser::Comma);
}

tree::TerminalNode *RalParser::VariableDeclarationContext::Comma(size_t i) {
  return getToken(RalParser::Comma, i);
}

size_t RalParser::VariableDeclarationContext::getRuleIndex() const {
  return RalParser::RuleVariableDeclaration;
}

std::any
RalParser::VariableDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitVariableDeclaration(this);
  else
    return visitor->visitChildren(this);
}

RalParser::VariableDeclarationContext *RalParser::variableDeclaration() {
  VariableDeclarationContext *_localctx =
      _tracker.createInstance<VariableDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 26, RalParser::RuleVariableDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(141);
    type();
    setState(142);
    match(RalParser::Id);
    setState(143);
    match(RalParser::Equal);
    setState(144);
    expression(0);
    setState(151);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RalParser::Comma) {
      setState(145);
      match(RalParser::Comma);
      setState(146);
      match(RalParser::Id);
      setState(147);
      match(RalParser::Equal);
      setState(148);
      expression(0);
      setState(153);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrintStatementContext
//------------------------------------------------------------------

RalParser::PrintStatementContext::PrintStatementContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::PrintStatementContext::TerminalOutput() {
  return getToken(RalParser::TerminalOutput, 0);
}

std::vector<RalParser::ExpressionContext *>
RalParser::PrintStatementContext::expression() {
  return getRuleContexts<RalParser::ExpressionContext>();
}

RalParser::ExpressionContext *
RalParser::PrintStatementContext::expression(size_t i) {
  return getRuleContext<RalParser::ExpressionContext>(i);
}

std::vector<tree::TerminalNode *> RalParser::PrintStatementContext::Comma() {
  return getTokens(RalParser::Comma);
}

tree::TerminalNode *RalParser::PrintStatementContext::Comma(size_t i) {
  return getToken(RalParser::Comma, i);
}

size_t RalParser::PrintStatementContext::getRuleIndex() const {
  return RalParser::RulePrintStatement;
}

std::any
RalParser::PrintStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitPrintStatement(this);
  else
    return visitor->visitChildren(this);
}

RalParser::PrintStatementContext *RalParser::printStatement() {
  PrintStatementContext *_localctx =
      _tracker.createInstance<PrintStatementContext>(_ctx, getState());
  enterRule(_localctx, 28, RalParser::RulePrintStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(154);
    match(RalParser::TerminalOutput);
    setState(155);
    expression(0);
    setState(160);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RalParser::Comma) {
      setState(156);
      match(RalParser::Comma);
      setState(157);
      expression(0);
      setState(162);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InputStatementContext
//------------------------------------------------------------------

RalParser::InputStatementContext::InputStatementContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::InputStatementContext::TerminalInput() {
  return getToken(RalParser::TerminalInput, 0);
}

std::vector<tree::TerminalNode *> RalParser::InputStatementContext::Id() {
  return getTokens(RalParser::Id);
}

tree::TerminalNode *RalParser::InputStatementContext::Id(size_t i) {
  return getToken(RalParser::Id, i);
}

std::vector<tree::TerminalNode *> RalParser::InputStatementContext::Comma() {
  return getTokens(RalParser::Comma);
}

tree::TerminalNode *RalParser::InputStatementContext::Comma(size_t i) {
  return getToken(RalParser::Comma, i);
}

size_t RalParser::InputStatementContext::getRuleIndex() const {
  return RalParser::RuleInputStatement;
}

std::any
RalParser::InputStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitInputStatement(this);
  else
    return visitor->visitChildren(this);
}

RalParser::InputStatementContext *RalParser::inputStatement() {
  InputStatementContext *_localctx =
      _tracker.createInstance<InputStatementContext>(_ctx, getState());
  enterRule(_localctx, 30, RalParser::RuleInputStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(163);
    match(RalParser::TerminalInput);
    setState(164);
    match(RalParser::Id);
    setState(169);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RalParser::Comma) {
      setState(165);
      match(RalParser::Comma);
      setState(166);
      match(RalParser::Id);
      setState(171);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStatementContext
//------------------------------------------------------------------

RalParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent,
                                                  size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *RalParser::IfStatementContext::If() {
  return getToken(RalParser::If, 0);
}

RalParser::ExpressionContext *RalParser::IfStatementContext::expression() {
  return getRuleContext<RalParser::ExpressionContext>(0);
}

tree::TerminalNode *RalParser::IfStatementContext::Then() {
  return getToken(RalParser::Then, 0);
}

RalParser::InstructionsContext *RalParser::IfStatementContext::instructions() {
  return getRuleContext<RalParser::InstructionsContext>(0);
}

tree::TerminalNode *RalParser::IfStatementContext::EndOfIfOrSwitchStatement() {
  return getToken(RalParser::EndOfIfOrSwitchStatement, 0);
}

size_t RalParser::IfStatementContext::getRuleIndex() const {
  return RalParser::RuleIfStatement;
}

std::any
RalParser::IfStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitIfStatement(this);
  else
    return visitor->visitChildren(this);
}

RalParser::IfStatementContext *RalParser::ifStatement() {
  IfStatementContext *_localctx =
      _tracker.createInstance<IfStatementContext>(_ctx, getState());
  enterRule(_localctx, 32, RalParser::RuleIfStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(RalParser::If);
    setState(173);
    expression(0);
    setState(174);
    match(RalParser::Then);
    setState(175);
    instructions();
    setState(176);
    match(RalParser::EndOfIfOrSwitchStatement);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext
//------------------------------------------------------------------

RalParser::ReturnStatementContext::ReturnStatementContext(
    ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

tree::TerminalNode *
RalParser::ReturnStatementContext::LoopBreakAndAlgorhitmReturn() {
  return getToken(RalParser::LoopBreakAndAlgorhitmReturn, 0);
}

size_t RalParser::ReturnStatementContext::getRuleIndex() const {
  return RalParser::RuleReturnStatement;
}

std::any
RalParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RalParserVisitor *>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}

RalParser::ReturnStatementContext *RalParser::returnStatement() {
  ReturnStatementContext *_localctx =
      _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 34, RalParser::RuleReturnStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(178);
    match(RalParser::LoopBreakAndAlgorhitmReturn);

  } catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool RalParser::sempred(RuleContext *context, size_t ruleIndex,
                        size_t predicateIndex) {
  switch (ruleIndex) {
  case 7:
    return expressionSempred(antlrcpp::downCast<ExpressionContext *>(context),
                             predicateIndex);

  default:
    break;
  }
  return true;
}

bool RalParser::expressionSempred(ExpressionContext *_localctx,
                                  size_t predicateIndex) {
  switch (predicateIndex) {
  case 0:
    return precpred(_ctx, 6);
  case 1:
    return precpred(_ctx, 5);
  case 2:
    return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

void RalParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ralparserParserInitialize();
#else
  ::antlr4::internal::call_once(ralparserParserOnceFlag,
                                ralparserParserInitialize);
#endif
}
