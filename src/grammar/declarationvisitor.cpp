#include "declarationvisitor.h"

#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"

namespace RaLang {

DeclarationVisitor::DeclarationVisitor(SymbolTable &symbolTable) : m_symbolTable(symbolTable) {}

extern std::string getAlgorithmName(RalParser::AlgorithmNameContext *ctx);

std::any DeclarationVisitor::visitAlgorithmPrototype(RalParser::AlgorithmPrototypeContext *ctx) {

  // Get type of the function
  std::string returnType;
  if (ctx->type()) {
    returnType = ctx->type()->getText();
  }
  // no type means void function

  Scope *scope = m_symbolTable.getGlobals();
  // Resolve function return type in the Symbol Table
  auto resolvedType = resolveType(scope, returnType);

  // Get Name of the function
  std::string functionName = getAlgorithmName(ctx->algorithmName());

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
  std::vector<RalParser::FormalParameterContext *> formalParameterCtxs = ctx->formalParameter();
  size_t numberOfParameters = formalParameterCtxs.size();
  Scope *scope = m_symbolTable.getCurrentScope();
  Type *parameterType = nullptr;
  bool reference = false;
  for (size_t i = 0; i < numberOfParameters; i++) {
    antlr4::tree::TerminalNode *parameterId = formalParameterCtxs[i]->Id();
    std::string name = parameterId->getSymbol()->getText();
    RalParser::TypeContext *parameterTypeCtx = formalParameterCtxs[i]->type();
    if (parameterTypeCtx == nullptr) {
      if (parameterType == nullptr) {
        int line = ctx->getStart()->getLine();
        throw VariableNotFoundException("Parameter type not specified for " + name + " (line " + std::to_string(line) +
                                        ")");
      }
    } else {
      std::string parameterTypeName = parameterTypeCtx->getText();
      parameterType = resolveType(scope, parameterTypeName);
    }
    const auto &parameterModifiers = formalParameterCtxs[i]->parameterModifier();
    if (parameterModifiers.size()) {
      reference = false;
      for (const auto &modifier : parameterModifiers) {
        if (modifier->WriteOnlyParameterModifier() || modifier->ReadWriteParameterModifier()) {
          reference = true;
        }
      }
    }
    VariableSymbol *parameter = m_symbolTable.createVariableSymbol(name, parameterType);
    parameter->setReference(reference);
    scope->define(std::unique_ptr<Symbol>(parameter));
  }
  return visitChildren(ctx);
}

} // namespace RaLang
