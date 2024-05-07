#include "ast.h"

#include "ralexceptions.h"

#include <assert.h>

namespace RaLang {

void Ast::add(std::shared_ptr<AstModule> module) { m_modules.push_back(module); }

const std::vector<std::shared_ptr<AstModule>> &Ast::getNodes() const { return m_modules; }

std::string Ast::dump() {
  std::string result;
  for (auto &module : getNodes()) {
    result += module->toString(0);
  }
  return result;
}

Token::Token(Type type, const std::string &text) : m_type(type), m_text(text) {}

Token::Type Token::getType() const { return m_type; }

const std::string &Token::getValue() const { return m_text; }

const std::string &Token::toString() const { return m_text; }

AstNode::AstNode(int line, const Token &token) : m_line(line), m_token(token) {}

AstNode::~AstNode() = default;

int AstNode::getLine() const { return m_line; }

Token::Type AstNode::getTokenType() const { return m_token.getType(); }

const std::string &AstNode::getValue() const { return m_token.getValue(); }

std::string AstNode::toString(int level) {
  std::string intend(level, '\t');
  std::string result = intend;
  result += m_token.toString() + " line: " + std::to_string(getLine()) + " value: " + getValue();
  result += '\n';
  for (auto &node : getNodes()) {
    result += node->toString(level + 1);
  }
  return result;
}

void AstNode::addNode(std::shared_ptr<AstNode> node) { m_nodes.push_back(node); }

const std::vector<std::shared_ptr<AstNode>> &AstNode::getNodes() const { return m_nodes; }

AstAlgorithm::AstAlgorithm(int line, const Token &token) : AstNode(line, token), m_name(token.getValue()) {}

std::shared_ptr<AstAlgorithm> AstAlgorithm::create(const std::string &name, int line) {
  Token token(Token::ALGORITHM, name);
  return std::make_shared<AstAlgorithm>(line, token);
}

llvm::Value *AstAlgorithm::accept(GeneratorVisitor *v) { return v->visit(this); }

const std::string &AstAlgorithm::getName() const { return m_name; }

AstModule::AstModule(int line, const Token &token) : AstNode(line, token), m_name(token.getValue()) {}

std::shared_ptr<AstModule> AstModule::create(const std::string &name, int line) {
  Token token(Token::MODULE, name);
  return std::make_shared<AstModule>(line, token);
}

const std::string &AstModule::getName() const { return m_name; }

llvm::Value *AstModule::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstReturnStatement> AstReturnStatement::create(int line) {
  Token token(Token::RETURN_STATEMENT, "RETURN");
  return std::make_shared<AstReturnStatement>(line, token);
}

llvm::Value *AstReturnStatement::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstExpressionStatement> AstExpressionStatement::create(int line) {
  Token token(Token::EXPRESSION_STATEMENT, "EXPRESSION_STATEMENT");
  return std::make_shared<AstExpressionStatement>(line, token);
}

llvm::Value *AstExpressionStatement::accept(GeneratorVisitor *v) { return v->visit(this); }

AstAlgorithmCallExpression::AstAlgorithmCallExpression(int line, const Token &token)
    : AstExpression(line, token), m_name(token.getValue()) {}

std::shared_ptr<AstAlgorithmCallExpression> AstAlgorithmCallExpression::create(const std::string &name, int line) {
  Token token(Token::ALGORITHM_CALL, name);
  return std::make_shared<AstAlgorithmCallExpression>(line, token);
}

const std::string &AstAlgorithmCallExpression::getName() const { return m_name; }

llvm::Value *AstAlgorithmCallExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstIntExpression> AstIntExpression::create(const std::string &text, int line) {
  Token token(Token::INT, text);
  return std::make_shared<AstIntExpression>(line, token);
}

llvm::Value *AstIntExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

AstVariableDeclarationStatement::AstVariableDeclarationStatement(int line, const Token &token,
                                                                 const std::string typeName)
    : AstStatement(line, token), m_name(token.getValue()), m_typeName(typeName) {}

std::shared_ptr<AstVariableDeclarationStatement>
AstVariableDeclarationStatement::create(const std::string &name, const std::string &typeName, int line) {
  Token token(Token::VARIABLE_DECLARATION_STATEMENT, name);
  return std::make_shared<AstVariableDeclarationStatement>(line, token, typeName);
}

llvm::Value *AstVariableDeclarationStatement::accept(GeneratorVisitor *v) { return v->visit(this); }

const std::string &AstVariableDeclarationStatement::getName() const { return m_name; }

const std::string &AstVariableDeclarationStatement::getTypeName() const { return m_typeName; }

AstVariableExpression::AstVariableExpression(int line, const Token &token)
    : AstExpression(line, token), m_name(token.getValue()) {}

std::shared_ptr<AstVariableExpression> AstVariableExpression::create(const std::string &name, int line) {
  Token token(Token::VARIABLE_EXPRESSION, name);
  return std::make_shared<AstVariableExpression>(line, token);
}

const std::string &AstVariableExpression::getName() const { return m_name; }

llvm::Value *AstVariableExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstPrintStatement> AstPrintStatement::create(int line) {
  Token token(Token::PRINT_STATEMENT, "PRINT_STATEMENT");
  return std::make_shared<AstPrintStatement>(line, token);
}

llvm::Value *AstPrintStatement::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstInputStatement> AstInputStatement::create(int line) {
  Token token(Token::INPUT_STATEMENT, "INPUT_STATEMENT");
  return std::make_shared<AstInputStatement>(line, token);
}

llvm::Value *AstInputStatement::accept(GeneratorVisitor *v) { return v->visit(this); }

AstVariableAffectationExpression::AstVariableAffectationExpression(int line, const Token &token)
    : AstExpression(line, token), m_name(token.getValue()) {}

std::shared_ptr<AstVariableAffectationExpression> AstVariableAffectationExpression::create(const std::string &name,
                                                                                           int line) {
  Token token(Token::VARIABLE_AFFECTATION_EXPRESSION, name);
  return std::make_shared<AstVariableAffectationExpression>(line, token);
}

const std::string &AstVariableAffectationExpression::getName() const { return m_name; }

llvm::Value *AstVariableAffectationExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

AstFunctionAffectationExpression::AstFunctionAffectationExpression(int line, const Token &token)
    : AstExpression(line, token), m_name(token.getValue()) {}

std::shared_ptr<AstFunctionAffectationExpression> AstFunctionAffectationExpression::create(const std::string &name,
                                                                                           int line) {
  Token token(Token::FUNCTION_AFFECTATION_EXPRESSION, name);
  return std::make_shared<AstFunctionAffectationExpression>(line, token);
}

const std::string &AstFunctionAffectationExpression::getName() const { return m_name; }

llvm::Value *AstFunctionAffectationExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstMathExpression> AstMathExpression::create(const std::string &operation, int line) {
  assert(operation.size() == 1);
  char op = operation[0];
  Token::Type t;
  switch (op) {
  case '*':
    t = Token::MUL;
    break;
  case '/':
    t = Token::DIV;
    break;
  case '%':
    t = Token::MOD;
    break;
  case '-':
    t = Token::MINUS;
    break;
  case '+':
    t = Token::PLUS;
    break;
  defult : { throw NotImplementedException(); }
  }
  Token token(t, operation);
  return std::make_shared<AstMathExpression>(line, token);
}

llvm::Value *AstMathExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstBinaryConditionalExpression> AstBinaryConditionalExpression::create(const std::string &operation,
                                                                                       int line) {
  Token::Type t;
  if (operation == "=") {
    t = Token::COND_EQ;
  } else if (operation == "!=") {
    t = Token::COND_NE;
  } else if (operation == ">") {
    t = Token::COND_GT;
  } else if (operation == ">=") {
    t = Token::COND_GE;
  } else if (operation == "<") {
    t = Token::COND_LT;
  } else if (operation == "<=") {
    t = Token::COND_LE;
  } else {
    throw NotImplementedException();
  }
  Token token(t, operation);
  return std::make_shared<AstBinaryConditionalExpression>(line, token);
}

llvm::Value *AstBinaryConditionalExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstUnaryExpression> AstUnaryExpression::create(const std::string &operation, int line) {
  assert(operation.size() == 1);
  char op = operation[0];
  Token::Type t;
  switch (op) {
  case '-':
    t = Token::UNARI_MINUS;
    break;
  defult : { throw NotImplementedException(); }
  }
  Token token(t, "UNARY_MINUS");
  return std::make_shared<AstUnaryExpression>(line, token);
}

llvm::Value *AstUnaryExpression::accept(GeneratorVisitor *v) { return v->visit(this); }

AstIfStatement::AstIfStatement(int line, const Token &token, std::shared_ptr<AstExpression> ifCondition,
                               std::vector<std::shared_ptr<AstStatement>> thenBlock,
                               std::vector<std::shared_ptr<AstStatement>> elseBlock)
    : AstStatement(line, token), m_ifCondition(std::move(ifCondition)), m_thenBlock(std::move(thenBlock)),
      m_elseBlock(std::move(elseBlock)) {}

std::shared_ptr<AstIfStatement> AstIfStatement::create(int line, std::shared_ptr<AstExpression> ifCondition,
                                                       std::vector<std::shared_ptr<AstStatement>> thenBlock,
                                                       std::vector<std::shared_ptr<AstStatement>> elseBlock) {
  Token token(Token::IF_STATEMENT, "IF");
  return std::shared_ptr<AstIfStatement>(
      new AstIfStatement(line, token, std::move(ifCondition), std::move(thenBlock), std::move(elseBlock)));
}

bool AstIfStatement::hasElse() const { return m_elseBlock.size() > 0; }

llvm::Value *AstIfStatement::accept(GeneratorVisitor *v) { return v->visit(this); }

std::shared_ptr<AstExpression> AstIfStatement::ifCondition() const { return m_ifCondition; }

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

} // namespace RaLang
