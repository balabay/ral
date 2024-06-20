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
  // Main function is 1st function TODO: handle arguments;
  if (astMainAlg == nullptr) {
    throw VariableNotFoundException("No entry point");
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

  auto mainAlgName = astMainAlg->getName();
  AlgSymbol *mainAlgSymbol = resolveAlgorithm(localScopeMainFunction, mainAlgName, astMainAlg->getLine());
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

  std::vector<PrintFormatSpecifier> formatSpecifiers = statement->getFormatSpecifiers();
  size_t i = 0;
  for (auto &exprAstNode : statement->getNodes()) {
    llvm::Value *exprValue = exprAstNode->accept(this);
    llvm::Type *type = exprValue->getType();
    if (type->isIntegerTy()) {
      std::string format = "%";
      if (formatSpecifiers[i].first) {
        format += '*';
        llvm::Value *widthValue = formatSpecifiers[i].first->accept(this);
        args.push_back(widthValue);
      }
      format += 'd';
      formats.push_back(format);
    } else if (type->isDoubleTy()) {
      std::string format = "%";
      if (formatSpecifiers[i].first) {
        format += '*';
        llvm::Value *widthValue = formatSpecifiers[i].first->accept(this);
        args.push_back(widthValue);
      }
      if (formatSpecifiers[i].second) {
        format += ".*";
        llvm::Value *precisionValue = formatSpecifiers[i].second->accept(this);
        args.push_back(precisionValue);
      }
      format += 'f';
      formats.push_back(format);
    } else if (std::dynamic_pointer_cast<AstStringLiteralExpression>(exprAstNode)) {
      formats.push_back("%s");
    } else {
      throw NotImplementedException();
    }
    args.push_back(exprValue);
    i++;
  }

  std::ostringstream format;
  std::copy(formats.begin(), formats.end(), std::ostream_iterator<std::string>(format, ""));
  std::string formatString = format.str();
  llvm::Constant *global = m_builder.CreateGlobalStringPtr(formatString.c_str());
  args.insert(args.begin(), global);

  AlgSymbol *printFunctionSymbol = resolveAlgorithm(statement->getScope(), RAL_PRINT_CALL, statement->getLine());

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
  m_symbolTable.pushScope(algorithm->getLocalScope());
  m_debugInfo->createLocalScope(algorithm->getLine());

  // Create storage for return value
  if (!returnType->isVoidTy()) {
    VariableSymbol *returnValueSymbol =
        resolveVariable(algorithm->getLocalScope(), RAL_RET_VALUE, algorithm->getLine());
    assert(returnValueSymbol);
    llvm::AllocaInst *returnValueAttrAlloca = createEntryBlockAlloca(&m_llvmContext, f, returnValueSymbol);
    returnValueSymbol->setValue(returnValueAttrAlloca);
    m_debugInfo->defineLocalVariable(returnValueSymbol, returnValueAttrAlloca, algorithm->getLine());
  }

  for (auto node : algorithm->getNodes()) {
    node->accept(this);
    if (m_has_return_statement) {
      break;
    }
  }

  if (!m_has_return_statement) {
    addReturnStatement(algorithm->getLocalScope());
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
  int line = algorithmCall->getLine();
  auto name = algorithmCall->getName();
  AlgSymbol *algSymbol = resolveAlgorithm(algorithmCall->getScope(), name, line);
  llvm::Function *f = algSymbol->getFunction();

  auto actualArgs = algorithmCall->getNodes();
  assert(f->arg_size() == actualArgs.size());

  std::vector<llvm::Value *> argValues;
  for (unsigned i = 0; i != actualArgs.size(); i++) {
    auto astExpr = dynamic_cast<AstExpression *>(actualArgs[i].get());
    assert(astExpr);
    llvm::Value *exprValue = astExpr->accept(this);
    argValues.push_back(exprValue);
    if (argValues.back() == nullptr) {
      throw VariableNotFoundException("Incorrect argument " + std::to_string(i) + ". Line " + std::to_string(line));
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

  auto leftAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(leftAstExpr);
  auto rightAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[1]);
  assert(rightAstExpr);
  assert(leftAstExpr->getTypeKind() == rightAstExpr->getTypeKind());

  llvm::Value *leftExprValue = nodes[0]->accept(this);
  assert(leftExprValue);
  llvm::Value *rightExprValue = nodes[1]->accept(this);
  assert(rightExprValue);

  AstTokenType t = expression->getTokenType();
  if ((leftAstExpr->getTypeKind() == TypeKind::Int) || (leftAstExpr->getTypeKind() == TypeKind::Boolean)) {
    return compareIntExpressions(t, leftExprValue, rightExprValue);
  } else if (leftAstExpr->getTypeKind() == TypeKind::Real) {
    return compareRealExpressions(t, leftExprValue, rightExprValue);
  }
  throw NotImplementedException("Type comparison " + std::to_string(expression->getLine()));
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
                                  astTokenTypeToString(expression->getTokenType()) + " at line " +
                                  std::to_string(expression->getLine()));
  }
}

