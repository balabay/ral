#include "irgenerator.h"

#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"

#include <llvm/IR/Verifier.h>

namespace RaLang {

IrGenerator::IrGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable, llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder, llvm::Module &module)
    : m_emitDebugInfo(emitDebugInfo), m_path(path), m_symbolTable(symbolTable),
      m_llvmContext(llvmContext), m_builder(builder), m_module(module){
}

void IrGenerator::visit(Ast &ast)
{
    for(auto & module : ast.getNodes())
    {
        module->accept(this);
    }
}

llvm::Value *IrGenerator::visit(AstModule *module)
{
    for(auto & node : module->getNodes())
    {
        node->accept(this);
    }

    // Generate the entry point - int main() function
    llvm::FunctionType *functionType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(m_llvmContext), {}, false);
    llvm::Function *function = llvm::Function::Create(
        functionType, llvm::GlobalValue::LinkageTypes::ExternalLinkage, RAL_MAIN,
        m_module);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(m_builder.getContext());

    MethodSymbol *mainFunctionSymbol = m_symbolTable.createMethodSymbol(
        RAL_MAIN,
        dynamic_cast<Type *>(m_symbolTable.getGlobals()->resolve(RAL_INT)));
    mainFunctionSymbol->setValue(function);
    m_symbolTable.pushScope(mainFunctionSymbol);
    auto localScopeMainFunction =
        m_symbolTable.createLocalScope(mainFunctionSymbol);
    m_symbolTable.pushScope(localScopeMainFunction);
    block->insertInto(function);
    m_builder.SetInsertPoint(block);

    // Main function is 1st function without arguments;
    MethodSymbol *mainAlg = m_symbolTable.getGlobals()->getMainAlgorithm();

    auto mainAlgName = mainAlg->getName();
    auto calleeMainAlg = static_cast<llvm::Function *>(mainAlg->getValue());
    if (calleeMainAlg == nullptr) {
      throw VariableNotFoundException("No function " + mainAlgName);
    }

    // Don't try to pass a name in to CallInst::Create when trying to create a call that returns void.
    m_builder.CreateCall(calleeMainAlg, {});
    m_builder.CreateRet(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), 0, true));
    m_symbolTable.popScope(); // localScopeMainFunction
    m_symbolTable.popScope(); // mainFunctionSymbol
    return function;
}

static llvm::AllocaInst *createEntryBlockAlloca(llvm::LLVMContext *context,
                                                llvm::Function *function,
                                                Symbol *symbol) {
  llvm::IRBuilder<> builder(&function->getEntryBlock(),
                         function->getEntryBlock().begin());
  return builder.CreateAlloca(symbol->getType()->createLlvmType(*context), nullptr,
                           symbol->getName());
}

