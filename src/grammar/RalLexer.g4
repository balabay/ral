lexer grammar RalLexer;

// Operators
Add: '+';
Sub: '-';
Mul: '*';
Div: '/';
Mod: '%';
Pow: '**';

Gt: '>';
Gte: '>=';
Lt: '<';
Lte: '<=';
Eq: '=';
Ne: '!=';

Equal: ':=';

// Pairs
OpeningParen: '(';
ClosingParen: ')';
OpeningBrace: '{';
ClosingBrace: '}';
OpeningBracket: '[';
ClosingBracket: ']';

Comma: ',';

FormatSeparator: ':';
StatementSeparator: ';';

// Types
IntegerTypeName: 'цел';
FloatingPointTypeName: 'вещ';

AlgorhitmHeader: 'алг';
BeginAlgorhitmImplementation: 'нач';
EndAlgorhitmImplementation: 'кон';

// Print
TerminalInput: 'ввод';
TerminalOutput: 'вывод';

// Statements
If: 'если';
Then: 'то';
Else: 'иначе';
EndOfIfOrSwitchStatement: 'вс' YeYo;
fragment YeYo: [её];
LoopBreakAndAlgorhitmReturn: 'выход';
FunctionReturnValue: 'знач';

LogicalAnd: 'и';
LogicalOr: 'или';
LogicalNot: 'не';

NewLine: 'нс';

LoopBegin: 'нц';
LoopCount: 'раз';
LoopEnd: 'кц';

Id: Name;
fragment Name: [a-zA-Z_а-яА-Я] [a-zA-Zа-яА-Я0-9_]*;

// Literals
StringLiteral: STRING_LITERAL_SINGLE_QUOTE | STRING_LITERAL_DOUBLE_QUOTE;
ZeroLiteral: '0';
DecimalLiteral: NONZERODIGIT DIGIT*;
RealLiteral: ('0' .. '9')+ (('.' ('0' .. '9')+ (EXPONENT)?)? | EXPONENT);

fragment EXPONENT: ('E'|'e') ('+' | '-')? ('0' .. '9')+;

// Comments
Comment: '|' CommentLine* -> skip;
fragment CommentLine: ~ [\\\r\n] | Escape;

// Specials
WhiteSpace: [ \t]+ -> skip;
LineTerminator: [\r\n] -> channel(HIDDEN);

fragment Escape: '\\\'' | '\\"' | '\\\\' | '\\n' | '\\r';

fragment DIGIT: [0-9];
fragment NONZERODIGIT: [1-9];

fragment STRING_LITERAL_SINGLE_QUOTE
    : '\'' ('\'\'' | ~ ('\''))* '\''
    ;

fragment STRING_LITERAL_DOUBLE_QUOTE
    : '"' ('""' | ~ ('"'))* '"'
    ;
