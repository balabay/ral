#include "irgenerator.h"

#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"
#include "utils.h"

#include <llvm/IR/Verifier.h>

namespace RaLang {

IrGenerator::IrGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable,
                         llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder, llvm::Module &module)
    : m_emitDebugInfo(emitDebugInfo), m_path(path), m_symbolTable(symbolTable), m_llvmContext(llvmContext),
      m_builder(builder), m_module(module) {}

void IrGenerator::visit(Ast &ast) {
  for (auto &module : ast.getNodes()) {
    module->accept(this);
  }
}

llvm::Value *IrGenerator::visit(AstModule *module) {
  for (auto &node : module->getNodes()) {
    node->accept(this);
  }

  // Generate the entry point - int main() function
  llvm::FunctionType *functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(m_llvmContext), {}, false);
  llvm::Function *function =
      llvm::Function::Create(functionType, llvm::GlobalValue::LinkageTypes::ExternalLinkage, RAL_MAIN, m_module);
  llvm::BasicBlock *block = llvm::BasicBlock::Create(m_builder.getContext());

  MethodSymbol *mainFunctionSymbol =
      m_symbolTable.createMethodSymbol(RAL_MAIN, dynamic_cast<Type *>(m_symbolTable.getGlobals()->resolve(RAL_INT)));
  mainFunctionSymbol->setValue(function);
  m_symbolTable.pushScope(mainFunctionSymbol);
  auto localScopeMainFunction = m_symbolTable.createLocalScope(mainFunctionSymbol);
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

  // Don't try to pass a name in to CallInst::Create when trying to create a
  // call that returns void.
  m_builder.CreateCall(calleeMainAlg, {});
  m_builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), 0, true));
  m_symbolTable.popScope(); // localScopeMainFunction
  m_symbolTable.popScope(); // mainFunctionSymbol
  return function;
}

llvm::Value *IrGenerator::visit(AstPrintStatement *statement) {
  std::vector<std::string> formats;
  std::vector<llvm::Value *> args;

  for (auto &exprAstNode : statement->getNodes()) {
    llvm::Value *exprValue = exprAstNode->accept(this);
    llvm::Type *type = exprValue->getType();

    args.push_back(exprValue);

    if (type->isIntegerTy()) {
      formats.push_back("%d");
    } else {
      throw NotImplementedException();
    }
  }

  std::ostringstream format;
  std::copy(formats.begin(), formats.end(), std::ostream_iterator<std::string>(format, " "));
  std::string formatString = format.str() + '\n';
  auto global = m_builder.CreateGlobalStringPtr(formatString.c_str());
  args.insert(args.begin(), global);

  auto *printFunctionSymbol = dynamic_cast<MethodSymbol *>(m_symbolTable.getCurrentScope()->resolve(RAL_PRINT_CALL));
  assert(printFunctionSymbol);

  auto *printFunction = static_cast<llvm::Function *>(printFunctionSymbol->getValue());
  assert(printFunction);
  m_builder.CreateCall(printFunction, args);

  return {};
}

static llvm::AllocaInst *createEntryBlockAlloca(llvm::LLVMContext *context, llvm::Function *function, Symbol *symbol) {
  llvm::IRBuilder<> builder(&function->getEntryBlock(), function->getEntryBlock().begin());
  return builder.CreateAlloca(symbol->getType()->createLlvmType(*context), nullptr, symbol->getName());
}

