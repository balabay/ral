#include "generatorbasevisitor.h"

namespace RaLang {

GeneratorBaseVisitor::GeneratorBaseVisitor()
{

}

void GeneratorBaseVisitor::visit(Ast &ast)
{
    for(auto & module : ast.getNodes())
    {
        module->accept(this);
    }
}

llvm::Value *GeneratorBaseVisitor::visit(AstAlgorithm *algorithm)
{
    return visitNodes(algorithm);
}

llvm::Value *GeneratorBaseVisitor::visit(AstAlgorithmCallExpression *algorithmCall)
{
    return visitNodes(algorithmCall);
}

llvm::Value *GeneratorBaseVisitor::visit(AstExpressionStatement *expressionStatement)
{
    return visitNodes(expressionStatement);
}

llvm::Value *GeneratorBaseVisitor::visit(AstIntExpression *expression)
{
    return visitNodes(expression);
}

llvm::Value *GeneratorBaseVisitor::visit(AstModule *module)
{
    return visitNodes(module);
}

llvm::Value *GeneratorBaseVisitor::visit(AstReturnStatement *returnStatement)
{
    return visitNodes(returnStatement);
}

llvm::Value *GeneratorBaseVisitor::visitNodes(AstNode *n)
{
    llvm::Value *result = nullptr;
    for(auto node : n->getNodes())
    {
        result = node->accept(this);
    }
    return result;
}

} // namespace RaLang
