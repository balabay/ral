#include "Visitor.hpp"

#include <exceptions/NotImplementedException.hpp>
#include <exceptions/VariableNotFoundException.hpp>
#include <llvm/IR/Verifier.h>

using namespace antlr4;
using namespace RaLang;

Visitor::Visitor()
    : llvm_context(std::make_unique<llvm::LLVMContext>()),
      builder(*this->llvm_context),
      module(std::make_unique<llvm::Module>("output", *this->llvm_context)) {

  module->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
                        llvm::DEBUG_METADATA_VERSION);

  // Construct the DIBuilder
  debugBuilder = std::make_unique<llvm::DIBuilder>(*module);
}

void Visitor::emitLocation(antlr4::ParserRuleContext *node,
                           llvm::DICompileUnit *unit) {
  if (!node)
    return builder.SetCurrentDebugLocation(llvm::DebugLoc());
  llvm::DIScope *scope;
  if (LexicalBlocks.empty())
    scope = unit;
  else
    scope = LexicalBlocks.back();
  builder.SetCurrentDebugLocation(
      llvm::DILocation::get(scope->getContext(), node->getStart()->getLine(),
                            node->getStart()->getStartIndex(), scope));
}

llvm::DIType *Visitor::getDebugType() {
  if (debugInfo.itype)
    return debugInfo.itype;

  debugInfo.itype =
      debugBuilder->createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);
  return debugInfo.itype;
}

llvm::DISubroutineType *Visitor::createFunctionType(unsigned NumArgs) {
  llvm::SmallVector<llvm::Metadata *, 8> EltTys;
  llvm::DIType *intTy = getDebugType();

  // Add the result type.
  EltTys.push_back(intTy);

  for (unsigned i = 0, e = NumArgs; i != e; ++i)
    EltTys.push_back(intTy);

  return debugBuilder->createSubroutineType(
      debugBuilder->getOrCreateTypeArray(EltTys));
}

Scope &Visitor::currentScope() { return this->scopes.back(); }

llvm::AllocaInst *Visitor::getVariable(const std::string &name) {
  for (auto it = this->scopes.rbegin(); it != this->scopes.rend(); it++) {
    auto variable = it->getVariable(name);

    if (variable) {
      return variable;
    }
  }

  return nullptr;
}

llvm::Function *Visitor::printfPrototype() {
  auto printf_type = llvm::FunctionType::get(
      llvm::Type::getVoidTy(*this->llvm_context),
      {llvm::Type::getInt8PtrTy(*this->llvm_context)}, true);
  auto func = this->module->getOrInsertFunction(
      "printf", printf_type,
      llvm::AttributeList().get(*this->llvm_context, 1U,
                                llvm::Attribute::NoAlias));

  return llvm::cast<llvm::Function>(func.getCallee());
}

llvm::Function *Visitor::inputPrototype() {
  auto scanf_type = llvm::FunctionType::get(
      llvm::Type::getVoidTy(*this->llvm_context),
      {llvm::Type::getInt8PtrTy(*this->llvm_context)}, true);
  auto func = this->module->getOrInsertFunction(
      "scanf", scanf_type,
      llvm::AttributeList().get(*this->llvm_context, 1U,
                                llvm::Attribute::NoAlias));

  return llvm::cast<llvm::Function>(func.getCallee());
}

void Visitor::fromFile(const std::string &path) {
  std::ifstream stream;
  stream.open(path);

  debugInfo.unit = debugBuilder->createCompileUnit(
      llvm::dwarf::DW_LANG_C, debugBuilder->createFile(path, ""),
      "RAL Compiler", 1, "", 0);

  auto input = new ANTLRInputStream(stream);
  auto lexer = new RalLexer(input);
  auto tokens = new CommonTokenStream(lexer);
  auto parser = new RalParser(tokens);

  RalParser::ModuleContext *context = parser->module();
  this->visitModule(context);
  debugBuilder->finalize();
}

