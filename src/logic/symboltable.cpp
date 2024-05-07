#include "symboltable.h"

#include "ralconsts.h"
#include "ralexceptions.h"
#include "typeconvertions.h"

#include <llvm/IR/Function.h>

namespace RaLang {

llvm::Value *Symbol::getValue() const { return m_value; }

Type *Symbol::getType() const { return m_type; }

void Symbol::setValue(llvm::Value *newValue) { m_value = newValue; }

Symbol::Symbol(const std::string &name, Type *type) : m_name(name), m_type(type) {}

std::string Symbol::getName() const { return m_name; }

SymbolTable::SymbolTable() : m_globals(new GlobalScope()) {
  m_scopes.push_back(std::move(std::unique_ptr<Scope>(m_globals)));
  pushScope(m_globals);
  initTypeSystem();
  initStandardFunctions();
}

MethodSymbol *SymbolTable::createMethodSymbol(const std::string &name, Type *type) {
  MethodSymbol *m = new MethodSymbol(name, type, m_globals);
  return m;
}

VariableSymbol *SymbolTable::createVariableSymbol(const std::string &name, Type *type) {
  VariableSymbol *v = new VariableSymbol(name, type);
  return v;
}

LocalScope *SymbolTable::createLocalScope(Scope *enclosingScope) {
  LocalScope *scope = new LocalScope(enclosingScope);
  m_scopes.emplace_back(scope);
  return scope;
}

GlobalScope *SymbolTable::getGlobals() const { return m_globals; }

void SymbolTable::popScope() {
  if (m_scopeStack.size()) {
    m_scopeStack.pop();
  } else {
    throw ScopeException("No current scope to pop.");
  }
}

Scope *SymbolTable::getCurrentScope() {
  if (m_scopeStack.size()) {
    return m_scopeStack.top();
  } else {
    throw ScopeException("No current scope to get.");
  }
}

void SymbolTable::pushScope(Scope *scope) {
  assert(scope);
  if (m_scopeStack.size()) {
    if (scope->getEnclosingScope() != m_scopeStack.top()) {
      throw ScopeException("Wrong parent scope " + m_scopeStack.top()->getScopeName() + " for scope " +
                           scope->getScopeName());
    }
  }
  m_scopeStack.push(scope);
}

void SymbolTable::removeSubScopes() {
  while (!m_scopeStack.empty()) {
    m_scopeStack.pop();
  }
  pushScope(m_globals);

  // Functions are not a member of Scope, they are member of nodes of m_globals
  auto itEnd =
      std::remove_if(m_scopes.begin(), m_scopes.end(),
                     [m_globals = this->m_globals](std::unique_ptr<Scope> &scope) { return scope.get() != m_globals; });
  m_scopes.erase(itEnd, m_scopes.end());
}

std::string SymbolTable::dump() {
  std::string result;
  for (auto &scope : m_scopes) {
    result += scope->dump(0);
  }
  return result;
}

std::string SymbolTable::dumpScope() {
  std::string result;
  Scope *scope = getCurrentScope();
  while (scope) {
    result += scope->dump(0);
    scope = scope->getEnclosingScope();
  }
  return result;
}

void SymbolTable::initTypeSystem() {

  m_globals->define(std::unique_ptr<Symbol>(new BuiltInTypeSymbol(RAL_INT, nullptr)));
  m_globals->define(std::unique_ptr<Symbol>(new BuiltInTypeSymbol(RAL_FLOAT, nullptr)));
  m_globals->define(std::unique_ptr<Symbol>(new BuiltInTypeSymbol(RAL_VOID, nullptr)));
}

void SymbolTable::initStandardFunctions() {
  initPrint();
  initInput();
}

void SymbolTable::initPrint() {
  Type *voidType = resolveType(getGlobals(), "");
  MethodSymbol *printFunctionSymbol = createMethodSymbol(RAL_PRINT_CALL, voidType);
  m_globals->define(std::unique_ptr<Symbol>(printFunctionSymbol));
}

void SymbolTable::initInput() {
  Type *voidType = resolveType(getGlobals(), "");
  MethodSymbol *inputFunctionSymbol = createMethodSymbol(RAL_INPUT_CALL, voidType);
  m_globals->define(std::unique_ptr<Symbol>(inputFunctionSymbol));
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
  m_symbols.insert(std::make_pair(sym->getName(), std::move(sym)));
}

Scope *BaseScope::getEnclosingScope() const { return m_enclosingScope; }

static std::string dumpSymbol(Symbol *s) {
  std::string result;
  result += s->getName() + " ";
  if (Type *type = s->getType()) {
    auto typeSymbol = dynamic_cast<Symbol *>(type);
    if (typeSymbol) {
      result += typeSymbol->getName();
    }
  }
  return result;
}

std::string BaseScope::dump(unsigned int level) {
  std::string intend(level, '\t');
  std::string result = intend;
  result += "Scope:" + getScopeName();
  Scope *parent = getEnclosingScope();
  if (parent) {
    result += "(" + parent->getScopeName() + ")";
  }
  result += '\n';
  intend += '\t';
  for (auto &symbol : m_symbols) {
    result += intend;
    result += dumpSymbol(symbol.second.get());
    result += '\n';
    if (auto symbolAsScope = dynamic_cast<Scope *>(symbol.second.get())) {
      result += symbolAsScope->dump(level + 2);
    }
  }
  return result;
}

ScopedSymbol::ScopedSymbol(const std::string &name, Type *type, Scope *enclosingScope)
    : Symbol(name, type), BaseScope(enclosingScope) {}

Symbol *ScopedSymbol::resolveType(const std::string &name) { return resolve(name); }

std::string ScopedSymbol::getScopeName() const { return Symbol::getName(); }

MethodSymbol::MethodSymbol(const std::string &name, Type *type, Scope *enclosingScope)
    : ScopedSymbol(name, type, enclosingScope) {}

void MethodSymbol::define(std::unique_ptr<Symbol> sym) {
  m_formalParameters.push_back(sym.get());
  ScopedSymbol::define(std::move(sym));
}

std::vector<Symbol *> MethodSymbol::getFormalParameters() { return m_formalParameters; }

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

MethodSymbol *GlobalScope::getMainAlgorithm() {
  for (auto &it : getSymbols()) {
    auto method = dynamic_cast<MethodSymbol *>(it.second.get());
    if (method) {
      if (method->getFormalParameters().size() == 0) {
        return method;
      }
    }
  }
  return nullptr;
}

StructSymbol::StructSymbol(const std::string &name, Scope *enclosingScope)
    : ScopedSymbol(name, nullptr, enclosingScope) {}

Symbol *StructSymbol::resolveMember(const std::string &name) {
  auto it = getSymbols().find(name);
  return (it != getSymbols().end()) ? it->second.get() : nullptr;
}

Type *resolveType(Scope *scope, const std::string &name) {
  std::string internalType = fromSourceTypeName(name);
  Symbol *resolvedSymbol = scope->resolve(internalType);
  auto resolvedType = dynamic_cast<Type *>(resolvedSymbol);
  if (resolvedType == nullptr) {
    throw VariableNotFoundException("unknown type " + name);
  }
  return resolvedType;
}

llvm::Type *BuiltInTypeSymbol::createLlvmType(llvm::LLVMContext &c) {
  llvm::Type *result = nullptr;
  std::string name = getName();
  if (name == RAL_INT) {
    result = llvm::Type::getInt32Ty(c);
  } else if (name == RAL_FLOAT) {
    result = llvm::Type::getDoubleTy(c);
  } else if (name == RAL_VOID) {
    result = llvm::Type::getVoidTy(c);
  }
  if (result == nullptr) {
    throw VariableNotFoundException("unknown type name " + name);
  }
  return result;
}

} // namespace RaLang
