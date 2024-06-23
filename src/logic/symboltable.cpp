#include "symboltable.h"

#include "ralconsts.h"
#include "ralexceptions.h"
#include "typeconvertions.h"
#include "utils.h"

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Function.h>

namespace RaLang {

llvm::Value *Symbol::getValue() const { return m_value; }

Type *Symbol::getType() const { return m_type; }

void Symbol::setValue(llvm::Value *newValue) { m_value = newValue; }

bool Symbol::isReference() const { return m_reference; }

void Symbol::setReference(bool newReference) { m_reference = newReference; }

Symbol::Symbol(const std::string &name, Type *type) : m_name(name), m_type(type), m_reference(false) {}

std::string Symbol::getName() const { return m_name; }

std::vector<std::pair<std::string, std::string>> SymbolTable::getStandardAlgorithms() const {
  return m_standardAlgorithms;
}

SymbolTable::SymbolTable() : m_globals(new GlobalScope()) {
  m_scopes.push_back(std::move(std::unique_ptr<Scope>(m_globals)));
  pushScope(m_globals);
  initTypeSystem();
  initStandardFunctions();
}

AlgSymbol *SymbolTable::createAlgSymbol(const std::string &name, Type *type) {
  AlgSymbol *m = new AlgSymbol(name, type, m_globals);
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

  m_globals->define(std::unique_ptr<Symbol>(new BuiltInTypeSymbol(RAL_INT, TypeKind::Int)));
  m_globals->define(std::unique_ptr<Symbol>(new BuiltInTypeSymbol(RAL_REAL, TypeKind::Real)));
  m_globals->define(std::unique_ptr<Symbol>(new BuiltInTypeSymbol(RAL_VOID, TypeKind::Void)));
}

void SymbolTable::initStandardFunctions() {
  initPrint();
  initInput();

  registerAlgorithm({"div", "ral_div"}, RAL_INT, {{"numerator", RAL_INT}, {"denumerator", RAL_INT}});
  registerAlgorithm({"mod", ""}, RAL_INT, {{"numerator", RAL_INT}, {"denumerator", RAL_INT}});
  registerAlgorithm({"int", "ral_int"}, RAL_INT, {{"value", RAL_REAL}});
  registerAlgorithm({"pow", ""}, RAL_REAL, {{"x", RAL_REAL}, {"y", RAL_REAL}});
}

void SymbolTable::initPrint() {
  Type *voidType = resolveType(getGlobals(), "");
  AlgSymbol *printFunctionSymbol = createAlgSymbol(RAL_PRINT_CALL, voidType);
  m_globals->define(std::unique_ptr<Symbol>(printFunctionSymbol));
}

void SymbolTable::initInput() {
  Type *voidType = resolveType(getGlobals(), "");
  AlgSymbol *inputFunctionSymbol = createAlgSymbol(RAL_INPUT_CALL, voidType);
  m_globals->define(std::unique_ptr<Symbol>(inputFunctionSymbol));
}

void SymbolTable::registerAlgorithm(const std::pair<std::string, std::string> &name, const std::string &returnTypeName,
                                    const std::vector<std::pair<std::string, std::string>> &formalParameters) {
  Type *returnType = resolveType(getGlobals(), returnTypeName);
  assert(returnType);
  AlgSymbol *symbol = createAlgSymbol(name.first, returnType);
  assert(symbol);
  m_globals->define(std::unique_ptr<Symbol>(symbol));
  for (const auto &p : formalParameters) {
    Type *parameterType = resolveType(symbol, p.second);
    VariableSymbol *parameter = createVariableSymbol(p.first, parameterType);
    symbol->define(std::unique_ptr<Symbol>(parameter));
  }
  m_standardAlgorithms.push_back(name);
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
  if (s->isReference())
    result += "*";
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

AlgSymbol::AlgSymbol(const std::string &name, Type *type, Scope *enclosingScope)
    : ScopedSymbol(name, type, enclosingScope) {}

void AlgSymbol::define(std::unique_ptr<Symbol> sym) {
  m_formalParameters.push_back(sym.get());
  ScopedSymbol::define(std::move(sym));
}

std::vector<Symbol *> AlgSymbol::getFormalParameters() { return m_formalParameters; }

llvm::Function *AlgSymbol::getFunction() {
  assert(getValue());
  return static_cast<llvm::Function *>(getValue());
}

AlgSymbol *getCurrentAlg(Scope *scope) {
  while (scope != nullptr) {
    auto p = dynamic_cast<AlgSymbol *>(scope);
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
    : ScopedSymbol(name, nullptr, enclosingScope) {}

Symbol *StructSymbol::resolveMember(const std::string &name) {
  auto it = getSymbols().find(name);
  return (it != getSymbols().end()) ? it->second.get() : nullptr;
}

Type *resolveType(Scope *scope, const std::string &name) {
  auto resolvedType = resolveTypeNoException(scope, name);
  if (resolvedType == nullptr) {
    throw VariableNotFoundException("unknown type " + name);
  }
  return resolvedType;
}

Type *resolveTypeNoException(Scope *scope, const std::string &name) {
  std::string internalType = fromSourceTypeName(name);
  Symbol *resolvedSymbol = scope->resolve(internalType);
  auto resolvedType = dynamic_cast<Type *>(resolvedSymbol);
  return resolvedType;
}

BuiltInTypeSymbol::BuiltInTypeSymbol(const std::string &name, TypeKind typeKind)
    : Symbol(name, nullptr), m_typeKind(typeKind) {}

llvm::Type *BuiltInTypeSymbol::createLlvmType(llvm::LLVMContext &c) {
  switch (m_typeKind) {
  case TypeKind::Int:
    return llvm::Type::getInt32Ty(c);
  case TypeKind::Real:
    return llvm::Type::getDoubleTy(c);
  case TypeKind::Void:
    return llvm::Type::getVoidTy(c);
  default:
    throw VariableNotFoundException("unknown type name " + getName() + " (" + typeKindToString(m_typeKind) + ")");
  }
}

llvm::DIType *BuiltInTypeSymbol::createLlvmDIType(llvm::DIBuilder &debugBuilder) {
  // TODO: handle void correctly
  switch (m_typeKind) {
  case TypeKind::Int:
    return debugBuilder.createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);
  case TypeKind::Real:
    return debugBuilder.createBasicType("real", 64, llvm::dwarf::DW_ATE_float);
  case TypeKind::Void:
    return debugBuilder.createBasicType("void", 32, llvm::dwarf::DW_ATE_signed);
  default:
    throw VariableNotFoundException("unknown debug type name " + getName() + " (" + typeKindToString(m_typeKind) + ")");
  }
}

TypeKind BuiltInTypeSymbol::getTypeKind() const { return m_typeKind; }

AlgSymbol *resolveAlgorithm(Scope *scope, const std::string &name, int line) {
  assert(scope);
  Symbol *resolvedSymbol = scope->resolve(name);
  auto algSymbol = dynamic_cast<AlgSymbol *>(resolvedSymbol);
  if (algSymbol == nullptr) {
    throw VariableNotFoundException("Not an algorithm: '" + name + "' at line " + std::to_string(line));
  }
  return algSymbol;
}

VariableSymbol *resolveVariable(Scope *scope, const std::string &name, int line) {
  assert(scope);
  Symbol *resolvedSymbol = scope->resolve(name);
  auto variableSymbol = dynamic_cast<VariableSymbol *>(resolvedSymbol);
  if (variableSymbol == nullptr) {
    throw VariableNotFoundException("Not a variable: '" + name + "' at line " + std::to_string(line));
  }
  return variableSymbol;
}

} // namespace RaLang