llvm::Value *IrGenerator::visit(AstMathExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  const auto &nodes = expression->getNodes();
  assert(nodes.size() == 2);

  auto leftAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[0]);
  assert(leftAstExpr);
  auto rightAstExpr = std::dynamic_pointer_cast<AstExpression>(nodes[1]);
  assert(rightAstExpr);
  assert(leftAstExpr->getTypeKind() == rightAstExpr->getTypeKind());

  llvm::Value *leftExprValue = nodes[0]->accept(this);
  assert(leftExprValue);
  llvm::Value *rightExprValue = nodes[1]->accept(this);
  assert(rightExprValue);

  AstTokenType t = expression->getTokenType();
  if ((leftAstExpr->getTypeKind() == TypeKind::Int) || (leftAstExpr->getTypeKind() == TypeKind::Boolean)) {
    return mathIntExpressions(t, leftExprValue, rightExprValue);
  } else if (leftAstExpr->getTypeKind() == TypeKind::Real) {
    return mathRealExpressions(t, leftExprValue, rightExprValue);
  }
  throw NotImplementedException("Type comparison for math " + std::to_string(expression->getLine()));
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
    VariableSymbol *variableSymbol = resolveVariable(statement->getScope(), name, statement->getLine());

    // TODO: Handle globals
    auto variableValue = static_cast<llvm::AllocaInst *>(variableSymbol->getValue());
    llvm::Type *type = variableValue->getAllocatedType();

    args.push_back(variableValue);

    if (type->isIntegerTy()) {
      formats.push_back("%d");
    } else if (type->isFloatingPointTy()) {
      formats.push_back("%lg");
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

  AlgSymbol *inputFunctionSymbol = resolveAlgorithm(statement->getScope(), RAL_INPUT_CALL, statement->getLine());
  llvm::Function *inputFunction = inputFunctionSymbol->getFunction();
  m_builder.CreateCall(inputFunction, args);
}

void IrGenerator::visit(AstLoopStatement *statement) {
  switch (statement->getLoopType()) {
  case LoopType::K: {
    return loopK(statement);
  }
  case LoopType::While:
  case LoopType::For: {
    return loopFor(statement);
  }
  case LoopType::Until: {
    return loopUntil(statement);
  }
  default:
    throw NotImplementedException("Unsupported loop type at " + std::to_string(statement->getLine()));
  }
}

llvm::Value *IrGenerator::visit(AstNumberLiteralExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  std::string valueString = expression->getValue();
  // TODO: Use BuildInType to get llvm Type
  switch (expression->getTypeKind()) {
  case TypeKind::Int: {
    int value = std::stoi(valueString);
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(m_llvmContext), value, true);
  }
  case TypeKind::Real: {
    double value = std::stod(valueString);
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(m_llvmContext), value);
  }
  default:
    throw NotImplementedException("Unsupported literal " + valueString + "(" +
                                  typeKindToString(expression->getTypeKind()));
  }
}

