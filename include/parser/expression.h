#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lexer/token.h"
#include "parser/ast_printer.h"

#include <fmt/core.h>
#include <string>

class Expression {
    public:
        virtual std::string toString() const = 0;
        /* NOTE: This should probably accept any IExpressionVisitor,
         * so we dont need to add a new overload foreach new visitor created.
         * I just dont know enough templating to get this to work with also having
         * a generic return type. This clearly violates seperation of concerns
         */
        virtual std::string accept(AstPrinter& visitor) = 0;
};

// Concrete expressions
class LiteralExpression : public Expression {
    private:
        const std::string literal_;

    public:
        LiteralExpression(std::string literal) : literal_{std::move(literal)} {}
        std::string accept(AstPrinter& visitor) override {
            return visitor.visitLiteralExpression(*this);
        }
        std::string toString() const override { return literal_; }
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
        std::string accept(AstPrinter& visitor) override {
            return visitor.visitBinaryExpression(*this);
        }
        std::string toString() const override {
            return fmt::format("{} {} {}", left_.toString(), op_.literal(), right_.toString());
        }
        Expression& left() { return left_; }
        Token& op() { return op_; }
        Expression& right() { return right_; }
};

#endif
