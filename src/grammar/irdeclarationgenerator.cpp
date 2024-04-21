#include "irdeclarationgenerator.h"

#include "logic/symboltable.h"
#include "ralexceptions.h"

namespace RaLang {

IrDeclarationGenerator::IrDeclarationGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable, llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder, llvm::Module &module)
    : m_emitDebugInfo(emitDebugInfo), m_path(path), m_symbolTable(symbolTable),
      m_llvmContext(llvmContext), m_builder(builder), m_module(module){
}

llvm::Value *IrDeclarationGenerator::visit(AstAlgorithm *algorithm)
{
    std::string algName = algorithm->getName();

    auto algSymbol = dynamic_cast<MethodSymbol *>(
        m_symbolTable.getGlobals()->resolve(algName));
    if (algSymbol == nullptr) {
      throw VariableNotFoundException("No function " + algName);
    }

    std::vector<Symbol*> formalParameters = algSymbol->getFormalParameters();
    std::vector<llvm::Type *> functionArgs;
    for(auto symbol: formalParameters)
    {
        functionArgs.push_back(symbol->getType()->createLlvmType(m_llvmContext));
    }

    llvm::Type* returnType = algSymbol->getType()->createLlvmType(m_llvmContext);
    llvm::FunctionType *functionType = llvm::FunctionType::get(
        returnType, functionArgs, false);

    llvm::Function *f = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, algName, m_module);

    algSymbol->setValue(f);
    return f;
}

} // namespace RaLang
