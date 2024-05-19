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
    :   (type)? Id;

instructions: statement*;

statement:
        variableDeclaration
	| ifStatement
        | printStatement
        | inputStatement
        | expression
        | returnStatement
        ;

expression:
	'(' expression ')' # InParenExpression
	| '-' expression # UnaryNegativeExpression
        | functionCall # FunctionCallExpression
        | Id # NameExpression
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
	| FloatingPointTypeName;

variableDeclaration:
        type singleVariableDeclaration (',' singleVariableDeclaration)*;

singleVariableDeclaration: Id (Eq expression)?;

printStatement: TerminalOutput  expression (',' expression)*;

inputStatement: TerminalInput Id (',' Id)*;

ifStatement: If expression Then thenInstructions (Else elseInstructions)? EndOfIfOrSwitchStatement;

thenInstructions: instructions;
elseInstructions: instructions;

returnStatement: LoopBreakAndAlgorhitmReturn;
