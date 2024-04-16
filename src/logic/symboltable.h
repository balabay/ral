#pragma once

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace llvm {
class Function;
class Value;
} // namespace llvm

/** The implementation of the Symbol Table is inspired by the Java code
 *  from the "Language Implementation Patterns" book by
 *  Visit http://www.pragmaticprogrammer.com/titles/tpdsl for more book
 * information.
 */

namespace RaLang {

/** A "tag" to indicate which symbols are types */
// TODO: Should be adopted to real type handling.
// Current implementation assumes INT32 everywhere,
// so the Type is not very useful.
class Type {
public:
  virtual ~Type() = default;
};

class Scope;
class SymbolTable;

/** A generic programming language symbol */
class Symbol {
  std::string m_name; // All symbols at least have a name

  // Scope and SymbolTable are responcible for ownership,
  // that is why raw pointers here.
  Type *m_type;
  Scope *m_scope; // All symbols know what scope contains them.
  llvm::Value *m_value;

protected:
  Symbol(const std::string &name);
  Symbol(const std::string &name, Type *type, llvm::Value *value);

public:
  virtual ~Symbol() = default;
  std::string getName() const;
  Scope *getScope() const;
  void setScope(Scope *newScope);
  llvm::Value *getValue() const;
  Type *getType() const;
};

class Scope {
public:
  virtual ~Scope() = default;
  virtual std::string getScopeName() const = 0;

  /** Where to look next for symbols;  */
  virtual Scope *getEnclosingScope() const = 0;

  /** Define a symbol in the current scope.
   *  Takes ownership on symbol.
   */
  virtual void define(std::unique_ptr<Symbol> sym) = 0;

  /** Look up name in this scope or in enclosing scope if not here */
  virtual Symbol *resolve(const std::string &name) = 0;
  virtual std::string dump(unsigned level) = 0;
};

using SymbolMap = std::map<std::string, std::unique_ptr<Symbol>>;

class BaseScope : public Scope {
  Scope *m_enclosingScope; // null if global (outermost) scope
  SymbolMap m_symbols;     // Scope is an owner of it's symbols

protected:
  BaseScope(Scope *parent);
  SymbolMap &getSymbols();

public:
  Symbol *resolve(const std::string &name) override;
  void define(std::unique_ptr<Symbol> sym) override;
  Scope *getEnclosingScope() const override;
  std::string dump(unsigned level) override;
};

class LocalScope : public BaseScope {
  friend class SymbolTable;
  using BaseScope::BaseScope;

public:
  std::string getScopeName() const override;
};

class GlobalScope : public BaseScope {
  friend class SymbolTable;
  GlobalScope();

public:
  std::string getScopeName() const override;
};

class BuiltInTypeSymbol : public Symbol, public Type {
  friend class SymbolTable;
  using Symbol::Symbol;
};

/** Represents a variable definition (name,type) in symbol table */
class VariableSymbol : public Symbol {
  friend class SymbolTable;
  using Symbol::Symbol;
};

class ScopedSymbol : public Symbol, public BaseScope {
protected:
  ScopedSymbol(const std::string &name, Type *type, Scope *enclosingScope,
               llvm::Value *value);
  ScopedSymbol(const std::string &name, Scope *enclosingScope);

public:
  Symbol *resolveType(const std::string &name);
  std::string getScopeName() const override;
};

class MethodSymbol : public ScopedSymbol {
  friend class SymbolTable;
  MethodSymbol(const std::string &name, Type *type, Scope *enclosingScope,
               llvm::Function *function);
};

/** Find the function that contains the scope. */
MethodSymbol *getCurrentMethod(Scope *scope);
Type *resolveType(Scope *scope, const std::string &name);

class StructSymbol : public ScopedSymbol, public Type {
  friend class SymbolTable;
  StructSymbol(const std::string &name, Scope *enclosingScope);

public:
  /** For a.b, only look in fields to resolve b, not up scope tree */
  Symbol *resolveMember(const std::string &name);
};

class SymbolTable {
  GlobalScope *m_globals;
  // Symbol Table is an owner of Scopes
  std::vector<std::unique_ptr<Scope>> m_scopes;
  std::stack<Scope *> m_scopeStack;

public:
  SymbolTable();
  MethodSymbol *createMethodSymbol(const std::string &name, Type *type,
                                   llvm::Function *function);
  VariableSymbol *createVariableSymbol(const std::string &name, Type *type,
                                       llvm::Value *value);
  LocalScope *createLocalScope(Scope *enclosingScope);
  GlobalScope *getGlobals() const;
  void popScope();
  Scope *getCurrentScope();
  void pushScope(Scope *scope);
  std::string dump();

protected:
  void initTypeSystem();
};

} // namespace RaLang
