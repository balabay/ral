#pragma once
#include "logic/symboltable.h"
#include "runtime/RalParserBaseVisitor.h"

namespace RaLang {

class SymbolTable;
class DeclarationVisitor : public RalParserBaseVisitor
{
public:
    DeclarationVisitor(SymbolTable* symbolTable);

private:
    SymbolTable* m_symbolTable;
};

} // namespace RaLang
