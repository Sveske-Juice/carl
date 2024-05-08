#ifndef STATEMENT_H
#define STATEMENT_H

#include "parser/expression.h"
#include "parser/istatement_visitor.h"
#include <memory>

class Statement {
    public:
        virtual void accept(IStatementVisitor& visitor) = 0;
};

class ExpressionStatement : public Statement {
    private:
        std::unique_ptr<Expression> expression_;

    public:
        ExpressionStatement(std::unique_ptr<Expression> expression) : expression_{std::move(expression)} {}
        virtual void accept(IStatementVisitor& visitor) override {
            visitor.visitExpressionStatement(*this);
        }
        Expression& expression() { return *expression_; }
};

#endif
