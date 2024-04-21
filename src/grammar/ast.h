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
class AstModule;
class AstExpressionStatement;
class AstReturnStatement;

class Type;

class GeneratorVisitor {
public:
  virtual llvm::Value *visit(AstAlgorithm *algorithm) = 0;
  virtual llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) = 0;
  virtual llvm::Value *visit(AstExpressionStatement *expressionStatement) = 0;
  virtual llvm::Value *visit(AstModule *module) = 0;
  virtual llvm::Value *visit(AstReturnStatement *returnStatement) = 0;
};

class Token {
public:
  enum Type {
    MODULE,
    ALGORITHM,
    // Statements
    RETURN,
    EXPRESSION_STATEMENT,
    // Expressions
    INT,
    FLOAT,
    ALGORITHM_CALL
  };

  Token(Type type, const std::string &text);
  Type getType() const;

  const std::string &getValue() const;
  const std::string &toString() const;

private:
  Type m_type;
  std::string m_text;
};

class AstNode {
public:
  AstNode(int line, const Token &token);
  virtual ~AstNode();

  int getLine() const;
  Token::Type getType() const;
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
  static std::shared_ptr<AstAlgorithm> create(const std::string &name,
                                              int line = 0);
  llvm::Value *accept(GeneratorVisitor *v) override;
  const std::string &getName() const;

private:
  std::string m_name;
};

class AstModule : public AstNode {
public:
  AstModule(int line, const Token &token);
  static std::shared_ptr<AstModule> create(const std::string &name,
                                           int line = 0);
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
  static std::shared_ptr<AstReturnStatement> create(int line = 0);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstExpressionStatement : public AstStatement {
public:
  using AstStatement::AstStatement;
  static std::shared_ptr<AstExpressionStatement> create(int line = 0);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstExpression : public AstNode {
public:
  using AstNode::AstNode;
};

class AstAlgorithmCallExpression : public AstExpression {
public:
  AstAlgorithmCallExpression(int line, const Token &token);
  static std::shared_ptr<AstAlgorithmCallExpression>
  create(const std::string &name, int line = 0);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
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
