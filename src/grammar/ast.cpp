#include "ast.h"

#include "ralconsts.h"
#include "ralexceptions.h"

#include <assert.h>

namespace RaLang {

static const char *AST_TOKEN_TYPE_STRINGS[] = {
    "ALGORITHM", "MODULE",
    // Statements
    "EXPRESSION_STATEMENT", "IF_STATEMENT", "INPUT_STATEMENT", "PRINT_STATEMENT", "RETURN_STATEMENT",
    "VARIABLE_DECLARATION_STATEMENT",
    // Expressions
    "ALGORITHM_CALL", "COND_EQ", "COND_GE", "COND_GT", "COND_LE", "COND_LT", "COND_NE", "DIV", "FLOAT",
    "FUNCTION_AFFECTATION_EXPRESSION", "NUMBER_LITERAL", "STRING_LITERAL", "LOGICAL_NOT", "LOGICAL_AND", "LOGICAL_OR",
    "MINUS", "MOD", "MUL", "PLUS", "UNARI_MINUS", "VARIABLE_AFFECTATION_EXPRESSION", "VARIABLE_EXPRESSION",
    "TYPE_PROMOTION_EXPRESSION"};

static_assert(std::size(AST_TOKEN_TYPE_STRINGS) == static_cast<size_t>(AstTokenType::_COUNT),
              "AST_TOKEN_TYPE_STRINGS must match AstTokenType");

std::string astTokenTypeToString(AstTokenType type) {
  size_t l = static_cast<size_t>(type);
  if (l >= 0 && l < static_cast<size_t>(AstTokenType::_COUNT)) {
    return AST_TOKEN_TYPE_STRINGS[l];
  } else {
    throw InternalException("AstTokenType is out of range: " + std::to_string(l));
  }
}

void Ast::add(std::shared_ptr<AstModule> module) { m_modules.push_back(module); }

const std::vector<std::shared_ptr<AstModule>> &Ast::getNodes() const { return m_modules; }

std::string Ast::dump() {
  std::string result;
  for (auto &module : getNodes()) {
    result += module->toString(0);
  }
  return result;
}

Token::Token(AstTokenType type, const std::string &text) : m_type(type), m_text(text) {}

AstTokenType Token::getType() const { return m_type; }

const std::string &Token::getText() const { return m_text; }

AstNode::AstNode(int line, const Token &token, Scope *scope) : m_line(line), m_token(token), m_scope(scope) {}

AstNode::~AstNode() = default;

int AstNode::getLine() const { return m_line; }

AstTokenType AstNode::getTokenType() const { return m_token.getType(); }

const std::string &AstNode::getValue() const { return m_token.getText(); }

std::string AstNode::toString(int level) {
  std::string intend(level, '\t');
  std::string result = intend;
  result += m_token.getText() + " type:" + astTokenTypeToString(m_token.getType()) +
            " line: " + std::to_string(getLine()) + " value: " + getValue();
  result += '\n';
  for (auto &node : getNodes()) {
    result += node->toString(level + 1);
  }
  return result;
}

void AstNode::addNode(std::shared_ptr<AstNode> node) { m_nodes.push_back(node); }

void AstNode::replaceNode(size_t pos, std::shared_ptr<AstNode> node) { m_nodes.at(pos) = node; }

const std::vector<std::shared_ptr<AstNode>> &AstNode::getNodes() const { return m_nodes; }

Scope *AstNode::getScope() { return m_scope; }

AstAlgorithm::AstAlgorithm(int line, const Token &token, Scope *scope, Scope *localScope)
    : AstNode(line, token, scope), m_name(token.getText()), m_localScope(localScope) {}

std::shared_ptr<AstAlgorithm> AstAlgorithm::create(int line, const std::string &name, Scope *scope, Scope *localScope) {
  Token token(AstTokenType::ALGORITHM, name);
  return std::shared_ptr<AstAlgorithm>(new AstAlgorithm(line, token, scope, localScope));
}

llvm::Value *AstAlgorithm::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

const std::string &AstAlgorithm::getName() const { return m_name; }

Scope *AstAlgorithm::getLocalScope() { return m_localScope; }

AstModule::AstModule(int line, const Token &token, Scope *scope)
    : AstNode(line, token, scope), m_name(token.getText()) {}

std::shared_ptr<AstModule> AstModule::create(int line, const std::string &name, Scope *scope) {
  Token token(AstTokenType::MODULE, name);
  return std::shared_ptr<AstModule>(new AstModule(line, token, scope));
}

const std::string &AstModule::getName() const { return m_name; }

llvm::Value *AstModule::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

std::shared_ptr<AstReturnStatement> AstReturnStatement::create(int line, Scope *scope) {
  Token token(AstTokenType::RETURN_STATEMENT, astTokenTypeToString(AstTokenType::RETURN_STATEMENT));
  return std::make_shared<AstReturnStatement>(line, token, scope);
}

llvm::Value *AstReturnStatement::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

std::shared_ptr<AstExpressionStatement> AstExpressionStatement::create(int line, Scope *scope) {
  Token token(AstTokenType::EXPRESSION_STATEMENT, astTokenTypeToString(AstTokenType::EXPRESSION_STATEMENT));
  return std::make_shared<AstExpressionStatement>(line, token, scope);
}

llvm::Value *AstExpressionStatement::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

AstAlgorithmCallExpression::AstAlgorithmCallExpression(int line, const Token &token, Scope *scope)
    : AstExpression(line, token, scope), m_name(token.getText()) {}

std::shared_ptr<AstAlgorithmCallExpression> AstAlgorithmCallExpression::create(int line, const std::string &name,
                                                                               Scope *scope) {
  Token token(AstTokenType::ALGORITHM_CALL, name);
  return std::shared_ptr<AstAlgorithmCallExpression>(new AstAlgorithmCallExpression(line, token, scope));
}

const std::string &AstAlgorithmCallExpression::getName() const { return m_name; }

llvm::Value *AstAlgorithmCallExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstNumberLiteralExpression> AstNumberLiteralExpression::create(int line, TypeKind type,
                                                                               const std::string &text, Scope *scope) {
  Token token(AstTokenType::NUMBER_LITERAL, text);
  auto result = std::make_shared<AstNumberLiteralExpression>(line, token, scope);
  result->setTypeKind(type);
  return result;
}

llvm::Value *AstNumberLiteralExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

AstVariableDeclarationStatement::AstVariableDeclarationStatement(int line, const Token &token, Scope *scope,
                                                                 const std::string typeName)
    : AstStatement(line, token, scope), m_name(token.getText()), m_typeName(typeName) {}

std::shared_ptr<AstVariableDeclarationStatement>
AstVariableDeclarationStatement::create(int line, const std::string &name, Scope *scope, const std::string &typeName) {
  Token token(AstTokenType::VARIABLE_DECLARATION_STATEMENT, name);
  return std::shared_ptr<AstVariableDeclarationStatement>(
      new AstVariableDeclarationStatement(line, token, scope, typeName));
}

llvm::Value *AstVariableDeclarationStatement::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

const std::string &AstVariableDeclarationStatement::getName() const { return m_name; }

const std::string &AstVariableDeclarationStatement::getTypeName() const { return m_typeName; }

AstVariableExpression::AstVariableExpression(int line, const Token &token, Scope *scope)
    : AstExpression(line, token, scope), m_name(token.getText()) {}

std::shared_ptr<AstVariableExpression> AstVariableExpression::create(int line, const std::string &name, Scope *scope) {
  Token token(AstTokenType::VARIABLE_EXPRESSION, name);
  return std::shared_ptr<AstVariableExpression>(new AstVariableExpression(line, token, scope));
}

const std::string &AstVariableExpression::getName() const { return m_name; }

llvm::Value *AstVariableExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstPrintStatement> AstPrintStatement::create(int line, Scope *scope) {
  Token token(AstTokenType::PRINT_STATEMENT, astTokenTypeToString(AstTokenType::PRINT_STATEMENT));
  return std::make_shared<AstPrintStatement>(line, token, scope);
}

llvm::Value *AstPrintStatement::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

std::shared_ptr<AstInputStatement> AstInputStatement::create(int line, Scope *scope) {
  Token token(AstTokenType::INPUT_STATEMENT, astTokenTypeToString(AstTokenType::INPUT_STATEMENT));
  return std::make_shared<AstInputStatement>(line, token, scope);
}

llvm::Value *AstInputStatement::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

AstVariableAffectationExpression::AstVariableAffectationExpression(int line, const Token &token, Scope *scope)
    : AstExpression(line, token, scope), m_name(token.getText()) {}

std::shared_ptr<AstVariableAffectationExpression>
AstVariableAffectationExpression::create(int line, const std::string &name, Scope *scope) {
  Token token(AstTokenType::VARIABLE_AFFECTATION_EXPRESSION, name);
  return std::shared_ptr<AstVariableAffectationExpression>(new AstVariableAffectationExpression(line, token, scope));
}

const std::string &AstVariableAffectationExpression::getName() const { return m_name; }

llvm::Value *AstVariableAffectationExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

AstFunctionAffectationExpression::AstFunctionAffectationExpression(int line, const Token &token, Scope *scope)
    : AstExpression(line, token, scope), m_name(token.getText()) {}

std::shared_ptr<AstFunctionAffectationExpression>
AstFunctionAffectationExpression::create(int line, const std::string &name, Scope *scope) {
  Token token(AstTokenType::FUNCTION_AFFECTATION_EXPRESSION, name);
  return std::shared_ptr<AstFunctionAffectationExpression>(new AstFunctionAffectationExpression(line, token, scope));
}

const std::string &AstFunctionAffectationExpression::getName() const { return m_name; }

llvm::Value *AstFunctionAffectationExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstMathExpression> AstMathExpression::create(int line, const std::string &operation, Scope *scope) {
  assert(operation.size() == 1);
  char op = operation[0];
  AstTokenType t;
  switch (op) {
  case '*':
    t = AstTokenType::MUL;
    break;
  case '/':
    t = AstTokenType::DIV;
    break;
  case '%':
    t = AstTokenType::MOD;
    break;
  case '-':
    t = AstTokenType::MINUS;
    break;
  case '+':
    t = AstTokenType::PLUS;
    break;
  defult : { throw NotImplementedException(); }
  }
  Token token(t, operation);
  return std::make_shared<AstMathExpression>(line, token, scope);
}

llvm::Value *AstMathExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstBinaryConditionalExpression> AstBinaryConditionalExpression::create(int line, AstTokenType type,
                                                                                       Scope *scope) {
  Token token(type, astTokenTypeToString(type));
  return std::make_shared<AstBinaryConditionalExpression>(line, token, scope);
}

llvm::Value *AstBinaryConditionalExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstUnaryExpression> AstUnaryExpression::create(int line, AstTokenType type, Scope *scope) {
  Token token(type, astTokenTypeToString(type));
  return std::make_shared<AstUnaryExpression>(line, token, scope);
}

llvm::Value *AstUnaryExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

AstIfStatement::AstIfStatement(int line, const Token &token, Scope *scope, std::shared_ptr<AstExpression> ifCondition,
                               std::vector<std::shared_ptr<AstStatement>> thenBlock,
                               std::vector<std::shared_ptr<AstStatement>> elseBlock)
    : AstStatement(line, token, scope), m_ifCondition(std::move(ifCondition)), m_thenBlock(std::move(thenBlock)),
      m_elseBlock(std::move(elseBlock)) {}

std::shared_ptr<AstIfStatement> AstIfStatement::create(int line, Scope *scope,
                                                       std::shared_ptr<AstExpression> ifCondition,
                                                       std::vector<std::shared_ptr<AstStatement>> thenBlock,
                                                       std::vector<std::shared_ptr<AstStatement>> elseBlock) {
  Token token(AstTokenType::IF_STATEMENT, astTokenTypeToString(AstTokenType::IF_STATEMENT));
  return std::shared_ptr<AstIfStatement>(
      new AstIfStatement(line, token, scope, std::move(ifCondition), std::move(thenBlock), std::move(elseBlock)));
}

bool AstIfStatement::hasElse() const { return m_elseBlock.size() > 0; }

llvm::Value *AstIfStatement::accept(GeneratorVisitor *v) {
  v->visit(this);
  return nullptr;
}

std::shared_ptr<AstExpression> AstIfStatement::ifCondition() const { return m_ifCondition; }

void AstIfStatement::replaceIfCondition(std::shared_ptr<AstExpression> expression) {
  assert(expression);
  m_ifCondition = expression;
}

std::vector<std::shared_ptr<AstStatement>> AstIfStatement::thenBlock() const { return m_thenBlock; }

std::vector<std::shared_ptr<AstStatement>> AstIfStatement::elseBlock() const { return m_elseBlock; }

std::string AstIfStatement::toString(int level) {
  std::string result = AstStatement::toString(level++);
  std::string intend(level, '\t');
  result += intend + "If:\n";
  result += m_ifCondition->toString(level);

  result += intend + "Then:\n";
  for (auto &node : m_thenBlock) {
    result += node->toString(level);
  }
  result += intend + "Else:\n";
  for (auto &node : m_elseBlock) {
    result += node->toString(level);
  }
  return result;
}

std::shared_ptr<AstBinaryLogicalExpression> AstBinaryLogicalExpression::create(int line, AstTokenType type,
                                                                               Scope *scope) {
  Token token(type, astTokenTypeToString(type));
  return std::make_shared<AstBinaryLogicalExpression>(line, token, scope);
}

llvm::Value *AstBinaryLogicalExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstStringLiteralExpression> AstStringLiteralExpression::create(int line, const std::string &text,
                                                                               Scope *scope) {
  Token token(AstTokenType::STRING_LITERAL, text);
  auto result = std::make_shared<AstStringLiteralExpression>(line, token, scope);
  result->setTypeKind(TypeKind::Array);
  return result;
}

llvm::Value *AstStringLiteralExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

TypeKind AstExpression::getTypeKind() const { return m_typeKind; }

void AstExpression::setTypeKind(TypeKind typeKind) { m_typeKind = typeKind; }

AstTypePromotionExpression::AstTypePromotionExpression(int line, const Token &token, Scope *scope, TypeKind typeKind)
    : AstExpression(line, token, scope) {
  setTypeKind(typeKind);
}

std::shared_ptr<AstTypePromotionExpression>
AstTypePromotionExpression::create(TypeKind typeKind, std::shared_ptr<AstExpression> original) {
  Token token(AstTokenType::TYPE_PROMOTION_EXPRESSION, astTokenTypeToString(AstTokenType::TYPE_PROMOTION_EXPRESSION));
  auto result = std::shared_ptr<AstTypePromotionExpression>(
      new AstTypePromotionExpression(original->getLine(), token, original->getScope(), typeKind));
  result->addNode(std::dynamic_pointer_cast<AstNode>(original));
  return result;
}

llvm::Value *AstTypePromotionExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

} // namespace RaLang
