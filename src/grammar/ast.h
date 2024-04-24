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
class AstIntExpression;
class AstModule;
class AstExpressionStatement;
class AstReturnStatement;
class AstVariableDeclarationStatement;
class AstVariableExpression;

class GeneratorVisitor {
public:
  virtual llvm::Value *visit(AstAlgorithm *algorithm) = 0;
  virtual llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall) = 0;
  virtual llvm::Value *visit(AstExpressionStatement *expressionStatement) = 0;
  virtual llvm::Value *visit(AstIntExpression *expression) = 0;
  virtual llvm::Value *visit(AstModule *module) = 0;
  virtual llvm::Value *visit(AstReturnStatement *returnStatement) = 0;
  virtual llvm::Value *visit(AstVariableDeclarationStatement *statement) = 0;
  virtual llvm::Value *visit(AstVariableExpression *expression) = 0;
};

class Token {
public:
  enum Type {
    MODULE,
    ALGORITHM,
    // Statements
    RETURN,
    EXPRESSION_STATEMENT,
    VARIABLE_DECLARATION_STATEMENT,
    // Expressions
    INT,
    FLOAT,
    VARIABLE_EXPRESSION,
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
  Token::Type getTokenType() const;
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
                                              int line);
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

class AstExpression : public AstNode {
public:
  using AstNode::AstNode;
};

class AstAlgorithmCallExpression : public AstExpression {
public:
  AstAlgorithmCallExpression(int line, const Token &token);
  static std::shared_ptr<AstAlgorithmCallExpression>
  create(const std::string &name, int line);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstIntExpression : public AstExpression {
public:
  using AstExpression::AstExpression;
  static std::shared_ptr<AstIntExpression> create(const std::string &text,
                                                  int line);
  llvm::Value *accept(GeneratorVisitor *v) override;
};

class AstVariableExpression : public AstExpression {
public:
  AstVariableExpression(int line, const Token &token);
  static std::shared_ptr<AstVariableExpression> create(const std::string &name,
                                                       int line);
  const std::string &getName() const;
  llvm::Value *accept(GeneratorVisitor *v) override;

private:
  std::string m_name;
};

class AstVariableDeclarationStatement : public AstStatement {
public:
  AstVariableDeclarationStatement(int line, const Token &token,
                                  const std::string typeName);
  static std::shared_ptr<AstVariableDeclarationStatement>
  create(const std::string &name, const std::string &typeName, int line);
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
