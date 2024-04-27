
// Generated from /home/blex/projects/ral/src/grammar/RalLexer.g4 by
// ANTLR 4.13.1

#include "RalLexer.h"

using namespace antlr4;

using namespace antlr4;

namespace {

struct RalLexerStaticData final {
  RalLexerStaticData(std::vector<std::string> ruleNames,
                     std::vector<std::string> channelNames,
                     std::vector<std::string> modeNames,
                     std::vector<std::string> literalNames,
                     std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  RalLexerStaticData(const RalLexerStaticData &) = delete;
  RalLexerStaticData(RalLexerStaticData &&) = delete;
  RalLexerStaticData &operator=(const RalLexerStaticData &) = delete;
  RalLexerStaticData &operator=(RalLexerStaticData &&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag rallexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
    RalLexerStaticData *rallexerLexerStaticData = nullptr;

void rallexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (rallexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(rallexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<RalLexerStaticData>(
      std::vector<std::string>{"Add",
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
                               "Name",
                               "DecimalLiteral",
                               "ZeroLiteral",
                               "Comment",
                               "CommentLine",
                               "WhiteSpace",
                               "LineTerminator",
                               "Escape",
                               "DIGIT",
                               "NONZERODIGIT"},
      std::vector<std::string>{"DEFAULT_TOKEN_CHANNEL", "HIDDEN"},
      std::vector<std::string>{"DEFAULT_MODE"},
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
      4,    0,   41,  262, 6,   -1,  2,    0,   7,    0,    2,   1,
      7,    1,   2,   2,   7,   2,   2,    3,   7,    3,    2,   4,
      7,    4,   2,   5,   7,   5,   2,    6,   7,    6,    2,   7,
      7,    7,   2,   8,   7,   8,   2,    9,   7,    9,    2,   10,
      7,    10,  2,   11,  7,   11,  2,    12,  7,    12,   2,   13,
      7,    13,  2,   14,  7,   14,  2,    15,  7,    15,   2,   16,
      7,    16,  2,   17,  7,   17,  2,    18,  7,    18,   2,   19,
      7,    19,  2,   20,  7,   20,  2,    21,  7,    21,   2,   22,
      7,    22,  2,   23,  7,   23,  2,    24,  7,    24,   2,   25,
      7,    25,  2,   26,  7,   26,  2,    27,  7,    27,   2,   28,
      7,    28,  2,   29,  7,   29,  2,    30,  7,    30,   2,   31,
      7,    31,  2,   32,  7,   32,  2,    33,  7,    33,   2,   34,
      7,    34,  2,   35,  7,   35,  2,    36,  7,    36,   2,   37,
      7,    37,  2,   38,  7,   38,  2,    39,  7,    39,   2,   40,
      7,    40,  2,   41,  7,   41,  2,    42,  7,    42,   2,   43,
      7,    43,  2,   44,  7,   44,  2,    45,  7,    45,   1,   0,
      1,    0,   1,   1,   1,   1,   1,    2,   1,    2,    1,   3,
      1,    3,   1,   4,   1,   4,   1,    5,   1,    5,    1,   6,
      1,    6,   1,   6,   1,   7,   1,    7,   1,    8,    1,   8,
      1,    8,   1,   9,   1,   9,   1,    10,  1,    10,   1,   10,
      1,    11,  1,   11,  1,   11,  1,    12,  1,    12,   1,   13,
      1,    13,  1,   14,  1,   14,  1,    15,  1,    15,   1,   16,
      1,    16,  1,   17,  1,   17,  1,    18,  1,    18,   1,   19,
      1,    19,  1,   19,  1,   19,  1,    20,  1,    20,   1,   20,
      1,    20,  1,   21,  1,   21,  1,    21,  1,    21,   1,   22,
      1,    22,  1,   22,  1,   22,  1,    23,  1,    23,   1,   23,
      1,    23,  1,   24,  1,   24,  1,    24,  1,    24,   1,   24,
      1,    25,  1,   25,  1,   25,  1,    25,  1,    25,   1,   25,
      1,    26,  1,   26,  1,   26,  1,    26,  1,    26,   1,   27,
      1,    27,  1,   27,  1,   28,  1,    28,  1,    28,   1,   28,
      1,    28,  1,   28,  1,   29,  1,    29,  1,    29,   1,   29,
      1,    30,  1,   30,  1,   30,  1,    30,  1,    30,   1,   30,
      1,    31,  1,   31,  1,   31,  1,    31,  1,    31,   1,   32,
      1,    32,  1,   33,  1,   33,  1,    33,  1,    33,   1,   34,
      1,    34,  1,   34,  1,   35,  1,    35,  1,    36,   1,   36,
      5,    36,  209, 8,   36,  10,  36,   12,  36,   212,  9,   36,
      1,    37,  1,   37,  5,   37,  216,  8,   37,   10,   37,  12,
      37,   219, 9,   37,  1,   38,  1,    38,  1,    39,   1,   39,
      5,    39,  225, 8,   39,  10,  39,   12,  39,   228,  9,   39,
      1,    39,  1,   39,  1,   40,  1,    40,  3,    40,   234, 8,
      40,   1,   41,  4,   41,  237, 8,    41,  11,   41,   12,  41,
      238,  1,   41,  1,   41,  1,   42,   1,   42,   1,    42,  1,
      42,   1,   43,  1,   43,  1,   43,   1,   43,   1,    43,  1,
      43,   1,   43,  1,   43,  1,   43,   1,   43,   3,    43,  257,
      8,    43,  1,   44,  1,   44,  1,    45,  1,    45,   0,   0,
      46,   1,   1,   3,   2,   5,   3,    7,   4,    9,    5,   11,
      6,    13,  7,   15,  8,   17,  9,    19,  10,   21,   11,  23,
      12,   25,  13,  27,  14,  29,  15,   31,  16,   33,   17,  35,
      18,   37,  19,  39,  20,  41,  21,   43,  22,   45,   23,  47,
      24,   49,  25,  51,  26,  53,  27,   55,  28,   57,   29,  59,
      30,   61,  31,  63,  32,  65,  33,   67,  34,   69,   35,  71,
      36,   73,  0,   75,  37,  77,  38,   79,  39,   81,   0,   83,
      40,   85,  41,  87,  0,   89,  0,    91,  0,    1,    0,   7,
      4,    0,   65,  90,  95,  95,  97,   122, 1040, 1103, 5,   0,
      48,   57,  65,  90,  95,  95,  97,   122, 1040, 1103, 3,   0,
      10,   10,  13,  13,  92,  92,  2,    0,   9,    9,    32,  32,
      2,    0,   10,  10,  13,  13,  1,    0,   48,   57,   1,   0,
      49,   57,  265, 0,   1,   1,   0,    0,   0,    0,    3,   1,
      0,    0,   0,   0,   5,   1,   0,    0,   0,    0,    7,   1,
      0,    0,   0,   0,   9,   1,   0,    0,   0,    0,    11,  1,
      0,    0,   0,   0,   13,  1,   0,    0,   0,    0,    15,  1,
      0,    0,   0,   0,   17,  1,   0,    0,   0,    0,    19,  1,
      0,    0,   0,   0,   21,  1,   0,    0,   0,    0,    23,  1,
      0,    0,   0,   0,   25,  1,   0,    0,   0,    0,    27,  1,
      0,    0,   0,   0,   29,  1,   0,    0,   0,    0,    31,  1,
      0,    0,   0,   0,   33,  1,   0,    0,   0,    0,    35,  1,
      0,    0,   0,   0,   37,  1,   0,    0,   0,    0,    39,  1,
      0,    0,   0,   0,   41,  1,   0,    0,   0,    0,    43,  1,
      0,    0,   0,   0,   45,  1,   0,    0,   0,    0,    47,  1,
      0,    0,   0,   0,   49,  1,   0,    0,   0,    0,    51,  1,
      0,    0,   0,   0,   53,  1,   0,    0,   0,    0,    55,  1,
      0,    0,   0,   0,   57,  1,   0,    0,   0,    0,    59,  1,
      0,    0,   0,   0,   61,  1,   0,    0,   0,    0,    63,  1,
      0,    0,   0,   0,   65,  1,   0,    0,   0,    0,    67,  1,
      0,    0,   0,   0,   69,  1,   0,    0,   0,    0,    71,  1,
      0,    0,   0,   0,   75,  1,   0,    0,   0,    0,    77,  1,
      0,    0,   0,   0,   79,  1,   0,    0,   0,    0,    83,  1,
      0,    0,   0,   0,   85,  1,   0,    0,   0,    1,    93,  1,
      0,    0,   0,   3,   95,  1,   0,    0,   0,    5,    97,  1,
      0,    0,   0,   7,   99,  1,   0,    0,   0,    9,    101, 1,
      0,    0,   0,   11,  103, 1,   0,    0,   0,    13,   105, 1,
      0,    0,   0,   15,  108, 1,   0,    0,   0,    17,   110, 1,
      0,    0,   0,   19,  113, 1,   0,    0,   0,    21,   115, 1,
      0,    0,   0,   23,  118, 1,   0,    0,   0,    25,   121, 1,
      0,    0,   0,   27,  123, 1,   0,    0,   0,    29,   125, 1,
      0,    0,   0,   31,  127, 1,   0,    0,   0,    33,   129, 1,
      0,    0,   0,   35,  131, 1,   0,    0,   0,    37,   133, 1,
      0,    0,   0,   39,  135, 1,   0,    0,   0,    41,   139, 1,
      0,    0,   0,   43,  143, 1,   0,    0,   0,    45,   147, 1,
      0,    0,   0,   47,  151, 1,   0,    0,   0,    49,   155, 1,
      0,    0,   0,   51,  160, 1,   0,    0,   0,    53,   166, 1,
      0,    0,   0,   55,  171, 1,   0,    0,   0,    57,   174, 1,
      0,    0,   0,   59,  180, 1,   0,    0,   0,    61,   184, 1,
      0,    0,   0,   63,  190, 1,   0,    0,   0,    65,   195, 1,
      0,    0,   0,   67,  197, 1,   0,    0,   0,    69,   201, 1,
      0,    0,   0,   71,  204, 1,   0,    0,   0,    73,   206, 1,
      0,    0,   0,   75,  213, 1,   0,    0,   0,    77,   220, 1,
      0,    0,   0,   79,  222, 1,   0,    0,   0,    81,   233, 1,
      0,    0,   0,   83,  236, 1,   0,    0,   0,    85,   242, 1,
      0,    0,   0,   87,  256, 1,   0,    0,   0,    89,   258, 1,
      0,    0,   0,   91,  260, 1,   0,    0,   0,    93,   94,  5,
      43,   0,   0,   94,  2,   1,   0,    0,   0,    95,   96,  5,
      45,   0,   0,   96,  4,   1,   0,    0,   0,    97,   98,  5,
      42,   0,   0,   98,  6,   1,   0,    0,   0,    99,   100, 5,
      47,   0,   0,   100, 8,   1,   0,    0,   0,    101,  102, 5,
      37,   0,   0,   102, 10,  1,   0,    0,   0,    103,  104, 5,
      62,   0,   0,   104, 12,  1,   0,    0,   0,    105,  106, 5,
      62,   0,   0,   106, 107, 5,   61,   0,   0,    107,  14,  1,
      0,    0,   0,   108, 109, 5,   60,   0,   0,    109,  16,  1,
      0,    0,   0,   110, 111, 5,   60,   0,   0,    111,  112, 5,
      61,   0,   0,   112, 18,  1,   0,    0,   0,    113,  114, 5,
      61,   0,   0,   114, 20,  1,   0,    0,   0,    115,  116, 5,
      33,   0,   0,   116, 117, 5,   61,   0,   0,    117,  22,  1,
      0,    0,   0,   118, 119, 5,   58,   0,   0,    119,  120, 5,
      61,   0,   0,   120, 24,  1,   0,    0,   0,    121,  122, 5,
      40,   0,   0,   122, 26,  1,   0,    0,   0,    123,  124, 5,
      41,   0,   0,   124, 28,  1,   0,    0,   0,    125,  126, 5,
      123,  0,   0,   126, 30,  1,   0,    0,   0,    127,  128, 5,
      125,  0,   0,   128, 32,  1,   0,    0,   0,    129,  130, 5,
      91,   0,   0,   130, 34,  1,   0,    0,   0,    131,  132, 5,
      93,   0,   0,   132, 36,  1,   0,    0,   0,    133,  134, 5,
      44,   0,   0,   134, 38,  1,   0,    0,   0,    135,  136, 5,
      1094, 0,   0,   136, 137, 5,   1077, 0,   0,    137,  138, 5,
      1083, 0,   0,   138, 40,  1,   0,    0,   0,    139,  140, 5,
      1074, 0,   0,   140, 141, 5,   1077, 0,   0,    141,  142, 5,
      1097, 0,   0,   142, 42,  1,   0,    0,   0,    143,  144, 5,
      1072, 0,   0,   144, 145, 5,   1083, 0,   0,    145,  146, 5,
      1075, 0,   0,   146, 44,  1,   0,    0,   0,    147,  148, 5,
      1085, 0,   0,   148, 149, 5,   1072, 0,   0,    149,  150, 5,
      1095, 0,   0,   150, 46,  1,   0,    0,   0,    151,  152, 5,
      1082, 0,   0,   152, 153, 5,   1086, 0,   0,    153,  154, 5,
      1085, 0,   0,   154, 48,  1,   0,    0,   0,    155,  156, 5,
      1074, 0,   0,   156, 157, 5,   1074, 0,   0,    157,  158, 5,
      1086, 0,   0,   158, 159, 5,   1076, 0,   0,    159,  50,  1,
      0,    0,   0,   160, 161, 5,   1074, 0,   0,    161,  162, 5,
      1099, 0,   0,   162, 163, 5,   1074, 0,   0,    163,  164, 5,
      1086, 0,   0,   164, 165, 5,   1076, 0,   0,    165,  52,  1,
      0,    0,   0,   166, 167, 5,   1077, 0,   0,    167,  168, 5,
      1089, 0,   0,   168, 169, 5,   1083, 0,   0,    169,  170, 5,
      1080, 0,   0,   170, 54,  1,   0,    0,   0,    171,  172, 5,
      1090, 0,   0,   172, 173, 5,   1086, 0,   0,    173,  56,  1,
      0,    0,   0,   174, 175, 5,   1080, 0,   0,    175,  176, 5,
      1085, 0,   0,   176, 177, 5,   1072, 0,   0,    177,  178, 5,
      1095, 0,   0,   178, 179, 5,   1077, 0,   0,    179,  58,  1,
      0,    0,   0,   180, 181, 5,   1074, 0,   0,    181,  182, 5,
      1089, 0,   0,   182, 183, 5,   1105, 0,   0,    183,  60,  1,
      0,    0,   0,   184, 185, 5,   1074, 0,   0,    185,  186, 5,
      1099, 0,   0,   186, 187, 5,   1093, 0,   0,    187,  188, 5,
      1086, 0,   0,   188, 189, 5,   1076, 0,   0,    189,  62,  1,
      0,    0,   0,   190, 191, 5,   1079, 0,   0,    191,  192, 5,
      1085, 0,   0,   192, 193, 5,   1072, 0,   0,    193,  194, 5,
      1095, 0,   0,   194, 64,  1,   0,    0,   0,    195,  196, 5,
      1080, 0,   0,   196, 66,  1,   0,    0,   0,    197,  198, 5,
      1080, 0,   0,   198, 199, 5,   1083, 0,   0,    199,  200, 5,
      1080, 0,   0,   200, 68,  1,   0,    0,   0,    201,  202, 5,
      1085, 0,   0,   202, 203, 5,   1077, 0,   0,    203,  70,  1,
      0,    0,   0,   204, 205, 3,   73,   36,  0,    205,  72,  1,
      0,    0,   0,   206, 210, 7,   0,    0,   0,    207,  209, 7,
      1,    0,   0,   208, 207, 1,   0,    0,   0,    209,  212, 1,
      0,    0,   0,   210, 208, 1,   0,    0,   0,    210,  211, 1,
      0,    0,   0,   211, 74,  1,   0,    0,   0,    212,  210, 1,
      0,    0,   0,   213, 217, 3,   91,   45,  0,    214,  216, 3,
      89,   44,  0,   215, 214, 1,   0,    0,   0,    216,  219, 1,
      0,    0,   0,   217, 215, 1,   0,    0,   0,    217,  218, 1,
      0,    0,   0,   218, 76,  1,   0,    0,   0,    219,  217, 1,
      0,    0,   0,   220, 221, 5,   48,   0,   0,    221,  78,  1,
      0,    0,   0,   222, 226, 5,   124,  0,   0,    223,  225, 3,
      81,   40,  0,   224, 223, 1,   0,    0,   0,    225,  228, 1,
      0,    0,   0,   226, 224, 1,   0,    0,   0,    226,  227, 1,
      0,    0,   0,   227, 229, 1,   0,    0,   0,    228,  226, 1,
      0,    0,   0,   229, 230, 6,   39,   0,   0,    230,  80,  1,
      0,    0,   0,   231, 234, 8,   2,    0,   0,    232,  234, 3,
      87,   43,  0,   233, 231, 1,   0,    0,   0,    233,  232, 1,
      0,    0,   0,   234, 82,  1,   0,    0,   0,    235,  237, 7,
      3,    0,   0,   236, 235, 1,   0,    0,   0,    237,  238, 1,
      0,    0,   0,   238, 236, 1,   0,    0,   0,    238,  239, 1,
      0,    0,   0,   239, 240, 1,   0,    0,   0,    240,  241, 6,
      41,   0,   0,   241, 84,  1,   0,    0,   0,    242,  243, 7,
      4,    0,   0,   243, 244, 1,   0,    0,   0,    244,  245, 6,
      42,   1,   0,   245, 86,  1,   0,    0,   0,    246,  247, 5,
      92,   0,   0,   247, 257, 5,   39,   0,   0,    248,  249, 5,
      92,   0,   0,   249, 257, 5,   34,   0,   0,    250,  251, 5,
      92,   0,   0,   251, 257, 5,   92,   0,   0,    252,  253, 5,
      92,   0,   0,   253, 257, 5,   110,  0,   0,    254,  255, 5,
      92,   0,   0,   255, 257, 5,   114,  0,   0,    256,  246, 1,
      0,    0,   0,   256, 248, 1,   0,    0,   0,    256,  250, 1,
      0,    0,   0,   256, 252, 1,   0,    0,   0,    256,  254, 1,
      0,    0,   0,   257, 88,  1,   0,    0,   0,    258,  259, 7,
      5,    0,   0,   259, 90,  1,   0,    0,   0,    260,  261, 7,
      6,    0,   0,   261, 92,  1,   0,    0,   0,    7,    0,   210,
      217,  226, 233, 238, 256, 2,   6,    0,   0,    0,    1,   0};
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
  rallexerLexerStaticData = staticData.release();
}

} // namespace

RalLexer::RalLexer(CharStream *input) : Lexer(input) {
  RalLexer::initialize();
  _interpreter =
      new atn::LexerATNSimulator(this, *rallexerLexerStaticData->atn,
                                 rallexerLexerStaticData->decisionToDFA,
                                 rallexerLexerStaticData->sharedContextCache);
}

RalLexer::~RalLexer() { delete _interpreter; }

std::string RalLexer::getGrammarFileName() const { return "RalLexer.g4"; }

const std::vector<std::string> &RalLexer::getRuleNames() const {
  return rallexerLexerStaticData->ruleNames;
}

const std::vector<std::string> &RalLexer::getChannelNames() const {
  return rallexerLexerStaticData->channelNames;
}

const std::vector<std::string> &RalLexer::getModeNames() const {
  return rallexerLexerStaticData->modeNames;
}

const dfa::Vocabulary &RalLexer::getVocabulary() const {
  return rallexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView RalLexer::getSerializedATN() const {
  return rallexerLexerStaticData->serializedATN;
}

const atn::ATN &RalLexer::getATN() const {
  return *rallexerLexerStaticData->atn;
}

void RalLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  rallexerLexerInitialize();
#else
  ::antlr4::internal::call_once(rallexerLexerOnceFlag, rallexerLexerInitialize);
#endif
}