llvm::Value *IrGenerator::visit(AstAlgorithm *algorithm)
{
    std::string algName = algorithm->getName();

    auto algSymbol = dynamic_cast<MethodSymbol *>(
        m_symbolTable.getGlobals()->resolve(algName));

    m_symbolTable.pushScope(algSymbol);

    std::vector<Symbol*> formalParameters = algSymbol->getFormalParameters();

    auto *f = static_cast<llvm::Function*>(algSymbol->getValue());
    llvm::Type* returnType = f->getReturnType();


    // Create a new basic block to start insertion into.
    llvm::BasicBlock *functionBasicBlock =
        llvm::BasicBlock::Create(m_builder.getContext(), "entry", f);
    m_builder.SetInsertPoint(functionBasicBlock);

//    // Create a subprogram DIE for this function.
//    llvm::DIFile *Unit = debugBuilder->createFile(debugInfo.unit->getFilename(),
//                                                  debugInfo.unit->getDirectory());
//    llvm::DIScope *FContext = Unit;
//    unsigned LineNo = functionContext->getStart()->getLine();
//    unsigned ScopeLine = LineNo;
//    llvm::DISubprogram *SP = debugBuilder->createFunction(
//        FContext, functionName, llvm::StringRef(), Unit, LineNo,
//        createFunctionType(F->arg_size()), ScopeLine,
//        llvm::DINode::FlagPrototyped, llvm::DISubprogram::SPFlagDefinition);
//    F->setSubprogram(SP);

//    // Push the current scope.
//    LexicalBlocks.push_back(SP);

//    // Unset the location for the prologue emission (leading instructions with no
//    // location in a function are considered part of the prologue and the debugger
//    // will run past them when breaking on a function)
//    emitLocation(nullptr, debugInfo.unit);

    // Set names for all arguments.
    unsigned index = 0;
    for (llvm::Function::arg_iterator attrIndex = f->arg_begin();
         index != formalParameters.size(); ++attrIndex, ++index) {
        Symbol *symbol = formalParameters[index];
      std::string name = symbol->getName();
      attrIndex->setName(name);
      // Add arguments to variable symbol table.
      llvm::AllocaInst *attrAlloca = createEntryBlockAlloca(&m_llvmContext, f, symbol);
      symbol->setValue(attrAlloca);
//      // Create a debug descriptor for the variable.
//      llvm::DILocalVariable *D = debugBuilder->createParameterVariable(
//          SP, name, index, Unit, LineNo, getDebugType(), true);

//      debugBuilder->insertDeclare(
//          Alloca, D, debugBuilder->createExpression(),
//          llvm::DILocation::get(SP->getContext(), LineNo, 0, SP),
//          m_builder.GetInsertBlock());
      m_builder.CreateStore(attrIndex, attrAlloca);
    }

//    emitLocation(functionContext->instructions(), debugInfo.unit);

    auto localScopeFunction = m_symbolTable.createLocalScope(algSymbol);
    m_symbolTable.pushScope(localScopeFunction);
    // Create storage for return value
    VariableSymbol* returnValueSymbol = nullptr;
    llvm::AllocaInst *returnValueAttrAlloca = nullptr;
    if (!returnType->isVoidTy())
    {
        returnValueSymbol = m_symbolTable.createVariableSymbol(RAL_RET_VALUE, algSymbol->getType());
        localScopeFunction->define(std::unique_ptr<Symbol>(returnValueSymbol));
        returnValueAttrAlloca = createEntryBlockAlloca(&m_llvmContext, f, returnValueSymbol);
        returnValueSymbol->setValue(returnValueAttrAlloca);
    }

    for (auto node: algorithm->getNodes())
    {
        // ? is it a memory leak
        llvm::Value* returnStatementFlag = node->accept(this);
        if (returnStatementFlag)
        {
            break;
        }
    }
    if (returnValueSymbol)
    {
        llvm::LoadInst* returnRegister = m_builder.CreateLoad(returnValueAttrAlloca->getAllocatedType(), returnValueAttrAlloca, false);
        m_builder.CreateRet(returnRegister);
    }
    else
    {
        m_builder.CreateRetVoid();
    }

    m_symbolTable.popScope(); // local
//    // Pop off the lexical block for the function.
//    LexicalBlocks.pop_back();
    llvm::verifyFunction(*f);
    m_symbolTable.popScope(); // algorithm
    return f;
}

llvm::Value *IrGenerator::visit(AstAlgorithmCallExpression *algorithmCall)
{
    //emitLocation(context, debugInfo.unit);
    auto name = algorithmCall->getName();
    auto *calleeSymbol = dynamic_cast<MethodSymbol*>(m_symbolTable.getCurrentScope()->resolve(name));
    assert(calleeSymbol);
    auto f = static_cast<llvm::Function *>(calleeSymbol->getValue());

    auto actualArgs = algorithmCall->getNodes();
    assert(f->arg_size() == actualArgs.size());

    std::vector<llvm::Value *> argValues;
    for (unsigned i = 0; i != actualArgs.size(); i++) {
      auto expr = dynamic_cast<AstExpression*>(actualArgs[i].get());
      assert(expr);
      llvm::Value *exprValue = expr->accept(this);
      argValues.push_back(exprValue);
      if (argValues.back() == 0) {
        throw VariableNotFoundException("Incorrect argument " + std::to_string(i) + ". Line " + std::to_string(algorithmCall->getLine()));
      }
    }
    llvm::Value *result = (f->getReturnType()->isVoidTy()) ? m_builder.CreateCall(f, argValues) : m_builder.CreateCall(f, argValues, name);
    return result;
}

llvm::Value *IrGenerator::visit(AstExpressionStatement *expressionStatement)
{
    const auto & nodes = expressionStatement->getNodes();
    assert(nodes.size()==1);
    nodes[0]->accept(this);
    return {};
}

llvm::Value *IrGenerator::visit(AstIntExpression *expression)
{
    std::string valueString = expression->getValue();
    int value = std::stoi(valueString);
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), value, true);
}

llvm::Value *IrGenerator::visit(AstReturnStatement *returnStatement)
{
    // AstReturnStatement returns something (not used)
    // All other statements does not return value
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), 1, true);
}

} // namespace RaLang