llvm::Value *IrGenerator::visit(AstAlgorithm *algorithm) {
  std::string algName = algorithm->getName();

  auto algSymbol = dynamic_cast<MethodSymbol *>(m_symbolTable.getGlobals()->resolve(algName));

  m_symbolTable.pushScope(algSymbol);

  std::vector<Symbol *> formalParameters = algSymbol->getFormalParameters();

  auto *f = static_cast<llvm::Function *>(algSymbol->getValue());
  llvm::Type *returnType = f->getReturnType();

  // Create a new basic block to start insertion into.
  llvm::BasicBlock *functionBasicBlock = llvm::BasicBlock::Create(m_builder.getContext(), "entry", f);
  m_builder.SetInsertPoint(functionBasicBlock);

  //    // Create a subprogram DIE for this function.
  //    llvm::DIFile *Unit =
  //    debugBuilder->createFile(debugInfo.unit->getFilename(),
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

  //    // Unset the location for the prologue emission (leading instructions
  //    with no
  //    // location in a function are considered part of the prologue and the
  //    debugger
  //    // will run past them when breaking on a function)
  //    emitLocation(nullptr, debugInfo.unit);

  // Set names for all arguments.
  unsigned index = 0;
  for (llvm::Function::arg_iterator attrIndex = f->arg_begin(); index != formalParameters.size();
       ++attrIndex, ++index) {
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
  if (!returnType->isVoidTy()) {
    VariableSymbol *returnValueSymbol = m_symbolTable.createVariableSymbol(RAL_RET_VALUE, algSymbol->getType());
    localScopeFunction->define(std::unique_ptr<Symbol>(returnValueSymbol));
    llvm::AllocaInst *returnValueAttrAlloca = createEntryBlockAlloca(&m_llvmContext, f, returnValueSymbol);
    returnValueSymbol->setValue(returnValueAttrAlloca);
  }

  llvm::Value *returnStatementFlag = nullptr;
  for (auto node : algorithm->getNodes()) {
    // ? is it a memory leak
    returnStatementFlag = node->accept(this);
    if (returnStatementFlag) {
      break;
    }
  }

  if (!returnStatementFlag) {
    addReturnStatement();
  }

  m_symbolTable.popScope(); // local
                            //    // Pop off the lexical block for the function.
                            //    LexicalBlocks.pop_back();
  llvm::verifyFunction(*f);
  m_symbolTable.popScope(); // algorithm
  return f;
}

llvm::Value *IrGenerator::visit(AstAlgorithmCallExpression *algorithmCall) {
  // emitLocation(context, debugInfo.unit);
  auto name = algorithmCall->getName();
  auto *calleeSymbol = dynamic_cast<MethodSymbol *>(m_symbolTable.getCurrentScope()->resolve(name));
  assert(calleeSymbol);
  auto f = static_cast<llvm::Function *>(calleeSymbol->getValue());

  auto actualArgs = algorithmCall->getNodes();
  assert(f->arg_size() == actualArgs.size());

  std::vector<llvm::Value *> argValues;
  for (unsigned i = 0; i != actualArgs.size(); i++) {
    auto expr = dynamic_cast<AstExpression *>(actualArgs[i].get());
    assert(expr);
    llvm::Value *exprValue = expr->accept(this);
    argValues.push_back(exprValue);
    if (argValues.back() == 0) {
      throw VariableNotFoundException("Incorrect argument " + std::to_string(i) + ". Line " +
                                      std::to_string(algorithmCall->getLine()));
    }
  }
  llvm::Value *result =
      (f->getReturnType()->isVoidTy()) ? m_builder.CreateCall(f, argValues) : m_builder.CreateCall(f, argValues, name);
  return result;
}

llvm::Value *IrGenerator::visit(AstBinaryConditionalExpression *expression) {
  //    emitLocation(context, debugInfo.unit);
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftExpression = nodes[0]->accept(this);
  assert(leftExpression);
  auto rightExpression = nodes[1]->accept(this);
  assert(rightExpression);

  Token::Type t = expression->getTokenType();
  switch (t) {
  case Token::COND_EQ:
    return m_builder.CreateICmpEQ(leftExpression, rightExpression);
  case Token::COND_NE:
    return m_builder.CreateICmpNE(leftExpression, rightExpression);
  case Token::COND_GT:
    return m_builder.CreateICmpSGT(leftExpression, rightExpression);
  case Token::COND_GE:
    return m_builder.CreateICmpSGE(leftExpression, rightExpression);
  case Token::COND_LT:
    return m_builder.CreateICmpSLT(leftExpression, rightExpression);
  case Token::COND_LE:
    return m_builder.CreateICmpSLE(leftExpression, rightExpression);
  default:
    throw NotImplementedException();
  }
}

llvm::Value *IrGenerator::visit(AstMathExpression *expression) {
  //    emitLocation(context, debugInfo.unit);
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftExpression = nodes[0]->accept(this);
  assert(leftExpression);
  auto rightExpression = nodes[1]->accept(this);
  assert(rightExpression);

  Token::Type t = expression->getTokenType();
  switch (t) {
  case Token::MUL:
    return m_builder.CreateMul(leftExpression, rightExpression);
  case Token::DIV:
    return m_builder.CreateSDiv(leftExpression, rightExpression);
  case Token::MOD:
    return m_builder.CreateSRem(leftExpression, rightExpression);
  case Token::PLUS:
    return m_builder.CreateAdd(leftExpression, rightExpression);
  case Token::MINUS:
    return m_builder.CreateSub(leftExpression, rightExpression);
  default:
    throw NotImplementedException();
  }
}

llvm::Value *IrGenerator::visit(AstExpressionStatement *expressionStatement) {
  const auto &nodes = expressionStatement->getNodes();
  assert(nodes.size() == 1);
  nodes[0]->accept(this);
  return {};
}

llvm::Value *IrGenerator::visit(AstInputStatement *statement) {
  std::vector<std::string> formats;
  std::vector<llvm::Value *> args;

  for (auto &astNode : statement->getNodes()) {
    auto variableAstNode = std::dynamic_pointer_cast<AstVariableExpression>(astNode);
    std::string name = variableAstNode->getName();
    Symbol *symbol = m_symbolTable.getCurrentScope()->resolve(name);
    auto variableSymbol = dynamic_cast<VariableSymbol *>(symbol);
    assert(variableSymbol);
    auto variableValue = static_cast<llvm::AllocaInst *>(variableSymbol->getValue());
    llvm::Type *type = variableValue->getAllocatedType();

    args.push_back(variableValue);

    if (type->isIntegerTy()) {
      formats.push_back("%d");
    } else {
      throw NotImplementedException();
    }
  }

  std::ostringstream format;
  std::copy(formats.begin(), formats.end(), std::ostream_iterator<std::string>(format, " "));
  std::string formatString = format.str();
  trim(formatString);
  auto global = m_builder.CreateGlobalStringPtr(formatString.c_str());
  args.insert(args.begin(), global);

  auto *inputFunctionSymbol = dynamic_cast<MethodSymbol *>(m_symbolTable.getCurrentScope()->resolve(RAL_INPUT_CALL));
  assert(inputFunctionSymbol);

  auto *inputFunction = static_cast<llvm::Function *>(inputFunctionSymbol->getValue());
  assert(inputFunction);
  m_builder.CreateCall(inputFunction, args);

  return {};
}

llvm::Value *IrGenerator::visit(AstIntExpression *expression) {
  std::string valueString = expression->getValue();
  int value = std::stoi(valueString);
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), value, true);
}

