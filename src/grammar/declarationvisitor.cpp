#include "declarationvisitor.h"

#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"

namespace RaLang {

DeclarationVisitor::DeclarationVisitor(SymbolTable &symbolTable) : m_symbolTable(symbolTable) {}

std::any DeclarationVisitor::visitAlgorithmPrototype(RalParser::AlgorithmPrototypeContext *ctx) {

  // Get type of the function
  std::string returnType;
  if (ctx->type()) {
    returnType = ctx->type()->getText();
  }
  // no type means void function

  Scope *scope = m_symbolTable.getCurrentScope();
  // Resolve function return type in the Symbol Table
  auto resolvedType = resolveType(scope, returnType);

  // Get Name of the function
  std::string functionName = ctx->Id()->getText();

  // Resolve the function name in the Symbol Table. It should not be resolved
  auto functionSymbol = scope->resolve(functionName);
  if (functionSymbol != nullptr) {
    throw VariableNotFoundException("redefinition of the function " + functionName);
  }

  // Add the function to the Symbol Table
  AlgSymbol *symbol = m_symbolTable.createAlgSymbol(functionName, resolvedType);
  scope->define(std::unique_ptr<Symbol>(symbol));
  m_symbolTable.pushScope(symbol);
  visitChildren(ctx);
  m_symbolTable.popScope();
  return {};
}

std::any DeclarationVisitor::visitFormalParameters(RalParser::FormalParametersContext *ctx) {
  std::vector<RalParser::TypeContext *> parameterTypes = ctx->type();
  std::vector<antlr4::tree::TerminalNode *> parameterIds = ctx->Id();
  assert(parameterTypes.size() == parameterIds.size());
  size_t numberOfParameters = parameterTypes.size();
  Scope *scope = m_symbolTable.getCurrentScope();
  for (size_t i = 0; i < numberOfParameters; i++) {
    Type *parameterType = resolveType(scope, parameterTypes[i]->getText());
    std::string name = parameterIds[i]->getSymbol()->getText();
    VariableSymbol *parameter = m_symbolTable.createVariableSymbol(name, parameterType);
    scope->define(std::unique_ptr<Symbol>(parameter));
  }
  return visitChildren(ctx);
}

} // namespace RaLang
