#pragma once

#include <memory>
#include <vector>

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>

namespace RaLang {

class AlgSymbol;
class Symbol;

class DebugInfoBase {
public:
  virtual ~DebugInfoBase() = default;
  virtual void emitLocation(int line) = 0;
  virtual void finalize() = 0;
  virtual llvm::DISubprogram *createAlgScope(int line, AlgSymbol *algSymbol) = 0;
  virtual llvm::DILexicalBlock *createLocalScope(int line) = 0;
  virtual void defineAlgParam(llvm::DISubprogram *debugAlgSymbol, Symbol *symbol, llvm::AllocaInst *attrAlloca,
                              int index) = 0;
  virtual void defineLocalVariable(Symbol *symbol, llvm::AllocaInst *attrAlloca, int line) = 0;

  virtual void popScope() = 0;
};

class DebugInfo : public DebugInfoBase {
public:
  DebugInfo(const std::string &path, llvm::Module &module, llvm::IRBuilder<> &builder);
  ~DebugInfo();

  llvm::DISubprogram *createAlgScope(int line, AlgSymbol *algSymbol) override;
  llvm::DILexicalBlock *createLocalScope(int line) override;
  void defineAlgParam(llvm::DISubprogram *debugAlgSymbol, Symbol *symbol, llvm::AllocaInst *attrAlloca,
                      int index) override;
  void defineLocalVariable(Symbol *symbol, llvm::AllocaInst *attrAlloca, int line) override;
  void emitLocation(int line) override;
  void finalize() override;
  void popScope() override;

private:
  llvm::DISubroutineType *createFunctionType(AlgSymbol *algSymbol);

private:
  llvm::IRBuilder<> &m_builder;
  llvm::DIBuilder m_debugBuilder;
  llvm::DICompileUnit *m_unit;
  std::vector<llvm::DIScope *> m_lexicalBlocks;
};

class DebugInfoDummy : public DebugInfoBase {
public:
  llvm::DISubprogram *createAlgScope(int line, AlgSymbol *algSymbol) override { return nullptr; }
  llvm::DILexicalBlock *createLocalScope(int line) override { return nullptr; }
  void defineAlgParam(llvm::DISubprogram *debugAlgSymbol, Symbol *symbol, llvm::AllocaInst *attrAlloca,
                      int index) override {}
  void defineLocalVariable(Symbol *symbol, llvm::AllocaInst *attrAlloca, int line) override {}
  void emitLocation(int line) override {}
  void finalize() override {}
  void popScope() override {}
};

} // namespace RaLang
