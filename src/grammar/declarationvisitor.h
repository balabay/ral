#pragma once

#include "runtime/RalParserBaseVisitor.h"

#include <llvm/IR/IRBuilder.h>

namespace llvm {
class Module;
class LLVMContext;
} // namespace llvm

namespace RaLang {

class SymbolTable;
class DeclarationVisitor : public RalParserBaseVisitor {
public:
  DeclarationVisitor(SymbolTable &symbolTable);
  std::any visitAlgorithmPrototype(RalParser::AlgorithmPrototypeContext *ctx) override;
  std::any visitFormalParameters(RalParser::FormalParametersContext *ctx) override;

private:
  SymbolTable &m_symbolTable;
};

} // namespace RaLang
