#include "parser/ast_printer.h"
#include "parser/expression.h"

#include <fmt/core.h>
#include <string>

std::string AstPrinter::print(Expression &expression) {
    return expression.accept(*this);
}

std::string AstPrinter::visitLiteralExpression(LiteralExpression &expression) {
    return expression.toString();
}

std::string AstPrinter::visitBinaryExpression(BinaryExpression &expression) {
    return fmt::format("({} {} {})", expression.left().accept(*this),
                       expression.op().literal(),
                       expression.right().accept(*this));
}
