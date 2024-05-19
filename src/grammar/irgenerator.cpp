#include "irgenerator.h"

#include "grammar/runtime/RalParser.h"
#include "logic/debuginfo.h"
#include "logic/symboltable.h"
#include "ralconsts.h"
#include "ralexceptions.h"
#include "utils.h"

#include <llvm/IR/Verifier.h>

namespace RaLang {

IrGenerator::IrGenerator(bool emitDebugInfo, const std::string &path, SymbolTable &symbolTable,
                         llvm::LLVMContext &llvmContext, llvm::IRBuilder<> &builder, llvm::Module &module)
    : m_path(path), m_symbolTable(symbolTable), m_llvmContext(llvmContext), m_builder(builder), m_module(module),
      m_has_return_statement(false) {
  if (emitDebugInfo) {
    m_debugInfo.reset(new DebugInfo(path, module, builder));
  } else {
    m_debugInfo.reset(new DebugInfoDummy());
  }
}

IrGenerator::~IrGenerator() = default;

void IrGenerator::visit(Ast &ast) {
  for (auto &module : ast.getNodes()) {
    module->accept(this);
  }
  m_debugInfo->finalize();
}

void IrGenerator::visit(AstModule *module) {
  AstAlgorithm *astMainAlg = nullptr;
  for (auto &node : module->getNodes()) {
    node->accept(this);
    if (astMainAlg == nullptr) {
      auto alg = std::dynamic_pointer_cast<AstAlgorithm>(node);
      if (alg) {
        astMainAlg = alg.get();
      }
    }
  }

  // Generate the entry point - int main() function
  llvm::FunctionType *functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(m_llvmContext), {}, false);
  llvm::Function *function =
      llvm::Function::Create(functionType, llvm::GlobalValue::LinkageTypes::ExternalLinkage, RAL_MAIN, m_module);
  llvm::BasicBlock *block = llvm::BasicBlock::Create(m_builder.getContext());

  AlgSymbol *mainFunctionSymbol =
      m_symbolTable.createAlgSymbol(RAL_MAIN, dynamic_cast<Type *>(m_symbolTable.getGlobals()->resolve(RAL_INT)));
  mainFunctionSymbol->setValue(function);
  m_symbolTable.pushScope(mainFunctionSymbol);
  auto localScopeMainFunction = m_symbolTable.createLocalScope(mainFunctionSymbol);
  m_symbolTable.pushScope(localScopeMainFunction);
  block->insertInto(function);
  m_builder.SetInsertPoint(block);

  // Main function is 1st function TODO: handle arguments;
  if (astMainAlg == nullptr) {
    throw VariableNotFoundException("No entry point");
  }

  auto mainAlgName = astMainAlg->getName();
  AlgSymbol *mainAlgSymbol = resolveAlgorithm(m_symbolTable.getCurrentScope(), mainAlgName, astMainAlg->getLine());
  if (mainAlgSymbol->getFormalParameters().size() != 0) {
    throw NotImplementedException("Entry point with parameters is not supported: " + mainAlgName);
  }

  auto calleeMainAlg = mainAlgSymbol->getFunction();
  assert(calleeMainAlg);
  if (calleeMainAlg == nullptr) {
    throw InternalException("No function " + mainAlgName);
  }

  llvm::Type *returnType = calleeMainAlg->getReturnType();
  if (!returnType->isVoidTy()) {
    throw NotImplementedException("Entry point should return no value: " + mainAlgName);
  }

  // Don't try to pass a name in to CallInst::Create when trying to create a
  // call that returns void.
  m_builder.CreateCall(calleeMainAlg, {});
  m_builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), 0, true));
  m_symbolTable.popScope(); // localScopeMainFunction
  m_symbolTable.popScope(); // mainFunctionSymbol
}

void IrGenerator::visit(AstPrintStatement *statement) {
  std::vector<std::string> formats;
  std::vector<llvm::Value *> args;

  for (auto &exprAstNode : statement->getNodes()) {
    llvm::Value *exprValue = exprAstNode->accept(this);
    llvm::Type *type = exprValue->getType();

    args.push_back(exprValue);

    if (type->isIntegerTy()) {
      formats.push_back("%d");
    } else if (type->isDoubleTy()) {
      formats.push_back("%f");
    } else if (std::dynamic_pointer_cast<AstStringLiteralExpression>(exprAstNode)) {
      formats.push_back("%s");
    } else {
      throw NotImplementedException();
    }
  }

  std::ostringstream format;
  std::copy(formats.begin(), formats.end(), std::ostream_iterator<std::string>(format, ""));
  std::string formatString = format.str();
  llvm::Constant *global = m_builder.CreateGlobalStringPtr(formatString.c_str());
  args.insert(args.begin(), global);

  AlgSymbol *printFunctionSymbol =
      resolveAlgorithm(m_symbolTable.getCurrentScope(), RAL_PRINT_CALL, statement->getLine());

  llvm::Function *printFunction = printFunctionSymbol->getFunction();
  m_builder.CreateCall(printFunction, args);
}

