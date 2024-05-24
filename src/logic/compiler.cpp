#include "compiler.h"

#include "grammar/ast.h"
#include "grammar/astbuildervisitor.h"
#include "grammar/declarationvisitor.h"
#include "grammar/irdeclarationgenerator.h"
#include "grammar/irgenerator.h"
#include "grammar/parsererrorlistener.h"
#include "grammar/runtime/RalLexer.h"
#include "grammar/runtime/RalParser.h"
#include "grammar/typecheckvisitor.h"
#include "logic/symboltable.h"
#include "ralexceptions.h"

#include <cassert>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace RaLang {

Compiler::Compiler() : m_emitDebugInfo(false) {}

Compiler::~Compiler() = default;

void Compiler::compile() {
  for (std::string const &file : m_sources) {
    std::cerr << "Compiling file:" << file << "\n";
    std::ifstream stream;
    stream.open(file);
    if (!stream) {
      throw FileNotFoundException(file);
    }

    antlr4::ANTLRInputStream input(stream);
    RalLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    RalParser parser(&tokens);
    parser.removeErrorListeners();

    ParserErrorListener errorListener;
    parser.addErrorListener(&errorListener);

    RalParser::ModuleContext *tree = parser.module();

    SymbolTable symbolTable;
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder(llvmContext);
    llvm::Module module(file, llvmContext);

    // First Pass parses declarations
    DeclarationVisitor declarationVisitor(symbolTable);
    declarationVisitor.visit(tree);
    std::cerr << "Symbol Table\n\n" << symbolTable.dump() << std::endl;

    // 2nd Pass builds internal AST from ANTLR AST
    Ast ast;
    AstBuilderVisitor astBuilderVisitor(file, symbolTable, ast);
    astBuilderVisitor.visit(tree);
    // std::cerr << "AST\n\n" << ast.dump() << std::endl;

    // 3rd Pass - semantic analysis and type checking
    TypeCheckVisitor typeCheckVisitor(file, symbolTable, ast);
    typeCheckVisitor.visit();
    std::cerr << "AST\n\n" << ast.dump() << std::endl;

    // Code Generation
    //    symbolTable.removeSubScopes();

    // Generate Declarations (llvm::Function* should be created
    // before body of other functions - to be able to call the
    // function.
    // This language does not require forward declarations of the functions
    RaLang::IrDeclarationGenerator declarationGenerator(m_emitDebugInfo, file, symbolTable, llvmContext, builder,
                                                        module);
    declarationGenerator.visit(ast);
    declarationGenerator.initStandardFunctions();

    RaLang::IrGenerator generator(m_emitDebugInfo, file, symbolTable, llvmContext, builder, module);
    generator.visit(ast);
    module.print(llvm::outs(), nullptr);
    std::cout << std::endl;
  }
}

void Compiler::addSource(const char *fileName) {
  assert(fileName);
  m_sources.push_back(fileName);
}

void Compiler::setEmitDebugInfo(bool enable) { m_emitDebugInfo = enable; }
} // namespace RaLang
