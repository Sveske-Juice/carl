#include "parser/ast_printer.h"
#include "parser/expression.h"

#include <cassert>
#include <fmt/core.h>
#include <string>

std::string AstPrinter::print(Expression &expression) {
    expression.accept(*this);
    assert(workingStack.size() == 1);
    return workingStack.top();
}

void AstPrinter::visitLiteralExpression(LiteralExpression &expression) {
    workingStack.push(expression.literal());
}

void AstPrinter::visitBinaryExpression(BinaryExpression &expression) {
    expression.left().accept(*this);
    std::string l = workingStack.top();
    workingStack.pop();

    expression.right().accept(*this);
    std::string r = workingStack.top();
    workingStack.pop();

    workingStack.push(fmt::format("({} {} {})", l, expression.op().literal(), r));
}

void AstPrinter::visitUnaryExpression(UnaryExpression &expression) {
    expression.operand().accept(*this);
    std::string o = workingStack.top();
    workingStack.pop();

    workingStack.push(fmt::format("{}{}", expression.op().literal(), o));
}
