#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "parser/iexpression_visitor.h"

#include <stack>
#include <string>

class AstPrinter : public IExpressionVisitor {
private:
    std::stack<std::string> workingStack;

public:
    std::string print(Expression &expression);
    void visitLiteralExpression(LiteralExpression &expression) override;
    void visitBinaryExpression(BinaryExpression &expression) override;
    void visitUnaryExpression(UnaryExpression &expression) override;
};

#endif
