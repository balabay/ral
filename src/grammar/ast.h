#pragma once

#include <memory>
#include <string>
#include <vector>

namespace llvm {
class Value;
}

namespace RaLang {

class AstModule;
class AstAlgorithm;
class AstReturnStatement;
class Type;

class Generator {
public:
  virtual llvm::Value *visit(AstModule *module) = 0;
  virtual llvm::Value *visit(AstAlgorithm *algorithm) = 0;
  virtual llvm::Value *visit(AstReturnStatement *algorithm) = 0;
};

class Token {
public:
  enum Type { INT, FLOAT, MODULE, ALGORITHM, RETURN };

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

  virtual llvm::Value *accept(Generator *v) = 0;

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
  llvm::Value *accept(Generator *v) override;
  const std::string &getName() const;

private:
  std::string m_name;
};

class AstModule : public AstNode {
public:
  AstModule(int line, const Token &token, const std::string &name);
  static std::shared_ptr<AstModule> create(const std::string &name,
                                           int line = 0);
  const std::string &getName() const;
  llvm::Value *accept(Generator *v) override;

private:
  std::string m_name;
};

class AstStatement : public AstNode {
public:
  using AstNode::AstNode;
};

class AstReturnStatement : public AstStatement {
public:
  AstReturnStatement(int line, const Token &token);
  static std::shared_ptr<AstReturnStatement> create(int line = 0);
  llvm::Value *accept(Generator *v) override;
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
