#include "irdeclarationgenerator.h"

#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"

namespace RaLang {

IrDeclarationGenerator::IrDeclarationGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable,
                                               llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder,
                                               llvm::Module &module)
    : m_emitDebugInfo(emitDebugInfo), m_path(path), m_symbolTable(symbolTable), m_llvmContext(llvmContext),
      m_builder(builder), m_module(module) {}

void IrDeclarationGenerator::visit(AstAlgorithm *algorithm) {
  initAlgorithm({algorithm->getName(), ""}, algorithm->getLine());
}

void IrDeclarationGenerator::initStandardAlgorithms() {
  initPrint();
  initInput();

  for (const auto &algName : m_symbolTable.getStandardAlgorithms()) {
    initAlgorithm(algName, 0);
  }
}

void IrDeclarationGenerator::initPrint() {
  AlgSymbol *algSymbol = resolveAlgorithm(m_symbolTable.getGlobals(), RAL_PRINT_CALL, 0);
  llvm::FunctionType *printfType =
      llvm::FunctionType::get(llvm::Type::getVoidTy(m_llvmContext), {llvm::Type::getInt8PtrTy(m_llvmContext)}, true);
  auto func = m_module.getOrInsertFunction("printf", printfType,
                                           llvm::AttributeList().get(m_llvmContext, 1U, llvm::Attribute::NoAlias));

  algSymbol->setValue(llvm::cast<llvm::Function>(func.getCallee()));
}

void IrDeclarationGenerator::initInput() {
  AlgSymbol *algSymbol = resolveAlgorithm(m_symbolTable.getGlobals(), RAL_INPUT_CALL, 0);
  llvm::FunctionType *scanfType =
      llvm::FunctionType::get(llvm::Type::getVoidTy(m_llvmContext), {llvm::Type::getInt8PtrTy(m_llvmContext)}, true);
  auto func = m_module.getOrInsertFunction("scanf", scanfType,
                                           llvm::AttributeList().get(m_llvmContext, 1U, llvm::Attribute::NoAlias));

  algSymbol->setValue(llvm::cast<llvm::Function>(func.getCallee()));
}

void IrDeclarationGenerator::initAlgorithm(const std::pair<std::string, std::string> &algName, int line) {
  AlgSymbol *algSymbol = resolveAlgorithm(m_symbolTable.getGlobals(), algName.first, line);

  std::vector<Symbol *> formalParameters = algSymbol->getFormalParameters();
  std::vector<llvm::Type *> functionArgs;
  for (auto symbol : formalParameters) {
    llvm::Type *t = symbol->getType()->createLlvmType(m_llvmContext);
    if (symbol->isReference()) {
      t = llvm::PointerType::getUnqual(t);
    }
    functionArgs.push_back(t);
  }

  llvm::Type *returnType = algSymbol->getType()->createLlvmType(m_llvmContext);
  llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, functionArgs, false);

  llvm::Function *f = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage,
                                             algName.second.size() ? algName.second : algName.first, m_module);

  algSymbol->setValue(f);
}

} // namespace RaLang