static llvm::AllocaInst *createEntryBlockAlloca(llvm::LLVMContext *context, llvm::Function *function, Symbol *symbol) {
  llvm::IRBuilder<> builder(&function->getEntryBlock(), function->getEntryBlock().begin());
  return builder.CreateAlloca(symbol->getType()->createLlvmType(*context), nullptr, symbol->getName());
}

void IrGenerator::visit(AstAlgorithm *algorithm) {
  std::string algName = algorithm->getName();
  AlgSymbol *algSymbol = resolveAlgorithm(m_symbolTable.getGlobals(), algName, algorithm->getLine());
  m_symbolTable.pushScope(algSymbol);

  std::vector<Symbol *> formalParameters = algSymbol->getFormalParameters();

  llvm::Function *f = algSymbol->getFunction();
  llvm::Type *returnType = f->getReturnType();

  // Create a new basic block to start insertion into.
  llvm::BasicBlock *functionBasicBlock = llvm::BasicBlock::Create(m_builder.getContext(), "entry", f);
  m_builder.SetInsertPoint(functionBasicBlock);

  llvm::DISubprogram *debugAlgSymbol = m_debugInfo->createAlgScope(algorithm->getLine(), algSymbol);

  // Set names for all arguments.
  unsigned index = 0;
  for (llvm::Function::arg_iterator attrIndex = f->arg_begin(); index != formalParameters.size();
       ++attrIndex, ++index) {
    Symbol *symbol = formalParameters[index];
    std::string name = symbol->getName();
    attrIndex->setName(name);
    llvm::AllocaInst *attrAlloca = createEntryBlockAlloca(&m_llvmContext, f, symbol);
    symbol->setValue(attrAlloca);
    m_debugInfo->defineAlgParam(debugAlgSymbol, symbol, attrAlloca, index + 1);
    m_builder.CreateStore(attrIndex, attrAlloca);
  }

  m_debugInfo->emitLocation(algorithm->getLine());

  auto localScopeFunction = m_symbolTable.createLocalScope(algSymbol);
  m_symbolTable.pushScope(localScopeFunction);

  m_debugInfo->createLocalScope(algorithm->getLine());

  // Create storage for return value
  if (!returnType->isVoidTy()) {
    VariableSymbol *returnValueSymbol = m_symbolTable.createVariableSymbol(RAL_RET_VALUE, algSymbol->getType());
    localScopeFunction->define(std::unique_ptr<Symbol>(returnValueSymbol));
    llvm::AllocaInst *returnValueAttrAlloca = createEntryBlockAlloca(&m_llvmContext, f, returnValueSymbol);
    returnValueSymbol->setValue(returnValueAttrAlloca);
    m_debugInfo->defineLocalVariable(returnValueSymbol, returnValueAttrAlloca, algorithm->getLine());
  }

  llvm::Value *returnStatementFlag = nullptr;
  for (auto node : algorithm->getNodes()) {
    node->accept(this);
    if (m_has_return_statement) {
      break;
    }
  }

  if (!m_has_return_statement) {
    addReturnStatement();
  } else {
    m_has_return_statement = false;
  }

  m_symbolTable.popScope(); // local
  m_debugInfo->popScope();  // debug information

  llvm::verifyFunction(*f);

  m_symbolTable.popScope(); // algorithm
  m_debugInfo->popScope();  // debug information
}

llvm::Value *IrGenerator::visit(AstAlgorithmCallExpression *algorithmCall) {
  m_debugInfo->emitLocation(algorithmCall->getLine());
  auto name = algorithmCall->getName();
  AlgSymbol *calleeSymbol = resolveAlgorithm(m_symbolTable.getCurrentScope(), name, algorithmCall->getLine());
  llvm::Function *f = calleeSymbol->getFunction();

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
  m_debugInfo->emitLocation(expression->getLine());
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftExpression = nodes[0]->accept(this);
  assert(leftExpression);
  auto rightExpression = nodes[1]->accept(this);
  assert(rightExpression);

  AstTokenType t = expression->getTokenType();
  switch (t) {
  case AstTokenType::COND_EQ:
    return m_builder.CreateICmpEQ(leftExpression, rightExpression);
  case AstTokenType::COND_NE:
    return m_builder.CreateICmpNE(leftExpression, rightExpression);
  case AstTokenType::COND_GT:
    return m_builder.CreateICmpSGT(leftExpression, rightExpression);
  case AstTokenType::COND_GE:
    return m_builder.CreateICmpSGE(leftExpression, rightExpression);
  case AstTokenType::COND_LT:
    return m_builder.CreateICmpSLT(leftExpression, rightExpression);
  case AstTokenType::COND_LE:
    return m_builder.CreateICmpSLE(leftExpression, rightExpression);
  default:
    throw NotImplementedException();
  }
}

