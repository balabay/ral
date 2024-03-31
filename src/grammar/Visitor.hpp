#pragma once

#include "runtime/RalLexer.h"
#include "runtime/RalParserBaseVisitor.h"

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <logic/Scope.hpp>
#include <vector>

namespace RaLang {

struct DebugInfo {
  llvm::DICompileUnit *unit;
  llvm::DIType *itype = nullptr;
};

class Visitor {
public:
  std::unique_ptr<llvm::LLVMContext> llvm_context;
  llvm::IRBuilder<> builder;
  std::unique_ptr<llvm::Module> module;
  std::vector<Scope> scopes;

  Visitor();

  Scope &currentScope();

  ValueInst getVariable(const std::string &name);

  void fromFile(const std::string &path);

  llvm::Function *printfPrototype();
  llvm::Function *inputPrototype();

  llvm::DIType *getDebugType();
  llvm::DISubroutineType *createFunctionType(unsigned NumArgs);

  void visitModule(RalParser::ModuleContext *context);
  void visitFunction(RalParser::FunctionContext *functionContext);

  void visitInstructions(RalParser::InstructionsContext *context);

  struct Body {
    llvm::BasicBlock *mainBlock = nullptr;
    llvm::BasicBlock *afterBlock = nullptr;
  };

  Body visitBody(RalParser::BodyContext *context,
                 llvm::BasicBlock *afterBlock = nullptr);

  llvm::Value *visitStatements(
      const std::vector<RalParser::StatementContext *> &statementContexts);

  llvm::Value *visitStatement(RalParser::StatementContext *context);

  void visitVariableDeclaration(RalParser::VariableDeclarationContext *context);

  void visitIfStatement(RalParser::IfStatementContext *context);

  void visitWhileStatement(RalParser::WhileStatementContext *context);

  void visitPrintStatement(RalParser::PrintStatementContext *context);
  void visitInputStatement(RalParser::InputStatementContext *context);

  llvm::Value *visitReturnStatement(RalParser::ReturnStatementContext *context);

  llvm::Value *visitExpression(RalParser::ExpressionContext *context);

  llvm::Value *visitUnaryNegativeExpression(
      RalParser::UnaryNegativeExpressionContext *context);

  llvm::Value *visitNameExpression(RalParser::NameExpressionContext *context);

  llvm::Value *visitFunctionCallExpression(
      RalParser::FunctionCallExpressionContext *context);

  llvm::Value *visitBinaryOperation(RalParser::BinaryOperationContext *context);

  llvm::Value *visitBinaryMultiplyOperation(
      RalParser::BinaryMultiplyOperationContext *context);

  llvm::Value *visitBinaryConditionalOperation(
      RalParser::BinaryConditionalOperationContext *context);

  llvm::Value *
  visitVariableAffectation(RalParser::VariableAffectationContext *context);

  llvm::Value *visitLiteral(RalParser::LiteralContext *context);

  llvm::Value *visitIntegerLiteral(RalParser::IntegerLiteralContext *context);

  llvm::Type *visitType(RalParser::TypeContext *context);

private:
  std::vector<llvm::DIScope *> LexicalBlocks;
  void emitLocation(antlr4::ParserRuleContext *node, llvm::DICompileUnit *unit);
  std::unique_ptr<llvm::DIBuilder> debugBuilder;
  struct DebugInfo debugInfo;
};
} // namespace RaLang
