#include "astbuildervisitor.h"

#include "grammar/ast.h"
#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"
#include "ralexceptions.h"

namespace RaLang {

AstBuilderVisitor::AstBuilderVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast)
    : m_symbolTable(symbolTable), m_ast(ast), m_fileName(fileName) {}

std::any AstBuilderVisitor::visitModule(RalParser::ModuleContext *ctx) {
  auto module = AstModule::create(m_fileName, ctx->getStart()->getLine());
  m_ast.add(module);

  for (auto algorithm : ctx->algorithm()) {
    std::any childResult = algorithm->accept(this);
    auto algorithmResult = std::any_cast<std::shared_ptr<AstAlgorithm>>(childResult);
    module->addNode(algorithmResult);
  }
  return module;
}

std::any AstBuilderVisitor::visitNameExpression(RalParser::NameExpressionContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = ctx->Id()->getSymbol()->getText();
  return getVariableByName(name, line);
}

std::any AstBuilderVisitor::visitInParenExpression(RalParser::InParenExpressionContext *ctx) {
  return ctx->expression()->accept(this);
}

std::any AstBuilderVisitor::visitPrintStatement(RalParser::PrintStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto printStatement = AstPrintStatement::create(line);
  auto expressions = ctx->expression();
  for (unsigned i = 0; i < expressions.size(); i++) {
    auto expr = expressions[i];
    std::any childResult = expr->accept(this);
    auto exprResult = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
    if (!exprResult) {
      throw VariableNotFoundException("Incorrect expression in print argument " + std::to_string(i) +
                                      ", line: " + std::to_string(line));
    }
    printStatement->addNode(exprResult);
  }
  return std::dynamic_pointer_cast<AstStatement>(printStatement);
}

std::any AstBuilderVisitor::visitInputStatement(RalParser::InputStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto inputStatement = AstInputStatement::create(line);
  auto variables = ctx->Id();
  for (unsigned i = 0; i < variables.size(); i++) {
    auto variable = variables[i];
    std::string name = variable->getSymbol()->getText();
    std::shared_ptr<AstExpression> variableResult = getVariableByName(name, line);
    inputStatement->addNode(variableResult);
  }
  return std::dynamic_pointer_cast<AstStatement>(inputStatement);
}

std::any AstBuilderVisitor::visitReturnStatement(RalParser::ReturnStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  return std::dynamic_pointer_cast<AstStatement>(AstReturnStatement::create(line));
}

std::any AstBuilderVisitor::visitAlgorithm(RalParser::AlgorithmContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = ctx->algorithmPrototype()->Id()->getSymbol()->getText();
  auto algorithm = AstAlgorithm::create(name, line);

  // Update scope to this algorithm
  Scope *scope = m_symbolTable.getCurrentScope();
  Symbol *symbol = scope->resolve(name);
  auto algorithmSymbol = dynamic_cast<AlgSymbol *>(symbol);
  assert(algorithmSymbol);

  // Get body
  m_symbolTable.pushScope(algorithmSymbol);
  std::any childResult = ctx->algorithmBody()->accept(this);
  if (childResult.has_value()) {
    auto statements = std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
    for (auto statement : statements) {
      algorithm->addNode(statement);
    }
  }
  m_symbolTable.popScope();

  return algorithm;
}

std::any AstBuilderVisitor::visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) {
  m_symbolTable.pushScope(m_symbolTable.createLocalScope(m_symbolTable.getCurrentScope()));
  auto result = ctx->instructions()->accept(this);
  m_symbolTable.popScope();
  return result;
}

std::any AstBuilderVisitor::visitBinaryConditionalOperation(RalParser::BinaryConditionalOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto node =
      ctx->Eq()
          ? ctx->Eq()
          : (ctx->Ne() ? ctx->Ne()
                       : (ctx->Gt() ? ctx->Gt()
                                    : (ctx->Gte() ? ctx->Gte()
                                                  : (ctx->Lt() ? ctx->Lt() : (ctx->Lte() ? ctx->Lte() : nullptr)))));
  std::string operation;
  if (node) {
    operation = node->getSymbol()->getText();
  } else {
    throw NotImplementedException();
  }
  auto astBinaryConditionalExpression = AstBinaryConditionalExpression::create(operation, line);
  auto expressions = ctx->expression();
  assert(expressions.size() == 2);
  for (unsigned i = 0; i < 2; i++) {
    auto expressionContext = expressions[i];
    std::any childResult = expressionContext->accept(this);
    if (childResult.has_value()) {
      auto astExpression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      astBinaryConditionalExpression->addNode(astExpression);
    } else {
      throw NotImplementedException();
    }
  }
  return std::dynamic_pointer_cast<AstExpression>(astBinaryConditionalExpression);
}

