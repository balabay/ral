#pragma once

#include <memory>
#include <string>
#include <vector>

namespace llvm {
class Value;
}

namespace RaLang {

class AstAlgorithm;
class AstAlgorithmCallExpression;
class AstBinaryConditionalExpression;
class AstBinaryLogicalExpression;
class AstFunctionAffectationExpression;
class AstNumberLiteralExpression;
class AstMathExpression;
class AstModule;
class AstExpressionStatement;
class AstIfStatement;
class AstInputStatement;
class AstPrintStatement;
class AstReturnStatement;
class AstStringLiteralExpression;
class AstTypePromotionExpression;
class AstUnaryExpression;
class AstVariableAffectationExpression;
class AstVariableDeclarationStatement;
class AstVariableExpression;

class Scope;

enum class TypeKind : uint8_t;

class GeneratorVisitor {
public:
  virtual void visit(AstAlgorithm *algorithm) = 0;
  virtual llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) = 0;
  virtual llvm::Value *visit(AstBinaryConditionalExpression *expression) = 0;
  virtual llvm::Value *visit(AstBinaryLogicalExpression *expression) = 0;
  virtual llvm::Value *visit(AstMathExpression *expression) = 0;
  virtual void visit(AstExpressionStatement *expressionStatement) = 0;
  virtual llvm::Value *visit(AstFunctionAffectationExpression *expression) = 0;
  virtual void visit(AstIfStatement *statement) = 0;
  virtual void visit(AstInputStatement *statement) = 0;
  virtual llvm::Value *visit(AstNumberLiteralExpression *expression) = 0;
  virtual void visit(AstModule *module) = 0;
  virtual void visit(AstReturnStatement *returnStatement) = 0;
  virtual void visit(AstPrintStatement *statement) = 0;
  virtual llvm::Value *visit(AstStringLiteralExpression *expression) = 0;
  virtual llvm::Value *visit(AstTypePromotionExpression *expression) = 0;
  virtual llvm::Value *visit(AstUnaryExpression *expression) = 0;
  virtual void visit(AstVariableDeclarationStatement *statement) = 0;
  virtual llvm::Value *visit(AstVariableExpression *expression) = 0;
  virtual llvm::Value *visit(AstVariableAffectationExpression *expression) = 0;
};

enum class AstTokenType {
  ALGORITHM,
  MODULE,
  // Statements
  EXPRESSION_STATEMENT,
  IF_STATEMENT,
  INPUT_STATEMENT,
  PRINT_STATEMENT,
  RETURN_STATEMENT,
  VARIABLE_DECLARATION_STATEMENT,
  // Expressions
  ALGORITHM_CALL,
  COND_EQ,
  COND_GE,
  COND_GT,
  COND_LE,
  COND_LT,
  COND_NE,
  DIV,
  FLOAT,
  FUNCTION_AFFECTATION_EXPRESSION,
  NUMBER_LITERAL,
  STRING_LITERAL,
  LOGICAL_NOT,
  LOGICAL_AND,
  LOGICAL_OR,
  MINUS,
  MOD,
  MUL,
  PLUS,
  UNARI_MINUS,
  VARIABLE_AFFECTATION_EXPRESSION,
  VARIABLE_EXPRESSION,
  TYPE_PROMOTION_EXPRESSION,
  _COUNT
};

std::string astTokenTypeToString(AstTokenType type);

class Token {
public:
  Token(AstTokenType type, const std::string &text);
  AstTokenType getType() const;

  const std::string &getText() const;

private:
  AstTokenType m_type;
  std::string m_text;
};

class AstNode {
public:
  AstNode(int line, const Token &token, Scope *scope);
  virtual ~AstNode();

  int getLine() const;
  AstTokenType getTokenType() const;
  const std::string &getValue() const;
  virtual std::string toString(int level);

  void addNode(std::shared_ptr<AstNode> node);
  void replaceNode(size_t pos, std::shared_ptr<AstNode> node);

  const std::vector<std::shared_ptr<AstNode>> &getNodes() const;
  Scope *getScope();

  virtual llvm::Value *accept(GeneratorVisitor *v) = 0;

private:
  int m_line;
  Token m_token;
  Scope *m_scope;
  std::vector<std::shared_ptr<AstNode>> m_nodes;
};

class AstAlgorithm : public AstNode {
  AstAlgorithm(int line, const Token &token, Scope *scope, Scope *localScope);

public:
  static std::shared_ptr<AstAlgorithm> create(int line, const std::string &name, Scope *scope, Scope *localScope);
  llvm::Value *accept(GeneratorVisitor *v) override;
  const std::string &getName() const;
  Scope *getLocalScope();

private:
  std::string m_name;
  Scope *m_localScope;
};

