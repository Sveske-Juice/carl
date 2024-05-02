#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>

class Expression;
class BinaryExpression;
class LiteralExpression;

class AstPrinter {
    public:
        std::string print(Expression& expression);
        std::string visitLiteralExpression(LiteralExpression& expression);
        std::string visitBinaryExpression(BinaryExpression& expression);
};

#endif
