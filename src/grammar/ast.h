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
class AstIntLiteralExpression;
class AstMathExpression;
class AstModule;
class AstExpressionStatement;
class AstIfStatement;
class AstInputStatement;
class AstPrintStatement;
class AstReturnStatement;
class AstStringLiteralExpression;
class AstUnaryExpression;
class AstVariableAffectationExpression;
class AstVariableDeclarationStatement;
class AstVariableExpression;

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
  virtual llvm::Value *visit(AstIntLiteralExpression *expression) = 0;
  virtual void visit(AstModule *module) = 0;
  virtual void visit(AstReturnStatement *returnStatement) = 0;
  virtual void visit(AstPrintStatement *statement) = 0;
  virtual llvm::Value *visit(AstStringLiteralExpression *expression) = 0;
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
  INT_LITERAL,
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
  VARIABLE_EXPRESSION
};

class Token {
public:
  Token(AstTokenType type, const std::string &text);
  AstTokenType getType() const;

  const std::string &getValue() const;
  const std::string &toString() const;

private:
  AstTokenType m_type;
  std::string m_text;
};

class AstNode {
public:
  AstNode(int line, const Token &token);
  virtual ~AstNode();

  int getLine() const;
  AstTokenType getTokenType() const;
  const std::string &getValue() const;
  virtual std::string toString(int level);

  void addNode(std::shared_ptr<AstNode> node);

  const std::vector<std::shared_ptr<AstNode>> &getNodes() const;

  virtual llvm::Value *accept(GeneratorVisitor *v) = 0;

private:
  int m_line;
  Token m_token;
  std::vector<std::shared_ptr<AstNode>> m_nodes;
};

class AstAlgorithm : public AstNode {
public:
  AstAlgorithm(int line, const Token &token);
  static std::shared_ptr<AstAlgorithm> create(const std::string &name, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
  const std::string &getName() const;

private:
  std::string m_name;
};

class AstModule : public AstNode {
public:
  AstModule(int line, const Token &token);
  static std::shared_ptr<AstModule> create(const std::string &name, int line);
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
public:
  using AstStatement::AstStatement;
  static std::shared_ptr<AstReturnStatement> create(int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstExpressionStatement : public AstStatement {
public:
  using AstStatement::AstStatement;
  static std::shared_ptr<AstExpressionStatement> create(int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstPrintStatement : public AstStatement {
public:
  using AstStatement::AstStatement;
  static std::shared_ptr<AstPrintStatement> create(int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstInputStatement : public AstStatement {
public:
  using AstStatement::AstStatement;
  static std::shared_ptr<AstInputStatement> create(int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstExpression : public AstNode {
public:
  using AstNode::AstNode;
};

class AstIfStatement : public AstStatement {
protected:
  AstIfStatement(int line, const Token &token, std::shared_ptr<AstExpression> ifCondition,
                 std::vector<std::shared_ptr<AstStatement>> thenBlock,
                 std::vector<std::shared_ptr<AstStatement>> elseBlock);

public:
  static std::shared_ptr<AstIfStatement> create(int line, std::shared_ptr<AstExpression> ifCondition,
                                                std::vector<std::shared_ptr<AstStatement>> thenBlock,
                                                std::vector<std::shared_ptr<AstStatement>> elseBlock);
  bool hasElse() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

  std::shared_ptr<AstExpression> ifCondition() const;
  std::vector<std::shared_ptr<AstStatement>> thenBlock() const;
  std::vector<std::shared_ptr<AstStatement>> elseBlock() const;
  std::string toString(int level) override;

private:
  std::shared_ptr<AstExpression> m_ifCondition;
  std::vector<std::shared_ptr<AstStatement>> m_thenBlock;
  std::vector<std::shared_ptr<AstStatement>> m_elseBlock;
};

class AstAlgorithmCallExpression : public AstExpression {
public:
  AstAlgorithmCallExpression(int line, const Token &token);
  static std::shared_ptr<AstAlgorithmCallExpression> create(const std::string &name, int line);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstIntLiteralExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstIntLiteralExpression> create(const std::string &text, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstStringLiteralExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstStringLiteralExpression> create(const std::string &text, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstMathExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstMathExpression> create(const std::string &operation, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstUnaryExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstUnaryExpression> create(AstTokenType type, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstBinaryConditionalExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstBinaryConditionalExpression> create(const std::string &operation, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstBinaryLogicalExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstBinaryLogicalExpression> create(AstTokenType type, int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstVariableExpression : public AstExpression {
public:
  AstVariableExpression(int line, const Token &token);
  static std::shared_ptr<AstVariableExpression> create(const std::string &name, int line);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstVariableAffectationExpression : public AstExpression {
public:
  AstVariableAffectationExpression(int line, const Token &token);
  static std::shared_ptr<AstVariableAffectationExpression> create(const std::string &name, int line);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstFunctionAffectationExpression : public AstExpression {
public:
  AstFunctionAffectationExpression(int line, const Token &token);
  static std::shared_ptr<AstFunctionAffectationExpression> create(const std::string &name, int line);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstVariableDeclarationStatement : public AstStatement {
public:
  AstVariableDeclarationStatement(int line, const Token &token, const std::string typeName);
  static std::shared_ptr<AstVariableDeclarationStatement> create(const std::string &name, const std::string &typeName,
                                                                 int line);
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