void IrGenerator::visit(AstReturnStatement *returnStatement) {
  m_debugInfo->emitLocation(returnStatement->getLine());
  m_has_return_statement = true;
  addReturnStatement(returnStatement->getScope());
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
      Symbol *symbol = expression->getScope()->resolve(RAL_REAL);
      BuiltInTypeSymbol *symbolType = static_cast<BuiltInTypeSymbol *>(symbol);
      assert(symbolType);
      return m_builder.CreateSIToFP(exprValue, symbolType->createLlvmType(m_llvmContext), "int_real_promo");
    } else if (expression->getTypeKind() == TypeKind::Boolean) {
      Symbol *symbol = expression->getScope()->resolve(RAL_INT);
      BuiltInTypeSymbol *symbolType = static_cast<BuiltInTypeSymbol *>(symbol);
      assert(symbolType);
      return m_builder.CreateICmpNE(
          exprValue, llvm::ConstantInt::get(symbolType->createLlvmType(m_llvmContext), 0, true), "int_bool_promo");
    }
  }

  if (astExpr->getTypeKind() == TypeKind::Real) {
    if (expression->getTypeKind() == TypeKind::Int) {
      Symbol *symbol = expression->getScope()->resolve(RAL_INT);
      BuiltInTypeSymbol *symbolType = static_cast<BuiltInTypeSymbol *>(symbol);
      assert(symbolType);
      return m_builder.CreateFPToSI(exprValue, symbolType->createLlvmType(m_llvmContext), "real_int_promo");
    } else if (expression->getTypeKind() == TypeKind::Boolean) {
      return m_builder.CreateFCmpONE(exprValue, llvm::ConstantFP::get(m_llvmContext, llvm::APFloat(0.0)),
                                     "real_bool_promo");
    }
  }

  if (astExpr->getTypeKind() == TypeKind::Boolean) {
    if (expression->getTypeKind() == TypeKind::Int) {
      Symbol *symbol = expression->getScope()->resolve(RAL_INT);
      BuiltInTypeSymbol *symbolType = static_cast<BuiltInTypeSymbol *>(symbol);
      assert(symbolType);
      return m_builder.CreateSExt(exprValue, symbolType->createLlvmType(m_llvmContext), "bool_int_promo");
    } else if (expression->getTypeKind() == TypeKind::Real) {
      return m_builder.CreateUIToFP(exprValue, llvm::Type::getDoubleTy(m_llvmContext), "bool_real_promo");
    }
  }

  throw NotImplementedException("Not implemented type promotion at line " + std::to_string(expression->getLine()));
}