llvm::Value *IrGenerator::visit(AstBinaryLogicalExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftExpression = nodes[0]->accept(this);
  assert(leftExpression);
  auto rightExpression = nodes[1]->accept(this);
  assert(rightExpression);

  AstTokenType t = expression->getTokenType();
  switch (t) {
  case AstTokenType::LOGICAL_AND:
    return m_builder.CreateAnd(leftExpression, rightExpression);
  case AstTokenType::LOGICAL_OR:
    return m_builder.CreateOr(leftExpression, rightExpression);
  default:
    throw NotImplementedException("Logical operation not supported " +
                                  std::to_string(static_cast<int>(expression->getTokenType())) + " at line " +
                                  std::to_string(expression->getLine()));
  }
}

llvm::Value *IrGenerator::visit(AstMathExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftExpression = nodes[0]->accept(this);
  assert(leftExpression);
  auto rightExpression = nodes[1]->accept(this);
  assert(rightExpression);

  AstTokenType t = expression->getTokenType();
  switch (t) {
  case AstTokenType::MUL:
    return m_builder.CreateMul(leftExpression, rightExpression);
  case AstTokenType::DIV:
    return m_builder.CreateSDiv(leftExpression, rightExpression);
  case AstTokenType::MOD:
    return m_builder.CreateSRem(leftExpression, rightExpression);
  case AstTokenType::PLUS:
    return m_builder.CreateAdd(leftExpression, rightExpression);
  case AstTokenType::MINUS:
    return m_builder.CreateSub(leftExpression, rightExpression);
  default:
    throw NotImplementedException();
  }
}

void IrGenerator::visit(AstExpressionStatement *expressionStatement) {
  const auto &nodes = expressionStatement->getNodes();
  assert(nodes.size() == 1);
  nodes[0]->accept(this);
}

void IrGenerator::visit(AstInputStatement *statement) {
  std::vector<std::string> formats;
  std::vector<llvm::Value *> args;

  m_debugInfo->emitLocation(statement->getLine());
  for (auto &astNode : statement->getNodes()) {
    auto variableAstNode = std::dynamic_pointer_cast<AstVariableExpression>(astNode);
    std::string name = variableAstNode->getName();
    VariableSymbol *variableSymbol = resolveVariable(m_symbolTable.getCurrentScope(), name, statement->getLine());

    // TODO: Handle globals
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

  AlgSymbol *inputFunctionSymbol =
      resolveAlgorithm(m_symbolTable.getCurrentScope(), RAL_INPUT_CALL, statement->getLine());
  llvm::Function *inputFunction = inputFunctionSymbol->getFunction();
  m_builder.CreateCall(inputFunction, args);
}

llvm::Value *IrGenerator::visit(AstNumberLiteralExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  std::string valueString = expression->getValue();
  switch (expression->getTokenType()) {
  case AstTokenType::INT_LITERAL: {
    int value = std::stoi(valueString);
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), value, true);
  }
  case AstTokenType::REAL_LITERAL: {
    double value = std::stod(valueString);
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(m_llvmContext), value);
  }
  default:
    throw NotImplementedException("Unsupported literal " + valueString + "(" +
                                  std::to_string(static_cast<int>(expression->getTokenType())));
  }
}

void IrGenerator::visit(AstReturnStatement *returnStatement) {
  m_debugInfo->emitLocation(returnStatement->getLine());
  m_has_return_statement = true;
  addReturnStatement();
}

llvm::Value *IrGenerator::visit(AstStringLiteralExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  std::string valueString = expression->getValue();
  llvm::GlobalVariable *constantString = m_builder.CreateGlobalString(valueString.c_str());
  constantString->setConstant(true);
  return constantString;
}