void Visitor::visitModule(RalParser::ModuleContext *context) {
  this->scopes.push_back(Scope(nullptr));
  std::vector<RalParser::FunctionContext *> functions = context->function();
  for (auto *f : functions) {
    this->visitFunction(f);
  }

  auto functionType = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(*this->llvm_context), {}, false);
  auto function = llvm::Function::Create(
      functionType, llvm::GlobalValue::LinkageTypes::ExternalLinkage, "main",
      this->module.get());
  auto block = llvm::BasicBlock::Create(builder.getContext());

  this->scopes.push_back(Scope(function));

  block->insertInto(function);
  this->builder.SetInsertPoint(block);

  // Main function is 1st function without arguments;
  RalParser::FunctionContext *main_alg = nullptr;
  for (auto *f : functions) {
    RalParser::FormalParametersContext *formal_parameters =
        f->formalParameters();
    std::vector<antlr4::tree::TerminalNode *> variable_names;
    if (formal_parameters) {
      variable_names = formal_parameters->VariableName();
    }
    if (variable_names.size() == 0) {
      main_alg = f;
      break;
    }
  }

  auto function_name = main_alg->VariableName()->getText();

  llvm::Function *CalleeF = module->getFunction(function_name);
  if (CalleeF == 0) {
    throw VariableNotFoundException("No function" + function_name);
  }
  auto fvalue = this->builder.CreateCall(CalleeF, {}, function_name);
  this->builder.CreateRet(llvm::ConstantInt::get(
      llvm::Type::getInt32Ty(*this->llvm_context), 0, true));

  this->scopes.pop_back(); // main
  this->scopes.pop_back();
}

static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::LLVMContext *TheContext,
                                                llvm::Function *TheFunction,
                                                llvm::StringRef VarName) {
  llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                         TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(llvm::Type::getInt32Ty(*TheContext), nullptr,
                           VarName);
}

void Visitor::visitFunction(RalParser::FunctionContext *functionContext) {
  RalParser::FormalParametersContext *formal_parameters =
      functionContext->formalParameters();
  std::vector<antlr4::tree::TerminalNode *> variable_names;
  if (formal_parameters) {
    variable_names = formal_parameters->VariableName();
  }

  std::vector<llvm::Type *> function_args(
      variable_names.size(), llvm::Type::getInt32Ty(builder.getContext()));

  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(builder.getContext()), function_args, false);

  // Get Name of the function
  auto function_name = functionContext->VariableName()->getText();
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, function_name, module.get());

  // If F conflicted, there was already something named 'Name'. If it has a
  // body, don't allow redefinition or reextern.
  if (F->getName() != function_name) {
    // Delete the one we just made and get the existing one.
    F->eraseFromParent();
    F = module->getFunction(function_name);
  }
  if (!F->empty()) {
    throw VariableNotFoundException("redefinition of function");
  }

  this->scopes.push_back(Scope(F));

  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BBF =
      llvm::BasicBlock::Create(builder.getContext(), "entry", F);
  builder.SetInsertPoint(BBF);

  // Create a subprogram DIE for this function.
  llvm::DIFile *Unit = debugBuilder->createFile(debugInfo.unit->getFilename(),
                                                debugInfo.unit->getDirectory());
  llvm::DIScope *FContext = Unit;
  unsigned LineNo = functionContext->getStart()->getLine();
  unsigned ScopeLine = LineNo;
  llvm::DISubprogram *SP = debugBuilder->createFunction(
      FContext, function_name, llvm::StringRef(), Unit, LineNo,
      createFunctionType(F->arg_size()), ScopeLine,
      llvm::DINode::FlagPrototyped, llvm::DISubprogram::SPFlagDefinition);
  F->setSubprogram(SP);

  // Push the current scope.
  LexicalBlocks.push_back(SP);

  // Unset the location for the prologue emission (leading instructions with no
  // location in a function are considered part of the prologue and the debugger
  // will run past them when breaking on a function)
  emitLocation(nullptr, debugInfo.unit);

  // Set names for all arguments.
  unsigned Idx = 0;
  for (llvm::Function::arg_iterator AI = F->arg_begin();
       Idx != variable_names.size(); ++AI, ++Idx) {
    auto name = variable_names[Idx]->getText();
    AI->setName(name);
    // Add arguments to variable symbol table.
    llvm::AllocaInst *Alloca =
        CreateEntryBlockAlloca(this->llvm_context.get(), F, name);
    // Create a debug descriptor for the variable.
    llvm::DILocalVariable *D = debugBuilder->createParameterVariable(
        SP, name, Idx, Unit, LineNo, getDebugType(), true);

    debugBuilder->insertDeclare(
        Alloca, D, debugBuilder->createExpression(),
        llvm::DILocation::get(SP->getContext(), LineNo, 0, SP),
        builder.GetInsertBlock());
    builder.CreateStore(AI, Alloca);
    this->currentScope().setVariable(name, Alloca);
  }

  emitLocation(functionContext->instructions(), debugInfo.unit);

  this->scopes.push_back(Scope(F));
  this->visitInstructions(functionContext->instructions());
  // Pop off the lexical block for the function.
  LexicalBlocks.pop_back();
  llvm::verifyFunction(*F);
  this->scopes.pop_back();
  this->scopes.pop_back();
}

