#ifndef IEXPRESSION_VISITOR_H
#define IEXPRESSION_VISITOR_H

class Expression;
class BinaryExpression;
class LiteralExpression;
class UnaryExpression;

class IExpressionVisitor {
    public:
        virtual void visitLiteralExpression(LiteralExpression& expression) = 0;
        virtual void visitBinaryExpression(BinaryExpression& expression) = 0;
        virtual void visitUnaryExpression(UnaryExpression& expression) = 0;
};

#endif
