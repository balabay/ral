#include "symboltable.h"

#include "ralconsts.h"

#include <llvm/IR/Function.h>

namespace RaLang {

llvm::Value *Symbol::getValue() const { return m_value; }

Symbol::Symbol(const std::string &name) : m_name(name), m_type(nullptr) {}

Symbol::Symbol(const std::string &name, Type *type, llvm::Value *value)
    : m_name(name), m_type(type), m_value(value) {}

std::string Symbol::getName() const { return m_name; }

Scope *Symbol::getScope() const { return m_scope; }

void Symbol::setScope(Scope *newScope) { m_scope = newScope; }

SymbolTable::SymbolTable() {
  m_globals = new GlobalScope();
  m_scopes.push_back(std::move(std::unique_ptr<Scope>(m_globals)));
  initTypeSystem();
}

MethodSymbol *SymbolTable::createMethodSymbol(const std::string &name,
                                              Type *type,
                                              llvm::Function *function) {
  MethodSymbol *m = new MethodSymbol(name, type, m_globals, function);
  return m;
}

VariableSymbol *SymbolTable::createVariableSymbol(const std::string &name,
                                                  Type *type,
                                                  llvm::Value *value) {
  VariableSymbol *v = new VariableSymbol(name, type, value);
  return v;
}

LocalScope *SymbolTable::createLocalScope(Scope *enclosingScope) {
  LocalScope *scope = new LocalScope(enclosingScope);
  m_scopes.emplace_back(scope);
  return scope;
}

GlobalScope *SymbolTable::getGlobals() const { return m_globals; }

void SymbolTable::initTypeSystem() {

  m_globals->define(std::make_unique<Symbol>(BuiltInTypeSymbol(RAL_INT)));
  m_globals->define(std::make_unique<Symbol>(BuiltInTypeSymbol(RAL_FLOAT)));
  m_globals->define(std::make_unique<Symbol>(BuiltInTypeSymbol(RAL_VOID)));
}

BaseScope::BaseScope(Scope *parent) { m_enclosingScope = parent; }

SymbolMap &BaseScope::getSymbols() { return m_symbols; }

Symbol *BaseScope::resolve(const std::string &name) {
  auto it = m_symbols.find(name);
  if (it != m_symbols.end()) {
    return it->second.get();
  } else if (m_enclosingScope != nullptr) {
    // if not here, check any enclosing scope
    return m_enclosingScope->resolve(name);
  }
  return nullptr;
}

void BaseScope::define(std::unique_ptr<Symbol> sym) {
  // TODO: check exists
  sym->setScope(this); // track the scope in each symbol
  m_symbols.insert(std::make_pair(sym->getName(), std::move(sym)));
}

Scope *BaseScope::getEnclosingScope() const { return m_enclosingScope; }

ScopedSymbol::ScopedSymbol(const std::string &name, Type *type,
                           Scope *enclosingScope, llvm::Value *value)
    : Symbol(name, type, value), BaseScope(enclosingScope) {}

ScopedSymbol::ScopedSymbol(const std::string &name, Scope *enclosingScope)
    : Symbol(name), BaseScope(enclosingScope) {}

Symbol *ScopedSymbol::resolveType(const std::string &name) {
  return resolve(name);
}

std::string ScopedSymbol::getScopeName() const { return Symbol::getName(); }

MethodSymbol::MethodSymbol(const std::string &name, Type *type,
                           Scope *enclosingScope, llvm::Function *function)
    : ScopedSymbol(name, type, enclosingScope, function) {}

MethodSymbol *getCurrentMethod(Scope *scope) {
  while (scope != nullptr) {
    auto p = dynamic_cast<MethodSymbol *>(scope);
    if (p) {
      return p;
    }
    scope = scope->getEnclosingScope();
  }
  return nullptr;
}

std::string LocalScope::getScopeName() const { return RAL_SCOPE_LOCAL; }

GlobalScope::GlobalScope() : BaseScope(nullptr) {}

std::string GlobalScope::getScopeName() const { return RAL_SCOPE_GLOBAL; }

StructSymbol::StructSymbol(const std::string &name, Scope *enclosingScope)
    : ScopedSymbol(name, enclosingScope) {}

Symbol *StructSymbol::resolveMember(const std::string &name) {
  auto it = getSymbols().find(name);
  return (it != getSymbols().end()) ? it->second.get() : nullptr;
}

} // namespace RaLang