void Visitor::visitInstructions(RalParser::InstructionsContext *context) {
  this->visitStatements(context->statement());
}

Visitor::Body Visitor::visitBody(RalParser::BodyContext *context,
                                 llvm::BasicBlock *afterBlock) {
  auto block = llvm::BasicBlock::Create(builder.getContext());

  this->builder.SetInsertPoint(block);
  auto function = this->currentScope().currentFunction;
  block->insertInto(function);

  this->scopes.push_back(Scope(function));

  llvm::Value *result = this->visitStatements(context->statement());

  this->scopes.pop_back();

  auto externalAfterBlock = afterBlock;

  if (!externalAfterBlock) {
    afterBlock = llvm::BasicBlock::Create(builder.getContext());
  }

  if (result == nullptr) {
    this->builder.CreateBr(afterBlock);
  }

  if (!externalAfterBlock) {
    this->builder.SetInsertPoint(afterBlock);
    afterBlock->insertInto(this->currentScope().currentFunction);
  }

  return Body{
      .mainBlock = block,
      .afterBlock = afterBlock,
  };
}

llvm::Value *Visitor::visitStatements(
    const std::vector<RalParser::StatementContext *> &statementContexts) {
  for (const auto &statementContext : statementContexts) {
    llvm::Value *value = this->visitStatement(statementContext);
    if (value)
      return value;
  }
  return nullptr;
}

llvm::Value *Visitor::visitStatement(RalParser::StatementContext *context) {
  if (auto variableDeclarationContext = context->variableDeclaration()) {
    this->visitVariableDeclaration(variableDeclarationContext);
  } else if (auto bodyContext = context->body()) {
    auto previousBlock = builder.GetInsertBlock();

    auto body = this->visitBody(bodyContext);

    builder.SetInsertPoint(previousBlock);
    builder.CreateBr(body.mainBlock);

    builder.SetInsertPoint(body.afterBlock);
  } else if (auto ifStatementContext = context->ifStatement()) {
    this->visitIfStatement(ifStatementContext);
  } else if (auto whileStatementContext = context->whileStatement()) {
    this->visitWhileStatement(whileStatementContext);
  } else if (auto printStatementContext = context->printStatement()) {
    this->visitPrintStatement(printStatementContext);
  } else if (auto inputStatementContext = context->inputStatement()) {
    this->visitInputStatement(inputStatementContext);
  } else if (auto expressionContext = context->expression()) {
    this->visitExpression(expressionContext);
  } else if (auto returnStatementContext = context->returnStatement()) {
    return this->visitReturnStatement(returnStatementContext);
  } else {
    throw NotImplementedException();
  }
  return nullptr;
}

