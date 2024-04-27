parser grammar RalParser;

options {
	tokenVocab = RalLexer;
}

module:
        algorithm* EOF;

algorithm:
        AlgorhitmHeader algorithmPrototype algorithmBody;

algorithmPrototype: type? Id ('(' formalParameters? ')')?;

algorithmBody: BeginAlgorhitmImplementation instructions EndAlgorhitmImplementation;

formalParameters
    :   type Id (',' type Id)*
    ;

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
        | <assoc = right> Id Equal expression # VariableAffectation
        | <assoc = right> FunctionReturnValue Equal expression # FunctionAffectation
        | literal # LiteralExpression;

functionCall: Id '(' args? ')';

args
    :   expression (',' expression)*
    ;

literal: integerLiteral;

integerLiteral:
	(
		DecimalLiteral
		| ZeroLiteral
	);

type:
	IntegerTypeName 
	| FloatingPointTypeName;

variableDeclaration:
        type Id Equal expression (',' Id Equal expression)*;

printStatement: TerminalOutput  expression (',' expression)*;

inputStatement: TerminalInput Id (',' Id)*;

ifStatement: If expression Then instructions EndOfIfOrSwitchStatement;

returnStatement: LoopBreakAndAlgorhitmReturn;