void IrGenerator::visit(AstVariableDeclarationStatement *statement) {
  m_debugInfo->emitLocation(statement->getLine());

  std::string name = statement->getName();
  llvm::Value *expression = nullptr;
  auto sz = statement->getNodes().size();
  assert(sz <= 1);
  if (sz == 1) {
    expression = statement->getNodes()[0]->accept(this);
  }
  Symbol *symbol = statement->getScope()->resolve(name);
  assert(symbol);
  AlgSymbol *alg = getCurrentAlg(statement->getScope());
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
  std::string name = expression->getName();
  Symbol *variableSymbol = expression->getScope()->resolve(name);
  llvm::Value *variableValue = variableSymbol->getValue();
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
  auto variableSymbol = expression->getScope()->resolve(name);
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

void IrGenerator::addReturnStatement(Scope *scope) {
  Symbol *returnSymbol = scope->resolve(RAL_RET_VALUE);
  auto *returnAllocaInst = returnSymbol ? static_cast<llvm::AllocaInst *>(returnSymbol->getValue()) : nullptr;
  if (returnAllocaInst) {
    auto value = m_builder.CreateLoad(returnAllocaInst->getAllocatedType(), returnAllocaInst);
    m_builder.CreateRet(value);
  } else {
    m_builder.CreateRetVoid();
  }
}

llvm::Value *IrGenerator::compareIntExpressions(AstTokenType t, llvm::Value *leftExprValue,
                                                llvm::Value *rightExprValue) {
  switch (t) {
  case AstTokenType::COND_EQ:
    return m_builder.CreateICmpEQ(leftExprValue, rightExprValue);
  case AstTokenType::COND_NE:
    return m_builder.CreateICmpNE(leftExprValue, rightExprValue);
  case AstTokenType::COND_GT:
    return m_builder.CreateICmpSGT(leftExprValue, rightExprValue);
  case AstTokenType::COND_GE:
    return m_builder.CreateICmpSGE(leftExprValue, rightExprValue);
  case AstTokenType::COND_LT:
    return m_builder.CreateICmpSLT(leftExprValue, rightExprValue);
  case AstTokenType::COND_LE:
    return m_builder.CreateICmpSLE(leftExprValue, rightExprValue);
  default:
    throw NotImplementedException();
  }
}

llvm::Value *IrGenerator::compareRealExpressions(AstTokenType t, llvm::Value *leftExprValue,
                                                 llvm::Value *rightExprValue) {
  switch (t) {
  case AstTokenType::COND_EQ:
    return m_builder.CreateFCmpOEQ(leftExprValue, rightExprValue);
  case AstTokenType::COND_NE:
    return m_builder.CreateFCmpONE(leftExprValue, rightExprValue);
  case AstTokenType::COND_GT:
    return m_builder.CreateFCmpOGT(leftExprValue, rightExprValue);
  case AstTokenType::COND_GE:
    return m_builder.CreateFCmpOGE(leftExprValue, rightExprValue);
  case AstTokenType::COND_LT:
    return m_builder.CreateFCmpOLT(leftExprValue, rightExprValue);
  case AstTokenType::COND_LE:
    return m_builder.CreateFCmpOLE(leftExprValue, rightExprValue);
  default:
    throw NotImplementedException();
  }
}

llvm::Value *IrGenerator::mathIntExpressions(AstTokenType t, llvm::Value *leftExprValue, llvm::Value *rightExprValue) {
  switch (t) {
  case AstTokenType::MUL:
    return m_builder.CreateMul(leftExprValue, rightExprValue);
  case AstTokenType::DIV:
    return m_builder.CreateSDiv(leftExprValue, rightExprValue);
  case AstTokenType::MOD:
    return m_builder.CreateSRem(leftExprValue, rightExprValue);
  case AstTokenType::PLUS:
    return m_builder.CreateAdd(leftExprValue, rightExprValue);
  case AstTokenType::MINUS:
    return m_builder.CreateSub(leftExprValue, rightExprValue);
  default:
    throw NotImplementedException();
  }
}

llvm::Value *IrGenerator::mathRealExpressions(AstTokenType t, llvm::Value *leftExprValue, llvm::Value *rightExprValue) {
  switch (t) {
  case AstTokenType::MUL:
    return m_builder.CreateFMul(leftExprValue, rightExprValue);
  case AstTokenType::DIV:
    return m_builder.CreateFDiv(leftExprValue, rightExprValue);
  case AstTokenType::MOD:
    return m_builder.CreateFRem(leftExprValue, rightExprValue);
  case AstTokenType::PLUS:
    return m_builder.CreateFAdd(leftExprValue, rightExprValue);
  case AstTokenType::MINUS:
    return m_builder.CreateFSub(leftExprValue, rightExprValue);
  default:
    throw NotImplementedException();
  }
}

void IrGenerator::loopK(AstLoopStatement *statement) {
  m_debugInfo->emitLocation(statement->getLine());
  auto astLoopCount = statement->getLoopExpression();
  auto astBody = statement->getNodes();

  llvm::Value *loopCountValue = astLoopCount->accept(this);

  llvm::BasicBlock *previousBlock = m_builder.GetInsertBlock();
  llvm::Function *function = previousBlock->getParent();
  assert(function);

  llvm::BasicBlock *headBB = llvm::BasicBlock::Create(m_llvmContext, "head", function);
  llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(m_llvmContext, "loop", function);
  llvm::BasicBlock *stepBB = llvm::BasicBlock::Create(m_llvmContext, "step", function);
  llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(m_llvmContext, "merge");

  m_builder.CreateBr(headBB);
  m_builder.SetInsertPoint(headBB);

  // Start the PHI node with an entry for Start.
  llvm::Type *type = llvm::Type::getInt32Ty(m_llvmContext);
  llvm::PHINode *variable = m_builder.CreatePHI(type, 2);
  variable->addIncoming(loopCountValue, previousBlock);

  auto zero = llvm::ConstantInt::get(type, 0);
  llvm::Value *condition = m_builder.CreateICmpNE(variable, zero);
  m_builder.CreateCondBr(condition, loopBB, mergeBB);

  m_builder.SetInsertPoint(loopBB);
  bool returnStatementFound = false;
  for (auto st : astBody) {
    st->accept(this);
    if (m_has_return_statement) {
      returnStatementFound = true;
      break;
    }
  }

  if (!m_has_return_statement) {
    m_builder.CreateBr(stepBB);
    m_builder.SetInsertPoint(stepBB);
    llvm::Value *updatedCounter = m_builder.CreateSub(variable, llvm::ConstantInt::get(type, 1), "updateCounter");
    stepBB = m_builder.GetInsertBlock();
    m_builder.CreateBr(headBB);
    variable->addIncoming(updatedCounter, stepBB);
    function->insert(function->end(), mergeBB);
    m_builder.SetInsertPoint(mergeBB);
  }
}

void IrGenerator::loopUntil(AstLoopStatement *statement) {
  m_debugInfo->emitLocation(statement->getLine());

  llvm::BasicBlock *previousBlock = m_builder.GetInsertBlock();
  llvm::Function *function = previousBlock->getParent();
  assert(function);

  llvm::BasicBlock *headBB = llvm::BasicBlock::Create(m_llvmContext, "head", function);
  llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(m_llvmContext, "loop", function);
  llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(m_llvmContext, "merge");

  m_builder.CreateBr(loopBB);
  m_builder.SetInsertPoint(loopBB);
  bool returnStatementFound = false;
  for (auto st : statement->getNodes()) {
    st->accept(this);
    if (m_has_return_statement) {
      returnStatementFound = true;
      break;
    }
  }

  if (!m_has_return_statement) {
    m_builder.CreateBr(headBB);
    m_builder.SetInsertPoint(headBB);

    llvm::Value *loopExpr = statement->getLoopExpression()->accept(this);
    m_builder.CreateCondBr(loopExpr, mergeBB, loopBB);

    function->insert(function->end(), mergeBB);
    m_builder.SetInsertPoint(mergeBB);
  }
}

void IrGenerator::loopFor(AstLoopStatement *statement) {
  m_debugInfo->emitLocation(statement->getLine());
  auto astStart = statement->getStartStatement();
  if (astStart)
    astStart->accept(this);

  llvm::BasicBlock *previousBlock = m_builder.GetInsertBlock();
  llvm::Function *function = previousBlock->getParent();
  assert(function);

  llvm::BasicBlock *headBB = llvm::BasicBlock::Create(m_llvmContext, "head", function);
  llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(m_llvmContext, "loop", function);
  auto astStep = statement->getStepExpression();
  llvm::BasicBlock *stepBB = astStep ? llvm::BasicBlock::Create(m_llvmContext, "step", function) : nullptr;
  llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(m_llvmContext, "merge");

  m_builder.CreateBr(headBB);
  m_builder.SetInsertPoint(headBB);

  llvm::Value *condition = statement->getLoopExpression()->accept(this);
  m_builder.CreateCondBr(condition, loopBB, mergeBB);

  m_builder.SetInsertPoint(loopBB);
  bool returnStatementFound = false;
  for (auto st : statement->getNodes()) {
    st->accept(this);
    if (m_has_return_statement) {
      returnStatementFound = true;
      break;
    }
  }

  if (!m_has_return_statement) {
    if (stepBB) {
      m_builder.CreateBr(stepBB);
      m_builder.SetInsertPoint(stepBB);
      astStep->accept(this);
    }
    m_builder.CreateBr(headBB);
    function->insert(function->end(), mergeBB);
    m_builder.SetInsertPoint(mergeBB);
  }
}

llvm::Value *IrGenerator::visit(AstFunctionAffectationExpression *expression) {
  m_debugInfo->emitLocation(expression->getLine());
  std::string name = RAL_RET_VALUE;
  auto variableSymbol = expression->getScope()->resolve(name);
  assert(variableSymbol);
  auto variableValue = variableSymbol->getValue();
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
