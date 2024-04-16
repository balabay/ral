#include "compiler.h"

#include "grammar/ast.h"
#include "grammar/astbuildervisitor.h"
#include "grammar/declarationvisitor.h"
#include "grammar/runtime/RalLexer.h"
#include "grammar/runtime/RalParser.h"
#include "logic/symboltable.h"

#include <cassert>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace RaLang {

Compiler::Compiler() : m_emitDebugInfo(true) {}

Compiler::~Compiler() = default;

void Compiler::compile() {
  for (std::string const &file : m_sources) {
    std::cerr << "Compiling file:" << file << "\n";
    std::ifstream stream;
    stream.open(file);

    antlr4::ANTLRInputStream input(stream);
    RalLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    RalParser parser(&tokens);

    RalParser::ModuleContext *tree = parser.module();

    SymbolTable symbolTable;
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder(llvmContext);
    llvm::Module module(file, llvmContext);

    // First Pass parses declarations
    DeclarationVisitor declarationVisitor(symbolTable);
    declarationVisitor.visit(tree);
    std::cerr << symbolTable.dump();
    // symbolTable->resolve();

    // 2nd Pass builds internal AST from ANTLR AST
    Ast ast;
    AstBuilderVisitor astBuilderVisitor(file, symbolTable, ast);
    astBuilderVisitor.visit(tree);
    std::cerr << ast.dump();

    //        // Code Generation
    //        RaLang::Visitor visitor(m_emitDebugInfo, file, symbolTable,
    //        llvmContext, builder, module); visitor.visitModule(tree);
    //        module.print(llvm::outs(), nullptr);
    //        std::cout << std::endl;
  }
}

void Compiler::addSource(const char *fileName) {
  assert(fileName);
  m_sources.push_back(fileName);
}

void Compiler::setEmitDebugInfo(bool enable) { m_emitDebugInfo = enable; }
} // namespace RaLang