void Visitor::visitVariableDeclaration(
    RalParser::VariableDeclarationContext *context) {
  emitLocation(context, debugInfo.unit);
  auto name = context->VariableName()->getText();
  auto expression = this->visitExpression(context->expression());
  auto type = expression->getType();

  auto alloca = builder.CreateAlloca(type, nullptr, name);

  builder.CreateStore(expression, alloca);

  this->currentScope().setVariable(name, alloca);
}

void Visitor::visitIfStatement(RalParser::IfStatementContext *context) {
  emitLocation(context, debugInfo.unit);
  auto expression = this->visitExpression(context->expression());
  auto type = expression->getType();

  auto previousBlock = builder.GetInsertBlock();

  auto body = this->visitBody(context->body());

  builder.SetInsertPoint(previousBlock);

  if (type->isIntegerTy(1)) {
    builder.CreateCondBr(expression, body.mainBlock, body.afterBlock);
  } else if (type->isIntegerTy()) {
    auto zero = llvm::ConstantInt::get(type, 0);
    auto condition = builder.CreateICmpNE(expression, zero);

    builder.CreateCondBr(condition, body.mainBlock, body.afterBlock);
  } else {
    throw NotImplementedException();
  }

  builder.SetInsertPoint(body.afterBlock);
}

void Visitor::visitWhileStatement(RalParser::WhileStatementContext *context) {
  emitLocation(context, debugInfo.unit);
  auto conditionBlock = llvm::BasicBlock::Create(builder.getContext());
  builder.CreateBr(conditionBlock);

  builder.SetInsertPoint(conditionBlock);
  conditionBlock->insertInto(this->currentScope().currentFunction);

  auto expression = this->visitExpression(context->expression());
  auto type = expression->getType();

  auto body = this->visitBody(context->body(), conditionBlock);

  auto afterBlock = llvm::BasicBlock::Create(builder.getContext());

  builder.SetInsertPoint(conditionBlock);

  if (type->isIntegerTy(1)) {
    builder.CreateCondBr(expression, body.mainBlock, afterBlock);
  } else if (type->isIntegerTy()) {
    auto zero = llvm::ConstantInt::get(type, 0);
    auto condition = builder.CreateICmpNE(expression, zero);

    builder.CreateCondBr(condition, body.mainBlock, afterBlock);
  } else {
    throw NotImplementedException();
  }

  builder.SetInsertPoint(afterBlock);
  afterBlock->insertInto(this->currentScope().currentFunction);
}

llvm::Value *
Visitor::visitReturnStatement(RalParser::ReturnStatementContext *context) {
  RalParser::ExpressionContext *expr_context = context->expression();
  if (!expr_context) {
    return builder.CreateRetVoid();
  }

  auto *value = visitExpression(expr_context);
  return builder.CreateRet(value);
}

llvm::Value *Visitor::visitExpression(RalParser::ExpressionContext *context) {
  if (auto inParenExpressionContext =
          dynamic_cast<RalParser::InParenExpressionContext *>(context)) {
    return this->visitExpression(inParenExpressionContext->expression());
  } else if (auto unaryNegativeExpressionContext =
                 dynamic_cast<RalParser::UnaryNegativeExpressionContext *>(
                     context)) {
    return this->visitUnaryNegativeExpression(unaryNegativeExpressionContext);
  } else if (auto nameExpressionContext =
                 dynamic_cast<RalParser::NameExpressionContext *>(context)) {
    return this->visitNameExpression(nameExpressionContext);
  } else if (auto functionCallExpressionContext =
                 dynamic_cast<RalParser::FunctionCallExpressionContext *>(
                     context)) {
    return this->visitFunctionCallExpression(functionCallExpressionContext);
  } else if (auto binaryOperationContext =
                 dynamic_cast<RalParser::BinaryOperationContext *>(context)) {
    return this->visitBinaryOperation(binaryOperationContext);
  } else if (auto binaryMultiplyOperationContext =
                 dynamic_cast<RalParser::BinaryMultiplyOperationContext *>(
                     context)) {
    return this->visitBinaryMultiplyOperation(binaryMultiplyOperationContext);
  } else if (auto binaryConditionalOperationContext =
                 dynamic_cast<RalParser::BinaryConditionalOperationContext *>(
                     context)) {
    return this->visitBinaryConditionalOperation(
        binaryConditionalOperationContext);
  } else if (auto variableAffectationContext =
                 dynamic_cast<RalParser::VariableAffectationContext *>(
                     context)) {
    return this->visitVariableAffectation(variableAffectationContext);
  } else if (auto literalContext =
                 dynamic_cast<RalParser::LiteralExpressionContext *>(context)) {
    return this->visitLiteral(literalContext->literal());
  }

  throw NotImplementedException();
}