llvm::Value *IrGenerator::visit(AstReturnStatement *returnStatement) {
  addReturnStatement();
  // AstReturnStatement returns something (not used)
  // All other statements does not return value
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), 1, true);
}

llvm::Value *IrGenerator::visit(AstUnaryExpression *expression) {
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 1);
  auto astExpr = nodes[0];
  assert(astExpr);
  llvm::Value *exprValue = astExpr->accept(this);

  auto type = exprValue->getType();
  // emitLocation(context, debugInfo.unit);

  if (type->isIntegerTy()) {
    auto zero = llvm::ConstantInt::get(type, 0);
    return m_builder.CreateSub(zero, exprValue);
  }

  throw NotImplementedException();
}

llvm::Value *IrGenerator::visit(AstVariableDeclarationStatement *statement) {
  //    emitLocation(context, debugInfo.unit);
  std::string name = statement->getName();
  std::string typeName = statement->getTypeName();
  auto *type = resolveType(m_symbolTable.getCurrentScope(), typeName);
  assert(type);
  llvm::Value *expression = nullptr;
  auto sz = statement->getNodes().size();
  assert(sz <= 1);
  if (sz == 1) {
    expression = statement->getNodes()[0]->accept(this);
  }
  Symbol *symbol = m_symbolTable.createVariableSymbol(name, type);
  assert(symbol);
  m_symbolTable.getCurrentScope()->define(std::unique_ptr<Symbol>(symbol));
  MethodSymbol *alg = getCurrentMethod(m_symbolTable.getCurrentScope());
  assert(alg);
  auto f = static_cast<llvm::Function *>(alg->getValue());
  assert(f);
  llvm::AllocaInst *variableAlloca = createEntryBlockAlloca(&m_llvmContext, f, symbol);
  if (expression) {
    m_builder.CreateStore(expression, variableAlloca, false);
  }
  symbol->setValue(variableAlloca);
  return {};
}

llvm::Value *IrGenerator::visit(AstVariableExpression *expression) {
  auto name = expression->getName();
  auto variableSymbol = m_symbolTable.getCurrentScope()->resolve(name);
  auto variableValue = variableSymbol->getValue();
  // TODO: Handle globals
  auto variable = static_cast<llvm::AllocaInst *>(variableValue);
  if (!variable) {
    throw VariableNotFoundException("Cannot allocate variable " + name + " at line " +
                                    std::to_string(expression->getLine()));
  }
  //    emitLocation(context, debugInfo.unit);

  // TODO: check is Load really required. If not then this function may be used
  // in Input statement
  return this->m_builder.CreateLoad(variable->getAllocatedType(), variable);
}

