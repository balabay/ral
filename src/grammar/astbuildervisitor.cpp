#include "astbuildervisitor.h"

#include "grammar/ast.h"
#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"
#include "utils.h"

namespace RaLang {

AstBuilderVisitor::AstBuilderVisitor(const std::string &fileName, SymbolTable &symbolTable, Ast &ast)
    : m_symbolTable(symbolTable), m_ast(ast), m_fileName(fileName) {}

std::any AstBuilderVisitor::visitModule(RalParser::ModuleContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto module = AstModule::create(line, m_fileName, m_symbolTable.getCurrentScope());
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
  return createVariableExpression(line, name);
}

std::any AstBuilderVisitor::visitInParenExpression(RalParser::InParenExpressionContext *ctx) {
  return ctx->expression()->accept(this);
}

std::any AstBuilderVisitor::visitPowerOperation(RalParser::PowerOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = "pow";
  std::vector<RalParser::ExpressionContext *> expressions = ctx->expression();
  assert(expressions.size() == 2);
  return createCallExpression(name, expressions, line);
}

std::any AstBuilderVisitor::visitPrintStatement(RalParser::PrintStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto printStatement = AstPrintStatement::create(line, m_symbolTable.getCurrentScope());
  std::vector<RalParser::FormatExpressionContext *> formatExpressions = ctx->formatExpression();
  for (unsigned i = 0; i < formatExpressions.size(); i++) {
    RalParser::FormatExpressionContext *formatExprCtx = formatExpressions[i];
    RalParser::ExpressionContext *expr = formatExprCtx->expression();
    std::any childResult = expr->accept(this);
    auto exprResult = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
    if (!exprResult) {
      throw VariableNotFoundException("Incorrect expression in print argument " + std::to_string(i) +
                                      ", line: " + std::to_string(line));
    }
    std::shared_ptr<AstExpression> widthExprResult;
    std::shared_ptr<AstExpression> precisionExprResult;
    RalParser::FormatSpecifierContext *formatSpecifier = formatExprCtx->formatSpecifier();
    if (formatSpecifier) {
      std::vector<RalParser::ExpressionContext *> formatExprs = formatSpecifier->expression();
      assert(formatExprs.size() > 0);
      assert(formatExprs.size() <= 2);

      RalParser::ExpressionContext *widthExpr = formatExprs[0];
      childResult = widthExpr->accept(this);
      widthExprResult = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      if (!widthExprResult) {
        throw VariableNotFoundException("Incorrect format width in print argument " + std::to_string(i) +
                                        ", line: " + std::to_string(line));
      }

      if (formatExprs.size() == 2) {
        RalParser::ExpressionContext *precisionExpr = formatExprs[1];
        if (precisionExpr) {
          childResult = precisionExpr->accept(this);
          precisionExprResult = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
          if (!precisionExprResult) {
            throw VariableNotFoundException("Incorrect format precision expression in print argument " +
                                            std::to_string(i) + ", line: " + std::to_string(line));
          }
        }
      }
    }
    printStatement->addFormatExpression(exprResult, widthExprResult, precisionExprResult);
  }
  return std::dynamic_pointer_cast<AstStatement>(printStatement);
}

std::any AstBuilderVisitor::visitRealLiteral(RalParser::RealLiteralContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string text = ctx->RealLiteral()->getSymbol()->getText();
  auto result = AstNumberLiteralExpression::create(line, TypeKind::Real, text, m_symbolTable.getCurrentScope());
  return std::dynamic_pointer_cast<AstExpression>(result);
}

std::any AstBuilderVisitor::visitInputStatement(RalParser::InputStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  auto inputStatement = AstInputStatement::create(line, m_symbolTable.getCurrentScope());
  auto variables = ctx->Id();
  for (unsigned i = 0; i < variables.size(); i++) {
    auto variable = variables[i];
    std::string name = variable->getSymbol()->getText();
    std::shared_ptr<AstExpression> variableResult = createVariableExpression(line, name);
    inputStatement->addNode(variableResult);
  }
  return std::dynamic_pointer_cast<AstStatement>(inputStatement);
}

std::any AstBuilderVisitor::visitReturnStatement(RalParser::ReturnStatementContext *ctx) {
  int line = ctx->getStart()->getLine();
  return std::dynamic_pointer_cast<AstStatement>(AstReturnStatement::create(line, m_symbolTable.getCurrentScope()));
}

std::any AstBuilderVisitor::visitAlgorithm(RalParser::AlgorithmContext *ctx) {
  int line = ctx->getStart()->getLine();

  // Update scope to this algorithm
  std::string name = getAlgorithmName(ctx->algorithmPrototype()->algorithmName());
  Scope *scope = m_symbolTable.getCurrentScope();
  AlgSymbol *algorithmSymbol = resolveAlgorithm(scope, name, line);
  m_symbolTable.pushScope(algorithmSymbol);
  Scope *localScope = m_symbolTable.createLocalScope(algorithmSymbol);
  m_symbolTable.pushScope(localScope);

  auto algorithm = AstAlgorithm::create(line, name, algorithmSymbol, localScope);

  // Get body
  std::any childResult = ctx->algorithmBody()->accept(this);
  if (childResult.has_value()) {
    auto statements = std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
    for (auto statement : statements) {
      algorithm->addNode(statement);
    }
  }
  m_symbolTable.popScope(); // Local Algorithm scope
  m_symbolTable.popScope(); // Algorithm parameters scope

  return algorithm;
}

std::any AstBuilderVisitor::visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) {
  return ctx->instructions()->accept(this);
}

