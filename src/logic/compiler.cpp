#include "compiler.h"

#include "grammar/declarationvisitor.h"
#include "grammar/runtime/RalLexer.h"
#include "grammar/runtime/RalParser.h"
#include "grammar/Visitor.hpp"
#include "logic/symboltable.h"

#include <cassert>

namespace RaLang {

Compiler::Compiler() :m_emitDebugInfo(true){
}

Compiler::~Compiler() = default;

void Compiler::compile()
{
    for (std::string const& file: m_sources) {
        std::cerr << "Compiling file:" << file << "\n";
        std::ifstream stream;
        stream.open(file);

        antlr4::ANTLRInputStream input(stream);
        RalLexer  lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);

        RalParser parser(&tokens);

        RalParser::ModuleContext *tree = parser.module();

        // First Pass parse declarations
        DeclarationVisitor declarationVisitor(m_symbolTable.get());
        declarationVisitor.visit(tree);
        //symbolTable->dump();
        //symbolTable->resolve();

        // Code Generation
        RaLang::Visitor visitor(m_emitDebugInfo, file);
        visitor.visitModule(tree);
        visitor.module->print(llvm::outs(), nullptr);
        std::cout << std::endl;
    }
}

void Compiler::addSource(const char *fileName)
{
    assert(fileName);
    m_sources.push_back(fileName);
}

void Compiler::setEmitDebugInfo(bool enable) { m_emitDebugInfo = enable; }
} // namespace RaLang