llvm::Value *Visitor::visitUnaryNegativeExpression(
    RalParser::UnaryNegativeExpressionContext *context) {
  auto expression = this->visitExpression(context->expression());
  auto type = expression->getType();
  emitLocation(context, debugInfo.unit);

  if (type->isIntegerTy()) {
    auto zero = llvm::ConstantInt::get(type, 0);
    return builder.CreateSub(zero, expression);
  }

  throw NotImplementedException();
}

llvm::Value *
Visitor::visitNameExpression(RalParser::NameExpressionContext *context) {
  auto name = context->VariableName()->getText();
  auto variable = this->getVariable(name);

  if (!variable) {
    throw VariableNotFoundException(name);
  }
  emitLocation(context, debugInfo.unit);
  return this->builder.CreateLoad(variable->getAllocatedType(),
                                  variable);
}

llvm::Value *Visitor::visitFunctionCallExpression(
    RalParser::FunctionCallExpressionContext *context) {
  emitLocation(context, debugInfo.unit);
  RalParser::FunctionCallContext *functionCallC = context->functionCall();
  auto name = functionCallC->VariableName()->getText();
  llvm::Function *CalleeF = module->getFunction(name);

  if (!CalleeF) {
    throw VariableNotFoundException(name);
  }

  std::vector<RalParser::ExpressionContext *> Args;
  if (functionCallC->args()) {
    Args = functionCallC->args()->expression();
  }

  if (CalleeF->arg_size() != Args.size()) {
    throw VariableNotFoundException("Incorrect # arguments passed");
  }

  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(visitExpression(Args[i]));
    if (ArgsV.back() == 0) {
      throw VariableNotFoundException("Incorrect argument");
    }
  }
  return this->builder.CreateCall(CalleeF, ArgsV, name);
}

llvm::Value *
Visitor::visitBinaryOperation(RalParser::BinaryOperationContext *context) {
  emitLocation(context, debugInfo.unit);
  auto leftExpression = this->visitExpression(context->expression(0));
  auto rightExpression = this->visitExpression(context->expression(1));

  if (context->Add()) {
    return builder.CreateAdd(leftExpression, rightExpression);
  } else if (context->Sub()) {
    return builder.CreateSub(leftExpression, rightExpression);
  }

  throw NotImplementedException();
}

llvm::Value *Visitor::visitBinaryMultiplyOperation(
    RalParser::BinaryMultiplyOperationContext *context) {
  emitLocation(context, debugInfo.unit);
  auto leftExpression = this->visitExpression(context->expression(0));
  auto rightExpression = this->visitExpression(context->expression(1));

  if (context->Mul()) {
    return builder.CreateMul(leftExpression, rightExpression);
  } else if (context->Div()) {
    return builder.CreateSDiv(leftExpression, rightExpression);
  } else if (context->Mod()) {
    return builder.CreateSRem(leftExpression, rightExpression);
  }

  throw NotImplementedException();
}

