#include "astbuildervisitor.h"

#include "grammar/ast.h"
#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"

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
    auto algorithmBody = std::any_cast<std::shared_ptr<AstNode>>(childResult);
    algorithm->addNode(algorithmBody);
  }
  m_symbolTable.popScope();

  return algorithm;
}

std::any
AstBuilderVisitor::visitAlgorithmBody(RalParser::AlgorithmBodyContext *ctx) {
  return visitChildren(ctx);
}

} // namespace RaLang