std::any AstBuilderVisitor::visitBinaryConditionalOperation(RalParser::BinaryConditionalOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  AstTokenType type =
      ctx->Eq()
          ? AstTokenType::COND_EQ
          : (ctx->Ne()
                 ? AstTokenType::COND_NE
                 : (ctx->Gt()
                        ? AstTokenType::COND_GT
                        : (ctx->Gte() ? AstTokenType::COND_GE
                                      : (ctx->Lt() ? AstTokenType::COND_LT
                                                   : (ctx->Lte() ? AstTokenType::COND_LE : AstTokenType::_COUNT)))));
  assert(type != AstTokenType::_COUNT);

  auto astBinaryConditionalExpression =
      AstBinaryConditionalExpression::create(line, type, m_symbolTable.getCurrentScope());
  auto result = std::dynamic_pointer_cast<AstExpression>(astBinaryConditionalExpression);
  addBinarySubExpressions(result, ctx->expression(), line);
  return result;
}

std::any AstBuilderVisitor::visitBinaryMultiplyOperation(RalParser::BinaryMultiplyOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  AstTokenType type = ctx->Mul()
                          ? AstTokenType::MUL
                          : (ctx->Div() ? AstTokenType::DIV : (ctx->Mod() ? AstTokenType::MOD : AstTokenType::_COUNT));
  assert(type != AstTokenType::_COUNT);
  auto astMathExpression = AstMathExpression::create(line, type, m_symbolTable.getCurrentScope());
  auto result = std::dynamic_pointer_cast<AstExpression>(astMathExpression);
  addBinarySubExpressions(result, ctx->expression(), line);
  return result;
}

std::any AstBuilderVisitor::visitBinaryOperation(RalParser::BinaryOperationContext *ctx) {
  int line = ctx->getStart()->getLine();
  AstTokenType type = ctx->Add() ? AstTokenType::PLUS : (ctx->Sub() ? AstTokenType::MINUS : AstTokenType::_COUNT);
  assert(type != AstTokenType::_COUNT);

  auto astMathExpression = AstMathExpression::create(line, type, m_symbolTable.getCurrentScope());
  auto result = std::dynamic_pointer_cast<AstExpression>(astMathExpression);
  addBinarySubExpressions(result, ctx->expression(), line);
  return result;
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
  auto result = AstNumberLiteralExpression::create(line, TypeKind::Int, text, m_symbolTable.getCurrentScope());
  return std::dynamic_pointer_cast<AstExpression>(result);
}

