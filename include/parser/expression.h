#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lexer/token.h"
#include "parser/iexpression_visitor.h"

#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <memory>
#include <string>

enum ExpressionType {
    // Literals
    EXPR_STRING,
    EXPR_NUMBER,

    // Binary
    EXPR_PLUS,
    EXPR_MINUS,
    EXPR_MULTIPLICATION,
    EXPR_DIVISION,
    EXPR_MODULU,

    // Unary
    EXPR_NEGATE,
};

static const char * expressionTypeToString[] = {
    "String",
    "Number",

    "Plus",
    "Minus",
    "Multiplication",
    "Division",
    "Modulus",

    "Negate",
};

class Expression {
    public:
        Expression* parent{nullptr};
        virtual ExpressionType expressionType() const = 0;
        virtual void accept(IExpressionVisitor& visitor) = 0;
        virtual std::string toString() const = 0;
        virtual std::unique_ptr<Expression> clone() const = 0;
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
        virtual ExpressionType expressionType() const override {
            switch (literalType_) {
                case STRING:
                    return EXPR_STRING;
                case NUMBER:
                      return EXPR_NUMBER;
                default:
                      throw std::exception(); // TODO: handle
            }
        }
        virtual std::unique_ptr<Expression> clone() const override {
            return std::make_unique<LiteralExpression>(*this);
        }
        std::string literal() const { return literal_; }
        TokenType type() const { return literalType_; }
        virtual std::string toString() const override { return literal(); }
};

class BinaryExpression : public Expression {
    private:
        std::unique_ptr<Expression> left_;
        Token op_;
        std::unique_ptr<Expression> right_;

    public:
        BinaryExpression(std::unique_ptr<Expression> left, Token token,
                         std::unique_ptr<Expression> right)
            : left_(std::move(left)), op_(std::move(token)), right_(std::move(right)) {
                left_->parent = this;
                right_->parent = this;
            }
        virtual std::string toString() const override { return "(" + left_->toString() + op_.literal() + right_->toString() + ")"; }

        void accept(IExpressionVisitor& visitor) override {
            visitor.visitBinaryExpression(*this);
        }

        virtual ExpressionType expressionType() const override {
            switch (op_.type()) {
                case PLUS:
                    return EXPR_PLUS;
                case MINUS:
                    return EXPR_MINUS;
                case STAR:
                    return EXPR_MULTIPLICATION;
                case SLASH:
                    return EXPR_DIVISION;
                case MODULUS:
                    return EXPR_MODULU;
                default:
                    throw std::exception(); // TODO: handle
            }
        }

        virtual std::unique_ptr<Expression> clone() const override {
            auto leftCopy = left_->clone();
            auto rightCopy = right_->clone();
            return std::make_unique<BinaryExpression>(std::move(leftCopy), op_, std::move(rightCopy));
        }

        Expression& left() { return *left_; }
        Token& op() { return op_; }
        Expression& right() { return *right_; }

        void setLeft(std::unique_ptr<Expression> value) { left_ = std::move(value); left_->parent = this; }
        void setRight(std::unique_ptr<Expression> value) { right_ = std::move(value); right_->parent = this; }
};

class UnaryExpression : public Expression {
    private:
        Token op_;
        std::unique_ptr<Expression> operand_;

    public:
        UnaryExpression(Token op,
                         std::unique_ptr<Expression> right)
            : op_(std::move(op)), operand_(std::move(right)) {
                operand_->parent = this;
            }

        void accept(IExpressionVisitor& visitor) override {
            visitor.visitUnaryExpression(*this);
        }

        virtual ExpressionType expressionType() const override {
            switch (op_.type()) {
                case MINUS:
                    return EXPR_NEGATE;
                default:
                    throw std::exception(); // TODO: handle
            }
        }

        virtual std::unique_ptr<Expression> clone() const override {
            auto operandCopy = operand_->clone();
            return std::make_unique<UnaryExpression>(op_, std::move(operandCopy));
        }

        Token& op() { return op_; }
        Expression& operand() { return *operand_; }
        void setOperand(std::unique_ptr<Expression> value) { operand_ = std::move(value); operand_->parent = this; }
        virtual std::string toString() const override { return op_.literal() + operand_->toString(); }
};

#endif
