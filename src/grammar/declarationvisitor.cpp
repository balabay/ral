#include "declarationvisitor.h"

#include "exceptions/VariableNotFoundException.hpp"
#include "ralconsts.h"

namespace RaLang {

DeclarationVisitor::DeclarationVisitor(SymbolTable &symbolTable, llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder, llvm::Module &module)
    :m_symbolTable(symbolTable),
      m_llvmContext(llvmContext),
      m_builder(builder),
      m_module(module)
{

}

std::any DeclarationVisitor::visitFunction(RalParser::FunctionContext *ctx) {
    RalParser::FormalParametersContext *formalParameters = ctx->formalParameters();
    std::vector<antlr4::tree::TerminalNode *> variableNames;
    if (formalParameters) {
      variableNames = formalParameters->VariableName();
    }

    std::vector<llvm::Type *> functionArgs(
        variableNames.size(), llvm::Type::getInt32Ty(m_builder.getContext()));

    llvm::FunctionType *functionType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(m_builder.getContext()), functionArgs, false);

    // Get Name of the function
    std::string functionName = ctx->VariableName()->getText();
    llvm::Function *f = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, functionName, m_module);

    // If 'f' conflicted, there was already something named 'functionName'. If it has a
    // body, don't allow redefinition or reextern.
    if (f->getName() != functionName) {
      // Delete the one we just made and get the existing one.
      f->eraseFromParent();
      f = m_module.getFunction(functionName);
    }
    if (!f->empty()) {
      throw VariableNotFoundException("redefinition of function" + functionName);
    }

    MethodSymbol* functionSymbol = m_symbolTable.createMethodSymbol(
        functionName,
        dynamic_cast<Type *>(m_symbolTable.getGlobals()->resolve(RAL_INT)), f);
    m_symbolTable.getGlobals()->define(std::unique_ptr<Symbol>(functionSymbol));

    return visitChildren(ctx);
}

} // namespace RaLang
