lexer grammar RalLexer;

// Operators
Add: '+';
Sub: '-';
Mul: '*';
Div: '/';
Mod: '%';

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

Id: Name;
fragment Name: [a-zA-Z_а-яА-Я] [a-zA-Zа-яА-Я0-9_]*;

// Literals
DecimalLiteral: NONZERODIGIT DIGIT*;

ZeroLiteral: '0';

// Comments
Comment: '|' CommentLine* -> skip;
fragment CommentLine: ~ [\\\r\n] | Escape;

// Specials
WhiteSpace: [ \t]+ -> skip;
LineTerminator: [\r\n] -> channel(HIDDEN);

fragment Escape: '\\\'' | '\\"' | '\\\\' | '\\n' | '\\r';

fragment DIGIT: [0-9];
fragment NONZERODIGIT: [1-9];