class AstModule : public AstNode {
  AstModule(int line, const Token &token, Scope *scope);

public:
  static std::shared_ptr<AstModule> create(int line, const std::string &name, Scope *scope);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstStatement : public AstNode {
public:
  using AstNode::AstNode;
};

class AstReturnStatement : public AstStatement {
  using AstStatement::AstStatement;

public:
  static std::shared_ptr<AstReturnStatement> create(int line, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstExpressionStatement : public AstStatement {
  using AstStatement::AstStatement;

public:
  static std::shared_ptr<AstExpressionStatement> create(int line, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstPrintStatement : public AstStatement {
  using AstStatement::AstStatement;

public:
  static std::shared_ptr<AstPrintStatement> create(int line, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstInputStatement : public AstStatement {
  using AstStatement::AstStatement;

public:
  static std::shared_ptr<AstInputStatement> create(int line, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstExpression : public AstNode {
protected:
  using AstNode::AstNode;

public:
  TypeKind getTypeKind() const;
  void setTypeKind(TypeKind typeKind);

private:
  TypeKind m_typeKind{};
};

class AstIfStatement : public AstStatement {
  AstIfStatement(int line, const Token &token, Scope *scope, std::shared_ptr<AstExpression> ifCondition,
                 std::vector<std::shared_ptr<AstStatement>> thenBlock,
                 std::vector<std::shared_ptr<AstStatement>> elseBlock);

public:
  static std::shared_ptr<AstIfStatement> create(int line, Scope *scope, std::shared_ptr<AstExpression> ifCondition,
                                                std::vector<std::shared_ptr<AstStatement>> thenBlock,
                                                std::vector<std::shared_ptr<AstStatement>> elseBlock);
  bool hasElse() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

  std::shared_ptr<AstExpression> ifCondition() const;
  void replaceIfCondition(std::shared_ptr<AstExpression> expression);
  std::vector<std::shared_ptr<AstStatement>> thenBlock() const;
  std::vector<std::shared_ptr<AstStatement>> elseBlock() const;
  std::string toString(int level) override;

private:
  std::shared_ptr<AstExpression> m_ifCondition;
  std::vector<std::shared_ptr<AstStatement>> m_thenBlock;
  std::vector<std::shared_ptr<AstStatement>> m_elseBlock;
};

class AstAlgorithmCallExpression : public AstExpression {
  AstAlgorithmCallExpression(int line, const Token &token, Scope *scope);

public:
  static std::shared_ptr<AstAlgorithmCallExpression> create(int line, const std::string &name, Scope *scope);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstNumberLiteralExpression : public AstExpression {
  using AstExpression::AstExpression;

public:
  static std::shared_ptr<AstNumberLiteralExpression> create(int line, TypeKind type, const std::string &text,
                                                            Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstStringLiteralExpression : public AstExpression {
  using AstExpression::AstExpression;

public:
  static std::shared_ptr<AstStringLiteralExpression> create(int line, const std::string &text, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstTypePromotionExpression : public AstExpression {
  AstTypePromotionExpression(int line, const Token &token, Scope *scope, TypeKind typeKind);

public:
  static std::shared_ptr<AstTypePromotionExpression> create(TypeKind typeKind, std::shared_ptr<AstExpression> original);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstMathExpression : public AstExpression {
  using AstExpression::AstExpression;

public:
  static std::shared_ptr<AstMathExpression> create(int line, const std::string &operation, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstUnaryExpression : public AstExpression {
  using AstExpression::AstExpression;

public:
  static std::shared_ptr<AstUnaryExpression> create(int line, AstTokenType type, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstBinaryConditionalExpression : public AstExpression {
  using AstExpression::AstExpression;

public:
  static std::shared_ptr<AstBinaryConditionalExpression> create(int line, AstTokenType type, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstBinaryLogicalExpression : public AstExpression {
  using AstExpression::AstExpression;

public:
  static std::shared_ptr<AstBinaryLogicalExpression> create(int line, AstTokenType type, Scope *scope);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstVariableExpression : public AstExpression {
  AstVariableExpression(int line, const Token &token, Scope *scope);

public:
  static std::shared_ptr<AstVariableExpression> create(int line, const std::string &name, Scope *scope);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstVariableAffectationExpression : public AstExpression {
  AstVariableAffectationExpression(int line, const Token &token, Scope *scope);

public:
  static std::shared_ptr<AstVariableAffectationExpression> create(int line, const std::string &name, Scope *scope);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstFunctionAffectationExpression : public AstExpression {
  AstFunctionAffectationExpression(int line, const Token &token, Scope *scope);

public:
  static std::shared_ptr<AstFunctionAffectationExpression> create(int line, const std::string &name, Scope *scope);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstVariableDeclarationStatement : public AstStatement {
  AstVariableDeclarationStatement(int line, const Token &token, Scope *scope, const std::string typeName);

public:
  static std::shared_ptr<AstVariableDeclarationStatement> create(int line, const std::string &name, Scope *scope,
                                                                 const std::string &typeName);
  llvm::Value *accept(GeneratorVisitor *v) override;
  const std::string &getName() const;
  const std::string &getTypeName() const;

private:
  std::string m_name;
  std::string m_typeName;
};

class Ast {
public:
  void add(std::shared_ptr<AstModule> module);

  const std::vector<std::shared_ptr<AstModule>> &getNodes() const;
  std::string dump();

private:
  std::vector<std::shared_ptr<AstModule>> m_modules;
};

} // namespace RaLang