std::any AstBuilderVisitor::visitLogicalAnd(RalParser::LogicalAndContext *ctx) {
  int line = ctx->getStart()->getLine();
  return createBinaryLogicalExpression(AstTokenType::LOGICAL_AND, ctx->expression(), line);
}

std::any AstBuilderVisitor::visitLogicalNot(RalParser::LogicalNotContext *ctx) {
  int line = ctx->getStart()->getLine();
  return createUnaryExpression(line, AstTokenType::LOGICAL_NOT, ctx->expression());
}

std::any AstBuilderVisitor::visitLogicalOr(RalParser::LogicalOrContext *ctx) {
  int line = ctx->getStart()->getLine();
  return createBinaryLogicalExpression(AstTokenType::LOGICAL_OR, ctx->expression(), line);
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
    auto expressionStatement = AstExpressionStatement::create(line, m_symbolTable.getCurrentScope());
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

std::any AstBuilderVisitor::visitStringLiteral(RalParser::StringLiteralContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string text = "\n";
  auto literal = ctx->StringLiteral();
  if (literal) {
    text = literal->getSymbol()->getText();
    unquote(text);
  }
  auto result = AstStringLiteralExpression::create(line, text, m_symbolTable.getCurrentScope());
  return std::dynamic_pointer_cast<AstExpression>(result);
}

std::shared_ptr<AstExpression>
AstBuilderVisitor::createUnaryExpression(int line, AstTokenType type, RalParser::ExpressionContext *expressionContext) {
  auto astUnaryExpression = AstUnaryExpression::create(line, type, m_symbolTable.getCurrentScope());
  std::any childResult = expressionContext->accept(this);
  if (childResult.has_value()) {
    auto astExpression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
    astUnaryExpression->addNode(astExpression);
  } else {
    throw NotImplementedException();
  }
  return std::dynamic_pointer_cast<AstExpression>(astUnaryExpression);
}

std::shared_ptr<AstExpression>
AstBuilderVisitor::createBinaryLogicalExpression(AstTokenType type,
                                                 std::vector<RalParser::ExpressionContext *> expressions, int line) {
  auto astBinaryLogicalExpression = AstBinaryLogicalExpression::create(line, type, m_symbolTable.getCurrentScope());
  auto result = std::dynamic_pointer_cast<AstExpression>(astBinaryLogicalExpression);
  addBinarySubExpressions(result, expressions, line);
  return result;
}

std::shared_ptr<AstExpression>
AstBuilderVisitor::createMathExpression(AstTokenType type, std::vector<RalParser::ExpressionContext *> expressions,
                                        int line) {
  auto astMathExpression = AstMathExpression::create(line, type, m_symbolTable.getCurrentScope());
  auto result = std::dynamic_pointer_cast<AstExpression>(astMathExpression);
  addBinarySubExpressions(result, expressions, line);
  return result;
}

void AstBuilderVisitor::addBinarySubExpressions(std::shared_ptr<AstExpression> &astExpr,
                                                std::vector<RalParser::ExpressionContext *> expressions, int line) {
  assert(expressions.size() == 2);
  for (unsigned i = 0; i < 2; i++) {
    auto expressionContext = expressions[i];
    std::any childResult = expressionContext->accept(this);
    if (childResult.has_value()) {
      auto astExpression = std::any_cast<std::shared_ptr<AstExpression>>(childResult);
      astExpr->addNode(astExpression);
    } else {
      throw NotImplementedException("Unexpected statement at " + std::to_string(line));
    }
  }
}

std::shared_ptr<AstExpression>
AstBuilderVisitor::createCallExpression(const std::string &name, std::vector<RalParser::ExpressionContext *> callArgs,
                                        int line) {
  auto *calleeSymbol = resolveAlgorithm(m_symbolTable.getCurrentScope(), name, line);
  std::vector<Symbol *> formalParameters = calleeSymbol->getFormalParameters();
  if (formalParameters.size() != callArgs.size()) {
    throw VariableNotFoundException("Incorrect # arguments passed to " + name + ". Expected " +
                                    std::to_string(formalParameters.size()) + ", actual " +
                                    std::to_string(callArgs.size()) + ", line: " + std::to_string(line));
  }

  auto functionCallExpression = AstAlgorithmCallExpression::create(line, name, m_symbolTable.getCurrentScope());
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

std::any AstBuilderVisitor::visitUnaryNegativeExpression(RalParser::UnaryNegativeExpressionContext *ctx) {
  int line = ctx->getStart()->getLine();
  return createUnaryExpression(line, AstTokenType::UNARI_MINUS, ctx->expression());
}

std::any AstBuilderVisitor::visitFunctionCall(RalParser::FunctionCallContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = getAlgorithmName(ctx->algorithmName());
  std::vector<RalParser::ExpressionContext *> callArgs;
  if (ctx->args()) {
    callArgs = ctx->args()->expression();
  }
  return createCallExpression(name, callArgs, line);
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

  std::vector<std::shared_ptr<AstStatement>> astElseBlock;
  if (ctx->elseInstructions()) {
    auto elseBlockContext = ctx->elseInstructions()->instructions();
    childResult = elseBlockContext->accept(this);
    if (childResult.has_value()) {
      astElseBlock = std::any_cast<std::vector<std::shared_ptr<AstStatement>>>(childResult);
    }
  }

  auto astIfStatement =
      AstIfStatement::create(line, m_symbolTable.getCurrentScope(), astIfCondition, astThenBlock, astElseBlock);
  return std::dynamic_pointer_cast<AstStatement>(astIfStatement);
}

std::any AstBuilderVisitor::visitVariableDeclaration(RalParser::VariableDeclarationContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string typeName = ctx->type()->getText();
  std::vector<RalParser::SingleVariableDeclarationContext *> variablesCtx = ctx->singleVariableDeclaration();

  std::vector<std::shared_ptr<AstStatement>> statements;
  for (unsigned i = 0; i < variablesCtx.size(); i++) {
    std::string variableName = variablesCtx[i]->Id()->getSymbol()->getText();
    auto *type = resolveType(m_symbolTable.getCurrentScope(), typeName);
    auto variableDeclarationStatement =
        AstVariableDeclarationStatement::create(line, variableName, m_symbolTable.getCurrentScope(), typeName);
    if (variablesCtx[i]->expression() != nullptr) {
      std::any childResult = variablesCtx[i]->expression()->accept(this);
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

std::shared_ptr<AstExpression> AstBuilderVisitor::createVariableExpression(int line, const std::string &name) {
  resolveVariable(m_symbolTable.getCurrentScope(), name, line); // just check it is a variable
  return std::dynamic_pointer_cast<AstExpression>(
      AstVariableExpression::create(line, name, m_symbolTable.getCurrentScope()));
}

std::string getAlgorithmName(RalParser::AlgorithmNameContext *ctx) {
  auto algorithmNameWords = ctx->Id();
  assert(algorithmNameWords.size() > 0);
  std::string name;
  for (auto word : algorithmNameWords) {
    name += word->getSymbol()->getText();
    name += " ";
  }
  trim(name);
  return name;
}

std::any AstBuilderVisitor::visitVariableAffectation(RalParser::VariableAffectationContext *ctx) {
  int line = ctx->getStart()->getLine();
  std::string name = ctx->Id()->getSymbol()->getText();
  auto variableAffectation = AstVariableAffectationExpression::create(line, name, m_symbolTable.getCurrentScope());

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
  auto functionAffectation = AstFunctionAffectationExpression::create(line, name, m_symbolTable.getCurrentScope());

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
