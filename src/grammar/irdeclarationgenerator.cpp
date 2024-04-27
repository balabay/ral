#include "irdeclarationgenerator.h"

#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"

namespace RaLang {

IrDeclarationGenerator::IrDeclarationGenerator(bool emitDebugInfo,
                                               const std::string &path,
                                               SymbolTable &symbolTable,
                                               llvm::LLVMContext &llvmContext,
                                               llvm::IRBuilder<> &builder,
                                               llvm::Module &module)
    : m_emitDebugInfo(emitDebugInfo), m_path(path), m_symbolTable(symbolTable),
      m_llvmContext(llvmContext), m_builder(builder), m_module(module) {}

llvm::Value *IrDeclarationGenerator::visit(AstAlgorithm *algorithm) {
  std::string algName = algorithm->getName();

  auto algSymbol = dynamic_cast<MethodSymbol *>(
      m_symbolTable.getGlobals()->resolve(algName));
  if (algSymbol == nullptr) {
    throw VariableNotFoundException("No function " + algName);
  }

  std::vector<Symbol *> formalParameters = algSymbol->getFormalParameters();
  std::vector<llvm::Type *> functionArgs;
  for (auto symbol : formalParameters) {
    functionArgs.push_back(symbol->getType()->createLlvmType(m_llvmContext));
  }

  llvm::Type *returnType = algSymbol->getType()->createLlvmType(m_llvmContext);
  llvm::FunctionType *functionType =
      llvm::FunctionType::get(returnType, functionArgs, false);

  llvm::Function *f = llvm::Function::Create(
      functionType, llvm::Function::ExternalLinkage, algName, m_module);

  algSymbol->setValue(f);
  return f;
}

void IrDeclarationGenerator::initStandardFunctions() {
  initPrint();
  initInput();
}

void IrDeclarationGenerator::initPrint() {
  auto algSymbol = dynamic_cast<MethodSymbol *>(
      m_symbolTable.getGlobals()->resolve(RAL_PRINT_CALL));
  if (algSymbol == nullptr) {
    throw VariableNotFoundException(RAL_PRINT_CALL);
  }
  auto printfType =
      llvm::FunctionType::get(llvm::Type::getVoidTy(m_llvmContext),
                              {llvm::Type::getInt8PtrTy(m_llvmContext)}, true);
  auto func = m_module.getOrInsertFunction(
      "printf", printfType,
      llvm::AttributeList().get(m_llvmContext, 1U, llvm::Attribute::NoAlias));

  algSymbol->setValue(llvm::cast<llvm::Function>(func.getCallee()));
}

void IrDeclarationGenerator::initInput() {
  auto algSymbol = dynamic_cast<MethodSymbol *>(
      m_symbolTable.getGlobals()->resolve(RAL_INPUT_CALL));
  if (algSymbol == nullptr) {
    throw VariableNotFoundException(RAL_INPUT_CALL);
  }
  auto scanfType =
      llvm::FunctionType::get(llvm::Type::getVoidTy(m_llvmContext),
                              {llvm::Type::getInt8PtrTy(m_llvmContext)}, true);
  auto func = m_module.getOrInsertFunction(
      "scanf", scanfType,
      llvm::AttributeList().get(m_llvmContext, 1U, llvm::Attribute::NoAlias));

  algSymbol->setValue(llvm::cast<llvm::Function>(func.getCallee()));
}

} // namespace RaLang
