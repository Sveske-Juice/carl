#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lexer/token.h"
#include "parser/iexpression_visitor.h"

#include <fmt/core.h>
#include <memory>
#include <string>

class Expression {
    public:
        virtual void accept(IExpressionVisitor& visitor) = 0;
};

// Concrete expressions
class LiteralExpression : public Expression {
    private:
        const TokenType literalType_; // Can be NUMBER, STRING, TRUE, FALSE, NULL
        const std::string literal_;

    public:
        LiteralExpression(TokenType literalType, std::string literal)
            : literalType_(literalType), literal_(std::move(literal)) {}
        void accept(IExpressionVisitor &visitor) override {
            visitor.visitLiteralExpression(*this);
        }
        std::string literal() const { return literal_; }
        TokenType type() const { return literalType_; }
};

class BinaryExpression : public Expression {
    private:
        std::unique_ptr<Expression> left_;
        Token op_;
        std::unique_ptr<Expression> right_;

    public:
        BinaryExpression(std::unique_ptr<Expression> left, Token token,
                         std::unique_ptr<Expression> right)
            : left_(std::move(left)), op_(std::move(token)), right_(std::move(right)) {}

        void accept(IExpressionVisitor& visitor) override {
            visitor.visitBinaryExpression(*this);
        }

        Expression& left() { return *left_; }
        Token& op() { return op_; }
        Expression& right() { return *right_; }
};

class UnaryExpression : public Expression {
    private:
        Token op_;
        std::unique_ptr<Expression> operand_;

    public:
        UnaryExpression(Token op,
                         std::unique_ptr<Expression> right)
            : op_(std::move(op)), operand_(std::move(right)) {}

        void accept(IExpressionVisitor& visitor) override {
            visitor.visitUnaryExpression(*this);
        }

        Token& op() { return op_; }
        Expression& operand() { return *operand_; }
};

#endif
