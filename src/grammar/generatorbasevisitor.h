#pragma once

#include "grammar/ast.h"
namespace RaLang {

class GeneratorBaseVisitor : public GeneratorVisitor
{
public:
    GeneratorBaseVisitor();

    // GeneratorVisitor interface
public:
    void visit(Ast& ast);
    llvm::Value *visit(AstAlgorithm *algorithm);
    llvm::Value *visit(AstAlgorithmCallExpression *algorithmCall);
    llvm::Value *visit(AstExpressionStatement *expressionStatement);
    llvm::Value *visit(AstModule *module);
    llvm::Value *visit(AstReturnStatement *returnStatement);

protected:
    llvm::Value *visitNodes(AstNode* n);
};

} // namespace RaLang
