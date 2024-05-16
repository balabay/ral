#include "debuginfo.h"

#include "logic/symboltable.h"

namespace RaLang {

DebugInfo::DebugInfo(const std::string &path, llvm::Module &module, llvm::IRBuilder<> &builder)
    : m_builder(builder), m_debugBuilder(module),
      m_unit(m_debugBuilder.createCompileUnit(llvm::dwarf::DW_LANG_C, m_debugBuilder.createFile(path, ""),
                                              "RAL Compiler", 1, "", 0)) {
  module.addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
}

void DebugInfo::finalize() { m_debugBuilder.finalize(); }

void DebugInfo::popScope() { m_lexicalBlocks.pop_back(); }

llvm::DISubroutineType *DebugInfo::createFunctionType(AlgSymbol *algSymbol) {
  assert(algSymbol);
  llvm::SmallVector<llvm::Metadata *> elementTypes;
  std::vector<Symbol *> parameters = algSymbol->getFormalParameters();
  Type *algResult = algSymbol->getType();

  // Add the result type.
  elementTypes.push_back(algResult->createLlvmDIType(m_debugBuilder));

  for (unsigned i = 0; i != parameters.size(); i++) {
    elementTypes.push_back(parameters[i]->getType()->createLlvmDIType(m_debugBuilder));
  }

  return m_debugBuilder.createSubroutineType(m_debugBuilder.getOrCreateTypeArray(elementTypes));
}

RaLang::DebugInfo::~DebugInfo() {}

llvm::DISubprogram *DebugInfo::createAlgScope(int line, AlgSymbol *algSymbol) {
  // Create a subprogram DIE for this function.
  llvm::DIFile *diFile = m_debugBuilder.createFile(m_unit->getFilename(), m_unit->getDirectory());
  llvm::DIScope *diFunctionScope = diFile;
  llvm::DISubprogram *debugFunction = m_debugBuilder.createFunction(
      diFunctionScope, algSymbol->getName(), llvm::StringRef(), diFile, line, createFunctionType(algSymbol), line,
      llvm::DINode::FlagPrototyped, llvm::DISubprogram::SPFlagDefinition);
  llvm::Function *f = algSymbol->getFunction();
  f->setSubprogram(debugFunction);

  // Push the current scope.
  m_lexicalBlocks.push_back(debugFunction);

  // Unset the location for the prologue emission (leading instructions with no
  // location in a function are considered part of the prologue and the debugger
  // will run past them when breaking on a function)
  m_builder.SetCurrentDebugLocation(nullptr);
  return debugFunction;
}

llvm::DILexicalBlock *DebugInfo::createLocalScope(int line) {
  assert(m_lexicalBlocks.size());
  llvm::DIScope *currentScope = m_lexicalBlocks.back();
  assert(currentScope);
  llvm::DIFile *diFile = currentScope->getFile();
  assert(diFile);
  llvm::DILexicalBlock *localScope = m_debugBuilder.createLexicalBlock(currentScope, diFile, line, 0);

  // Push the current scope.
  m_lexicalBlocks.push_back(localScope);
  return localScope;
}

void DebugInfo::defineAlgParam(llvm::DISubprogram *debugAlgSymbol, Symbol *symbol, llvm::AllocaInst *attrAlloca,
                               int index) {
  // Create a debug descriptor for the algorithm parameter.
  llvm::DIType *diType = symbol->getType()->createLlvmDIType(m_debugBuilder);
  llvm::DIScope *diFunctionScope = debugAlgSymbol->getScope();
  int line = debugAlgSymbol->getLine();
  auto diFile = static_cast<llvm::DIFile *>(diFunctionScope);
  llvm::DILocalVariable *d =
      m_debugBuilder.createParameterVariable(debugAlgSymbol, symbol->getName(), index, diFile, line, diType, true);

  llvm::DILocation *diLocation = llvm::DILocation::get(debugAlgSymbol->getContext(), line, 0, debugAlgSymbol);
  m_debugBuilder.insertDeclare(attrAlloca, d, m_debugBuilder.createExpression(), diLocation,
                               m_builder.GetInsertBlock());
}

void DebugInfo::defineLocalVariable(Symbol *symbol, llvm::AllocaInst *attrAlloca, int line) {
  // Create a debug descriptor for the local variable.
  llvm::DIType *diType = symbol->getType()->createLlvmDIType(m_debugBuilder);
  assert(m_lexicalBlocks.size());
  llvm::DIScope *diScope = m_lexicalBlocks.back();
  auto diFile = diScope->getFile();
  llvm::DILocalVariable *d = m_debugBuilder.createAutoVariable(diScope, symbol->getName(), diFile, line, diType, true);

  llvm::DILocation *diLocation = llvm::DILocation::get(m_builder.getContext(), line, 0, diScope);
  m_debugBuilder.insertDeclare(attrAlloca, d, m_debugBuilder.createExpression(), diLocation,
                               m_builder.GetInsertBlock());
}

void DebugInfo::emitLocation(int line) {
  llvm::DIScope *scope = m_lexicalBlocks.empty() ? m_unit : m_lexicalBlocks.back();
  m_builder.SetCurrentDebugLocation(llvm::DILocation::get(scope->getContext(), line, 0, scope));
}

} // namespace RaLang
