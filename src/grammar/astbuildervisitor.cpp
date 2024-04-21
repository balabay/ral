#include "astbuildervisitor.h"

#include "grammar/ast.h"
#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"
#include "ralexceptions.h"

namespace RaLang {

AstBuilderVisitor::AstBuilderVisitor(const std::string &fileName,
                                     SymbolTable &symbolTable, Ast &ast)
    : m_symbolTable(symbolTable), m_ast(ast), m_fileName(fileName) {}

std::any AstBuilderVisitor::visitModule(RalParser::ModuleContext *ctx) {
  auto module = AstModule::create(m_fileName, ctx->getStart()->getLine());
  m_ast.add(module);

  for (auto algorithm : ctx->algorithm()) {
    std::any childResult = algorithm->accept(this);
    auto algorithmResult =
        std::any_cast<std::shared_ptr<AstAlgorithm>>(childResult);
    module->addNode(algorithmResult);
  }
  return module;
}

std::any AstBuilderVisitor::visitReturnStatement(
    RalParser::ReturnStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  return AstReturnStatement::create(line);
}

std::any AstBuilderVisitor::visitAlgorithm(RalParser::AlgorithmContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = ctx->algorithmPrototype()->Id()->getSymbol()->getText();
  auto algorithm = AstAlgorithm::create(name, line);

  // Update scope to this algorithm
  Scope *scope = m_symbolTable.getCurrentScope();
  Symbol *symbol = scope->resolve(name);
  auto algorithmSymbol = dynamic_cast<MethodSymbol *>(symbol);
  assert(algorithmSymbol);

  // Get body
  m_symbolTable.pushScope(algorithmSymbol);
  std::any childResult = ctx->algorithmBody()->accept(this);
  if (childResult.has_value()) {
    auto statements =
        std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
    for (auto statement : statements) {
      algorithm->addNode(statement);
    }
  }
  m_symbolTable.popScope();

  return algorithm;
}

std::any
AstBuilderVisitor::visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) {
  return ctx->instructions()->accept(this);
}

std::any
AstBuilderVisitor::visitInstructions(RalParser::InstructionsContext *ctx) {
  std::vector<std::shared_ptr<AstStatement>> statements;
  auto statementContexts = ctx->statement();
  for (auto *statementContext : statementContexts) {
    std::any childResult = statementContext->accept(this);
    if (childResult.has_value()) {
      auto statement =
          std::any_cast<std::shared_ptr<AstStatement>>(childResult);
      statements.push_back(statement);
    }
  }
  return statements;
}

std::any
AstBuilderVisitor::visitIntegerLiteral(RalParser::IntegerLiteralContext *ctx) {
  std::string text =
      ctx->ZeroLiteral() ? "0" : ctx->DecimalLiteral()->getSymbol()->getText();
  auto result = AstIntExpression::create(text, ctx->getStart()->getLine());
  return std::dynamic_pointer_cast<AstExpression>(result);
}

std::any AstBuilderVisitor::visitStatement(RalParser::StatementContext *ctx) {
  if (auto *returnStatementContext = ctx->returnStatement()) {
    return returnStatementContext->accept(this);
  } else if (auto *expression = ctx->expression()) {
    int line = ctx->getStart()->getLine();
    auto expressionStatement = AstExpressionStatement::create(line);
    std::any childResult = expression->accept(this);
    if (childResult.has_value()) {
      auto expression =
          std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      expressionStatement->addNode(expression);
    } else {
      // TODO: process error
    }
    return std::dynamic_pointer_cast<AstStatement>(expressionStatement);
  }
  return {};
}

std::any
AstBuilderVisitor::visitFunctionCall(RalParser::FunctionCallContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto name = ctx->Id()->getText();
  auto *calleeSymbol = dynamic_cast<MethodSymbol *>(
      m_symbolTable.getCurrentScope()->resolve(name));

  if (!calleeSymbol) {
    throw VariableNotFoundException(name + ", line: " + std::to_string(line));
  }

  std::vector<RalParser::ExpressionContext *> callArgs;
  if (ctx->args()) {
    callArgs = ctx->args()->expression();
  }

  std::vector<Symbol *> formalParameters = calleeSymbol->getFormalParameters();
  if (formalParameters.size() != callArgs.size()) {
    throw VariableNotFoundException(
        "Incorrect # arguments passed to " + name + ". Expected " +
        std::to_string(formalParameters.size()) + ", actual " +
        std::to_string(callArgs.size()) + ", line: " + std::to_string(line));
  }

  auto functionCallExpression = AstAlgorithmCallExpression::create(name, line);
  for (unsigned i = 0; i != formalParameters.size(); ++i) {
    std::any childResult = callArgs[i]->accept(this);
    if (childResult.has_value()) {
      auto expression =
          std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      functionCallExpression->addNode(expression);
    } else {
      throw VariableNotFoundException("Incorrect argument " +
                                      std::to_string(i) +
                                      ", line: " + std::to_string(line));
    }
  }
  return std::dynamic_pointer_cast<AstExpression>(functionCallExpression);
}

} // namespace RaLang
