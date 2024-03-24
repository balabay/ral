parser grammar RalParser;

options {
	tokenVocab = RalLexer;
}

module:
        function* EOF;

function:
        FunctionDeclarator VariableName '(' formalParameters? ')' Begin instructions End;

formalParameters
    :   VariableName (',' VariableName)*
    ;

instructions: statement*;

body: '{' statement* '}';

statement:
	variableDeclaration InstructionsSeparator
	| body
	| ifStatement
	| whileStatement
	| printStatement InstructionsSeparator
	| expression InstructionsSeparator;

expression:
	'(' expression ')'										# InParenExpression
	| '-' expression										# UnaryNegativeExpression
        | functionCall # FunctionCallExpression
        | VariableName											# NameExpression
        | expression (Mul | Div | Mod) expression				# BinaryMultiplyOperation
	| expression (Add | Sub) expression						# BinaryOperation
	| expression (Gt | Gte | Lt | Lte | Eq | Ne) expression	# BinaryConditionalOperation
        | <assoc = right> VariableName Equal expression			# VariableAffectation
        | literal												# LiteralExpression;

functionCall: VariableName '(' args? ')';

args
    :   expression (',' expression)*
    ;

literal: integerLiteral;

integerLiteral:
	(
		DecimalLiteral
		| ZeroLiteral
		| HexadecimalLiteral
		| BinaryLiteral
	);

variableDeclaration:
        VariableDeclarator VariableName Equal expression;

printStatement: Print '(' expression (',' expression)* ')';

type: VariableName;

ifStatement: If expression body;

whileStatement: While expression body;

eos: (EOF | LineTerminator);
