#pragma once

#include "logic/symboltable.h"
#include "runtime/RalLexer.h"
#include "runtime/RalParserBaseVisitor.h"

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
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
  SymbolTable m_symbolTable;
  bool m_emitDebugInfo;
  std::string m_path;

  Visitor(bool emitDebugInfo, const std::string &path);

  llvm::Function *printfPrototype();
  llvm::Function *inputPrototype();

  llvm::DIType *getDebugType();
  llvm::DISubroutineType *createFunctionType(unsigned NumArgs);

  void visitModule(RalParser::ModuleContext *context);
  void visitFunction(RalParser::FunctionContext *functionContext);

  void visitInstructions(RalParser::InstructionsContext *context, Scope *scope);

  struct Body {
    llvm::BasicBlock *mainBlock = nullptr;
    llvm::BasicBlock *afterBlock = nullptr;
  };

  Body visitBody(RalParser::BodyContext *context, Scope *scope,
                 llvm::BasicBlock *afterBlock = nullptr);

  llvm::Value *visitStatements(
      const std::vector<RalParser::StatementContext *> &statementContexts,
      Scope *scope);

  llvm::Value *visitStatement(RalParser::StatementContext *context,
                              Scope *scope);

  void visitVariableDeclaration(RalParser::VariableDeclarationContext *context,
                                Scope *scope);

  void visitIfStatement(RalParser::IfStatementContext *context, Scope *scope);

  void visitWhileStatement(RalParser::WhileStatementContext *context,
                           Scope *scope);

  void visitPrintStatement(RalParser::PrintStatementContext *context,
                           Scope *scope);
  void visitInputStatement(RalParser::InputStatementContext *context,
                           Scope *scope);

  llvm::Value *visitReturnStatement(RalParser::ReturnStatementContext *context,
                                    Scope *scope);

  llvm::Value *visitExpression(RalParser::ExpressionContext *context,
                               Scope *scope);

  llvm::Value *visitUnaryNegativeExpression(
      RalParser::UnaryNegativeExpressionContext *context, Scope *scope);

  llvm::Value *visitNameExpression(RalParser::NameExpressionContext *context,
                                   Scope *scope);

  llvm::Value *
  visitFunctionCallExpression(RalParser::FunctionCallExpressionContext *context,
                              Scope *scope);

  llvm::Value *visitBinaryOperation(RalParser::BinaryOperationContext *context,
                                    Scope *scope);

  llvm::Value *visitBinaryMultiplyOperation(
      RalParser::BinaryMultiplyOperationContext *context, Scope *scope);

  llvm::Value *visitBinaryConditionalOperation(
      RalParser::BinaryConditionalOperationContext *context, Scope *scope);

  llvm::Value *
  visitVariableAffectation(RalParser::VariableAffectationContext *context,
                           Scope *scope);

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
