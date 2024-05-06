#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lexer/token.h"
#include "parser/iexpression_visitor.h"

#include <fmt/core.h>
#include <string>

class Expression {
    public:
        virtual void accept(IExpressionVisitor& visitor) = 0;
};

// Concrete expressions
class LiteralExpression : public Expression {
    private:
        const std::string literal_;

    public:
        LiteralExpression(std::string literal) : literal_{std::move(literal)} {}
        void accept(IExpressionVisitor& visitor) override {
            visitor.visitLiteralExpression(*this);
        }
        std::string literal() const { return literal_; }
};

class BinaryExpression : public Expression {
    private:
        Expression& left_;
        Token op_;
        Expression& right_;

    public:
        BinaryExpression(Expression &left, Token token,
                         Expression &right)
            : left_(left), op_(std::move(token)), right_(right) {}

        void accept(IExpressionVisitor& visitor) override {
            visitor.visitBinaryExpression(*this);
        }

        Expression& left() { return left_; }
        Token& op() { return op_; }
        Expression& right() { return right_; }
};

#endif