llvm::Value *Visitor::visitBinaryConditionalOperation(
    RalParser::BinaryConditionalOperationContext *context) {
  emitLocation(context, debugInfo.unit);
  auto leftExpression = this->visitExpression(context->expression(0));
  auto rightExpression = this->visitExpression(context->expression(1));

  if (context->Gt()) {
    return builder.CreateICmpSGT(leftExpression, rightExpression);
  } else if (context->Gte()) {
    return builder.CreateICmpSGE(leftExpression, rightExpression);
  } else if (context->Lt()) {
    return builder.CreateICmpSLT(leftExpression, rightExpression);
  } else if (context->Lte()) {
    return builder.CreateICmpSLE(leftExpression, rightExpression);
  } else if (context->Eq()) {
    return builder.CreateICmpEQ(leftExpression, rightExpression);
  } else if (context->Ne()) {
    return builder.CreateICmpNE(leftExpression, rightExpression);
  }

  throw NotImplementedException();
}

llvm::Value *Visitor::visitVariableAffectation(
    RalParser::VariableAffectationContext *context) {
  auto name = context->VariableName()->toString();
  auto variable = this->getVariable(name);

  if (!variable) {
    throw VariableNotFoundException(name);
  }

  auto expression = this->visitExpression(context->expression());
  this->builder.CreateStore(expression, variable);

  return this->builder.CreateLoad(variable->getAllocatedType(),
                                  variable);
}

llvm::Value *Visitor::visitLiteral(RalParser::LiteralContext *context) {
  if (auto integerLiteralContext = context->integerLiteral()) {
    return this->visitIntegerLiteral(integerLiteralContext);
  }

  throw NotImplementedException();
}

llvm::Value *
Visitor::visitIntegerLiteral(RalParser::IntegerLiteralContext *context) {
  emitLocation(context, debugInfo.unit);
  if (auto zeroLiteralContext = context->ZeroLiteral()) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*this->llvm_context),
                                  0, true);
  } else if (auto decimalLiteralContext = context->DecimalLiteral()) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*this->llvm_context),
                                  std::stol(decimalLiteralContext->getText()),
                                  true);
  } else if (auto hexadecimalLiteralContext = context->HexadecimalLiteral()) {
    return llvm::ConstantInt::get(
        llvm::Type::getInt32Ty(*this->llvm_context),
        std::stol(hexadecimalLiteralContext->getText(), nullptr, 16), true);
  } else if (auto binaryLiteralContext = context->BinaryLiteral()) {
    auto value = binaryLiteralContext->getText();

    // Remove "0x"
    value.erase(0, 2);

    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*this->llvm_context),
                                  std::stol(value, nullptr, 2), true);
  }

  throw NotImplementedException();
}

void Visitor::visitInputStatement(RalParser::InputStatementContext *context) {
  auto *var_g = context->VariableName();
  llvm::AllocaInst *var_c = getVariable(var_g->getText());
  if (var_c == nullptr) {
    throw VariableNotFoundException(var_g->getText());
  }
  /*set up scanf arguments*/
  llvm::Value *scanfFormat = builder.CreateGlobalStringPtr("%u");
  llvm::AllocaInst *Alloca = var_c;
  std::vector<llvm::Value *> scanfArgs = {scanfFormat, Alloca};

  llvm::Function *theScanf = inputPrototype();
  builder.CreateCall(theScanf, scanfArgs);
}

void Visitor::visitPrintStatement(RalParser::PrintStatementContext *context) {
  std::vector<std::string> formats;
  std::vector<llvm::Value *> args;

  for (auto &expressionContext : context->expression()) {
    auto value = this->visitExpression(expressionContext);
    auto type = value->getType();

    args.push_back(value);

    if (type->isIntegerTy()) {
      formats.push_back("%ld");
    } else {
      throw NotImplementedException();
    }
  }

  std::ostringstream format;
  std::copy(formats.begin(), formats.end(),
            std::ostream_iterator<std::string>(format, " "));
  auto formatString = format.str() + '\n';
  auto global = builder.CreateGlobalStringPtr(formatString.c_str());
  args.insert(args.begin(), global);

  auto function = this->printfPrototype();

  builder.CreateCall(function, args);
}

llvm::Type *Visitor::visitType(RalParser::TypeContext *context) {
  auto name = context->VariableName()->getText();

  if (name == "i32") {
    return llvm::Type::getInt32Ty(*this->llvm_context);
  }

  throw NotImplementedException();
}
