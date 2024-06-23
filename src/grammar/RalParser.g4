parser grammar RalParser;

options {
	tokenVocab = RalLexer;
}

module:
        algorithm* EOF;

algorithm:
        AlgorhitmHeader algorithmPrototype algorithmBody;

algorithmPrototype: type? algorithmName ('(' formalParameters? ')')?;

algorithmName: Id (Id)*;

algorithmBody: BeginAlgorhitmImplementation instructions EndAlgorhitmImplementation;

formalParameters : formalParameter (',' formalParameter)*;

formalParameter
    :   (parameterModifier)* (type)? Id;

parameterModifier: WriteOnlyParameterModifier | ReadOnlyParameterModifier | ReadWriteParameterModifier;
instructions: statement*;

statement:
        variableDeclaration (StatementSeparator)*
        | ifStatement (StatementSeparator)*
        | switchStatement (StatementSeparator)*
        | printStatement (StatementSeparator)*
        | inputStatement (StatementSeparator)*
        | expressionStatement (StatementSeparator)*
        | loopKStatement (StatementSeparator)*
        | loopWhileStatement (StatementSeparator)*
        | loopUntilStatement (StatementSeparator)*
        | loopForStatement (StatementSeparator)*
        | returnStatement (StatementSeparator)*
        ;

expressionStatement: expression;

expression:
	'(' expression ')' # InParenExpression
	| '-' expression # UnaryNegativeExpression
        | functionCall # FunctionCallExpression
        | Id # NameExpression
        | expression (Pow) expression # PowerOperation
        | expression (Mul | Div | Mod) expression # BinaryMultiplyOperation
	| expression (Add | Sub) expression # BinaryOperation
	| expression (Gt | Gte | Lt | Lte | Eq | Ne) expression	# BinaryConditionalOperation
        | LogicalNot expression	# LogicalNot
        | expression LogicalAnd expression	# LogicalAnd
        | expression LogicalOr expression	# LogicalOr
        | <assoc = right> Id Equal expression # VariableAffectation
        | <assoc = right> FunctionReturnValue Equal expression # FunctionAffectation
        | literal # LiteralExpression;

functionCall: algorithmName '(' args? ')';

args
    :   expression (',' expression)*
    ;

literal
    : integerLiteral
    | realLiteral
    | stringLiteral;

integerLiteral:
	(
		DecimalLiteral
		| ZeroLiteral
	);

realLiteral: RealLiteral;

stringLiteral: StringLiteral | NewLine;

type:
	IntegerTypeName 
        | FloatingPointTypeName
        | BooleanTypeName;

variableDeclaration:
        type singleVariableDeclaration (',' singleVariableDeclaration)*;

singleVariableDeclaration: Id (Eq expression)?;

printStatement: TerminalOutput  formatExpression (',' formatExpression)*;

formatExpression: expression (formatSpecifier)?;

// Width and Precision
formatSpecifier: FormatSeparator expression (FormatSeparator expression)?;

inputStatement: TerminalInput Id (',' Id)*;

ifStatement: If expression Then thenInstructions (Else elseInstructions)? EndOfIfOrSwitchStatement;

switchStatement: Switch (case)* (Else (statement)+)? EndOfIfOrSwitchStatement;

case: Case expression ':' (statement)+;

thenInstructions: instructions;
elseInstructions: instructions;

loopKStatement: LoopBegin expression LoopCount instructions LoopEnd;
loopWhileStatement: LoopBegin LoopWhile expression instructions LoopEnd;
loopUntilStatement: LoopBegin instructions LoopEnd Case expression;
loopForStatement: LoopBegin For Id From startExpression To endExpression (Step stepExpression)? instructions LoopEnd;
startExpression: expression;
endExpression: expression;
stepExpression: expression;

returnStatement: LoopBreakAndAlgorhitmReturn;