std::any AstBuilderVisitor::visitBinaryMultiplyOperation(RalParser::BinaryMultiplyOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto node = ctx->Mul() ? ctx->Mul() : (ctx->Div() ? ctx->Div() : (ctx->Mod() ? ctx->Mod() : nullptr));
  std::string operation;
  if (node) {
    operation = node->getSymbol()->getText();
  } else {
    throw NotImplementedException();
  }
  auto astMathExpression = AstMathExpression::create(operation, line);
  auto expressions = ctx->expression();
  assert(expressions.size() == 2);
  for (unsigned i = 0; i < 2; i++) {
    auto expressionContext = expressions[i];
    std::any childResult = expressionContext->accept(this);
    if (childResult.has_value()) {
      auto astExpression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      astMathExpression->addNode(astExpression);
    } else {
      throw NotImplementedException();
    }
  }
  return std::dynamic_pointer_cast<AstExpression>(astMathExpression);
}

std::any AstBuilderVisitor::visitBinaryOperation(RalParser::BinaryOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto node = ctx->Add() ? ctx->Add() : (ctx->Sub() ? ctx->Sub() : nullptr);
  std::string operation;
  if (node) {
    operation = node->getSymbol()->getText();
  } else {
    throw NotImplementedException();
  }

  auto astMathExpression = AstMathExpression::create(operation, line);
  auto expressions = ctx->expression();
  assert(expressions.size() == 2);
  for (unsigned i = 0; i < 2; i++) {
    auto expressionContext = expressions[i];
    std::any childResult = expressionContext->accept(this);
    if (childResult.has_value()) {
      auto astExpression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      astMathExpression->addNode(astExpression);
    } else {
      throw NotImplementedException();
    }
  }
  return std::dynamic_pointer_cast<AstExpression>(astMathExpression);
}

std::any AstBuilderVisitor::visitInstructions(RalParser::InstructionsContext *ctx) {
  std::vector<std::shared_ptr<AstStatement>> statements;
  auto statementContexts = ctx->statement();
  for (auto *statementContext : statementContexts) {
    std::any childResult = statementContext->accept(this);
    if (childResult.has_value()) {
      if (childResult.type() == typeid(std::shared_ptr<AstStatement>)) {
        auto statement = std::any_cast<std::shared_ptr<AstStatement>>(childResult);
        statements.push_back(statement);
      } else if (childResult.type() == typeid(std::vector<std::shared_ptr<AstStatement>>)) {
        auto currentStatements = std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
        std::copy(currentStatements.begin(), currentStatements.end(), std::back_inserter(statements));
      }
    }
  }
  return statements;
}

std::any AstBuilderVisitor::visitIntegerLiteral(RalParser::IntegerLiteralContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string text = ctx->ZeroLiteral() ? "0" : ctx->DecimalLiteral()->getSymbol()->getText();
  auto result = AstIntExpression::create(text, line);
  return std::dynamic_pointer_cast<AstExpression>(result);
}

std::any AstBuilderVisitor::visitStatement(RalParser::StatementContext *ctx) {
  if (auto *returnStatementContext = ctx->returnStatement()) {
    return returnStatementContext->accept(this);
  } else if (auto *variableDeclarationContext = ctx->variableDeclaration()) {
    return variableDeclarationContext->accept(this);
  } else if (auto *printStatementContext = ctx->printStatement()) {
    return printStatementContext->accept(this);
  } else if (auto *ifStatementContext = ctx->ifStatement()) {
    return ifStatementContext->accept(this);
  } else if (auto *inputStatementContext = ctx->inputStatement()) {
    return inputStatementContext->accept(this);
  } else if (auto *expression = ctx->expression()) {
    int line = ctx->getStart()->getLine();
    auto expressionStatement = AstExpressionStatement::create(line);
    std::any childResult = expression->accept(this);
    if (childResult.has_value()) {
      auto expression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      expressionStatement->addNode(expression);
    } else {
      // TODO: process error
    }
    return std::dynamic_pointer_cast<AstStatement>(expressionStatement);
  }
  throw NotImplementedException();
}

std::any AstBuilderVisitor::visitUnaryNegativeExpression(RalParser::UnaryNegativeExpressionContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string operation = ctx->Sub()->getSymbol()->getText();
  auto astUnaryExpression = AstUnaryExpression::create(operation, line);
  auto expressionContext = ctx->expression();
  std::any childResult = expressionContext->accept(this);
  if (childResult.has_value()) {
    auto astExpression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
    astUnaryExpression->addNode(astExpression);
  } else {
    throw NotImplementedException();
  }
  return std::dynamic_pointer_cast<AstExpression>(astUnaryExpression);
}

std::any AstBuilderVisitor::visitFunctionCall(RalParser::FunctionCallContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto name = ctx->Id()->getText();
  auto *calleeSymbol = dynamic_cast<AlgSymbol *>(m_symbolTable.getCurrentScope()->resolve(name));

  if (!calleeSymbol) {
    throw VariableNotFoundException(name + ", line: " + std::to_string(line));
  }

  std::vector<RalParser::ExpressionContext *> callArgs;
  if (ctx->args()) {
    callArgs = ctx->args()->expression();
  }

  std::vector<Symbol *> formalParameters = calleeSymbol->getFormalParameters();
  if (formalParameters.size() != callArgs.size()) {
    throw VariableNotFoundException("Incorrect # arguments passed to " + name + ". Expected " +
                                    std::to_string(formalParameters.size()) + ", actual " +
                                    std::to_string(callArgs.size()) + ", line: " + std::to_string(line));
  }

  auto functionCallExpression = AstAlgorithmCallExpression::create(name, line);
  for (unsigned i = 0; i != formalParameters.size(); ++i) {
    std::any childResult = callArgs[i]->accept(this);
    if (childResult.has_value()) {
      auto expression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      functionCallExpression->addNode(expression);
    } else {
      throw VariableNotFoundException("Incorrect argument " + std::to_string(i) + ", line: " + std::to_string(line));
    }
  }
  return std::dynamic_pointer_cast<AstExpression>(functionCallExpression);
}