llvm::Value *IrGenerator::visit(AstVariableAffectationExpression *expression) {
  auto name = expression->getName();
  auto variableSymbol = m_symbolTable.getCurrentScope()->resolve(name);
  auto variableValue = variableSymbol->getValue();
  // TODO: Handle globals
  auto variable = static_cast<llvm::AllocaInst *>(variableValue);
  if (!variable) {
    throw VariableNotFoundException("Cannot allocate variable " + name + " at line " +
                                    std::to_string(expression->getLine()));
  }

  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 1);
  auto astExpr = nodes[0];
  assert(astExpr);
  llvm::Value *exprValue = astExpr->accept(this);
  this->m_builder.CreateStore(exprValue, variable);
  return this->m_builder.CreateLoad(variable->getAllocatedType(), variable);
}

void IrGenerator::addReturnStatement() {
  Symbol *returnSymbol = m_symbolTable.getCurrentScope()->resolve(RAL_RET_VALUE);
  auto *returnAllocaInst = returnSymbol ? static_cast<llvm::AllocaInst *>(returnSymbol->getValue()) : nullptr;
  if (returnAllocaInst) {
    auto value = m_builder.CreateLoad(returnAllocaInst->getAllocatedType(), returnAllocaInst);
    m_builder.CreateRet(value);
  } else {
    m_builder.CreateRetVoid();
  }
}

llvm::Value *IrGenerator::visit(AstFunctionAffectationExpression *expression) {
  std::string name = RAL_RET_VALUE;
  auto variableSymbol = m_symbolTable.getCurrentScope()->resolve(name);
  assert(variableSymbol);
  auto variableValue = variableSymbol->getValue();
  // TODO: Handle globals
  auto variable = static_cast<llvm::AllocaInst *>(variableValue);
  if (!variable) {
    throw VariableNotFoundException("Cannot allocate function return variable " + name + " at line " +
                                    std::to_string(expression->getLine()));
  }

  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 1);
  auto astExpr = nodes[0];
  assert(astExpr);
  llvm::Value *exprValue = astExpr->accept(this);
  this->m_builder.CreateStore(exprValue, variable);
  return this->m_builder.CreateLoad(variable->getAllocatedType(), variable);
}

llvm::Value *IrGenerator::visit(AstIfStatement *statement) {
  //    emitLocation(context, debugInfo.unit);
  auto astIfCondition = statement->ifCondition();
  auto astThenBlock = statement->thenBlock();
  auto astElseBlock = statement->elseBlock();

  llvm::Value *ifConditionValue = astIfCondition->accept(this);
  auto type = ifConditionValue->getType();
  if (type->isIntegerTy()) {
    auto zero = llvm::ConstantInt::get(type, 0);
    ifConditionValue = m_builder.CreateICmpNE(ifConditionValue, zero);
  }

  llvm::BasicBlock *previousBlock = m_builder.GetInsertBlock();
  llvm::Function *function = previousBlock->getParent();
  assert(function);

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(m_llvmContext, "then", function);
  llvm::BasicBlock *elseBB = astElseBlock.size() ? llvm::BasicBlock::Create(m_llvmContext, "else") : nullptr;
  llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(m_llvmContext, "ifcont");

  m_builder.CreateCondBr(ifConditionValue, thenBB, elseBB ? elseBB : mergeBB);

  m_builder.SetInsertPoint(thenBB);

  bool returnStatementFound = false;
  llvm::Value *returnValue = nullptr;
  for (auto st : astThenBlock) {
    returnValue = st->accept(this);
    // TODO: good Return processing
    if (returnValue) {
      returnStatementFound = true;
      break;
    }
  }
  if (!returnStatementFound) {
    m_builder.CreateBr(mergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the
    // PHI.
    thenBB = m_builder.GetInsertBlock();
  }

  // Emit else block.
  bool elseReturnStatementFound = false;
  if (elseBB) {
    function->insert(function->end(), elseBB);
    m_builder.SetInsertPoint(elseBB);
    for (auto st : astElseBlock) {
      llvm::Value *v = st->accept(this);
      // TODO: good Return processing
      if (v) {
        returnValue = v;
        elseReturnStatementFound = true;
        break;
      }
    }
    if (!elseReturnStatementFound) {
      m_builder.CreateBr(mergeBB);
      // Codegen of 'Else' can change the current block, update elseBB for the
      // PHI.
      elseBB = m_builder.GetInsertBlock();
    }
  }
  if (returnStatementFound && elseReturnStatementFound) {
    return returnValue;
  }

  // Emit merge block.
  function->insert(function->end(), mergeBB);
  m_builder.SetInsertPoint(mergeBB);
  return {};
}

} // namespace RaLang