llvm::Value *IrGenerator::visit(AstTypePromotionExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 1);
  auto astExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(astExpr);
  llvm::Value *exprValue = astExpr->accept(this);

  // TODO: support all cases
  if (astExpr->getTypeKind() == TypeKind::Int) {
    if (expression->getTypeKind() == TypeKind::Real) {
      Type *symbolType = m_symbolTable.getCurrentScope()->resolve(RAL_REAL)->getType();
      assert(symbolType);
      return m_builder.CreateSIToFP(exprValue, symbolType->createLlvmType(m_llvmContext), "int_real_promo");
    }
  }

  if (astExpr->getTypeKind() == TypeKind::Real) {
    if (expression->getTypeKind() == TypeKind::Int) {
      Type *symbolType = m_symbolTable.getCurrentScope()->resolve(RAL_INT)->getType();
      assert(symbolType);
      return m_builder.CreateFPToSI(exprValue, symbolType->createLlvmType(m_llvmContext), "real_int_promo");
    }
  }

  throw NotImplementedException("Not implemented type promotion at line " + std::to_string(expression->getLine()));
}

llvm::Value *IrGenerator::visit(AstUnaryExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 1);
  auto astExpr = nodes[0];
  assert(astExpr);
  llvm::Value *exprValue = astExpr->accept(this);

  auto type = exprValue->getType();
  switch (expression->getTokenType()) {
  case AstTokenType::UNARI_MINUS: {
    if (type->isIntegerTy()) {
      auto zero = llvm::ConstantInt::get(type, 0);
      return m_builder.CreateSub(zero, exprValue);
    }
    throw NotImplementedException("Unary minus is not supported at line " + std::to_string(expression->getLine()));
  }
  case AstTokenType::LOGICAL_NOT: {
    if (type->isIntegerTy()) {
      return m_builder.CreateNot(exprValue);
    }
    throw NotImplementedException("Logical NOT is not supported at line " + std::to_string(expression->getLine()));
  }
  default:
    throw NotImplementedException("Unary operation not supported " +
                                  std::to_string(static_cast<int>(expression->getTokenType())) + " at line " +
                                  std::to_string(expression->getLine()));
  }
}

void IrGenerator::visit(AstVariableDeclarationStatement *statement) {
  m_debugInfo->emitLocation(statement->getLine());

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
  AlgSymbol *alg = getCurrentAlg(m_symbolTable.getCurrentScope());
  assert(alg);
  llvm::Function *f = alg->getFunction();

  llvm::AllocaInst *variableAlloca = createEntryBlockAlloca(&m_llvmContext, f, symbol);
  m_debugInfo->defineLocalVariable(symbol, variableAlloca, statement->getLine());
  if (expression) {
    m_builder.CreateStore(expression, variableAlloca, false);
  }
  symbol->setValue(variableAlloca);
}

llvm::Value *IrGenerator::visit(AstVariableExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  auto name = expression->getName();
  auto variableSymbol = m_symbolTable.getCurrentScope()->resolve(name);
  auto variableValue = variableSymbol->getValue();
  // TODO: Handle globals
  auto variable = static_cast<llvm::AllocaInst *>(variableValue);
  if (!variable) {
    throw VariableNotFoundException("Cannot allocate variable " + name + " at line " +
                                    std::to_string(expression->getLine()));
  }

  // TODO: check is Load really required. If not then this function may be used
  // in Input statement
  return this->m_builder.CreateLoad(variable->getAllocatedType(), variable);
}

llvm::Value *IrGenerator::visit(AstVariableAffectationExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
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
  m_debugInfo->emitLocation(expression->getLine());
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

void IrGenerator::visit(AstIfStatement *statement) {
  m_debugInfo->emitLocation(statement->getLine());
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
    st->accept(this);
    if (m_has_return_statement) {
      returnStatementFound = true;
      break;
    }
  }
  if (!m_has_return_statement) {
    m_builder.CreateBr(mergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the
    // PHI.
    thenBB = m_builder.GetInsertBlock();
  } else {
    m_has_return_statement = false;
  }

  // Emit else block.
  bool elseReturnStatementFound = false;
  if (elseBB) {
    function->insert(function->end(), elseBB);
    m_builder.SetInsertPoint(elseBB);
    for (auto st : astElseBlock) {
      st->accept(this);
      if (m_has_return_statement) {
        elseReturnStatementFound = true;
        break;
      }
    }
    if (!m_has_return_statement) {
      m_builder.CreateBr(mergeBB);
      // Codegen of 'Else' can change the current block, update elseBB for the
      // PHI.
      elseBB = m_builder.GetInsertBlock();
    } else {
      m_has_return_statement = false;
    }
  }
  if (!(returnStatementFound && elseReturnStatementFound)) {
    // Emit merge block.
    function->insert(function->end(), mergeBB);
    m_builder.SetInsertPoint(mergeBB);
  } else {
    m_has_return_statement = true;
  }
}

} // namespace RaLang