std::any AstBuilderVisitor::visitIfStatement(RalParser::IfStatementContext *ctx) {
  int line = ctx->getStart()->getLine();

  auto ifConditionContext = ctx->expression();
  std::shared_ptr<AstExpression> astIfCondition;
  std::any childResult = ifConditionContext->accept(this);
  if (childResult.has_value()) {
    astIfCondition = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
  } else {
    throw VariableNotFoundException("No condition in 'if' statement, line: " + std::to_string(line));
  }

  auto thenBlockContext = ctx->thenInstructions()->instructions();
  std::vector<std::shared_ptr<AstStatement>> astThenBlock;
  childResult = thenBlockContext->accept(this);
  if (childResult.has_value()) {
    astThenBlock = std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
  } else {
    throw VariableNotFoundException("No 'then' block in 'if' statement, line: " + std::to_string(line));
  }

  // TODO
  std::vector<std::shared_ptr<AstStatement>> astElseBlock;
  if (ctx->elseInstructions()) {
    auto elseBlockContext = ctx->elseInstructions()->instructions();
    childResult = elseBlockContext->accept(this);
    if (childResult.has_value()) {
      astElseBlock = std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
    }
  }

  auto astIfStatement = AstIfStatement::create(line, astIfCondition, astThenBlock, astElseBlock);
  return std::dynamic_pointer_cast<AstStatement>(astIfStatement);
}

std::any AstBuilderVisitor::visitVariableDeclaration(RalParser::VariableDeclarationContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto ids = ctx->Id();
  auto exprs = ctx->expression();

  // TODO: support variable declaration without initialization
  assert(ids.size() == exprs.size());

  std::vector<std::shared_ptr<AstStatement>> statements;
  for (unsigned i = 0; i < ids.size(); i++) {
    std::string variableName = ids[i]->getSymbol()->getText();
    std::string typeName = ctx->type()->getText();
    auto *type = resolveType(m_symbolTable.getCurrentScope(), typeName);
    auto variableDeclarationStatement = AstVariableDeclarationStatement::create(variableName, typeName, line);
    if (exprs[i] != nullptr) {
      std::any childResult = exprs[i]->accept(this);
      if (childResult.has_value()) {
        auto expression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
        variableDeclarationStatement->addNode(expression);
      } else {
        throw VariableNotFoundException("Incorrect initialization expression at " + std::to_string(i) +
                                        ", line: " + std::to_string(line));
      }
    }
    Symbol *symbol = m_symbolTable.createVariableSymbol(variableName, type);
    m_symbolTable.getCurrentScope()->define(std::unique_ptr<Symbol>(symbol));
    statements.push_back(std::dynamic_pointer_cast<AstStatement>(variableDeclarationStatement));
  }
  return statements;
}

std::shared_ptr<AstExpression> AstBuilderVisitor::getVariableByName(const std::string &name, int line) {
  Symbol *symbol = m_symbolTable.getCurrentScope()->resolve(name);
  auto variableSymbol = dynamic_cast<VariableSymbol *>(symbol);
  if (variableSymbol == nullptr) {
    throw VariableNotFoundException("Incorrect variable name " + name + ", line: " + std::to_string(line));
  }
  return std::dynamic_pointer_cast<AstExpression>(AstVariableExpression::create(name, line));
}

std::any AstBuilderVisitor::visitVariableAffectation(RalParser::VariableAffectationContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = ctx->Id()->getSymbol()->getText();
  auto variableAffectation = AstVariableAffectationExpression::create(name, line);

  std::any childResult = ctx->expression()->accept(this);
  if (childResult.has_value()) {
    auto expression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
    variableAffectation->addNode(expression);

  } else {
    throw VariableNotFoundException("Incorrect initialization expression at " + name +
                                    ", line: " + std::to_string(line));
  }
  return std::dynamic_pointer_cast<AstExpression>(variableAffectation);
}

std::any AstBuilderVisitor::visitFunctionAffectation(RalParser::FunctionAffectationContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = ctx->FunctionReturnValue()->getSymbol()->getText();
  auto functionAffectation = AstFunctionAffectationExpression::create(name, line);

  std::any childResult = ctx->expression()->accept(this);
  if (childResult.has_value()) {
    auto expression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
    functionAffectation->addNode(expression);

  } else {
    throw VariableNotFoundException("Incorrect function value expression at " + name +
                                    ", line: " + std::to_string(line));
  }
  return std::dynamic_pointer_cast<AstExpression>(functionAffectation);
}

} // namespace RaLang
