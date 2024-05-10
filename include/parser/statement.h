#ifndef STATEMENT_H
#define STATEMENT_H

#include "parser/expression.h"
#include "parser/istatement_visitor.h"
#include <memory>

class Statement {
public:
    virtual void accept(IStatementVisitor &visitor) = 0;
};

class ExpressionStatement : public Statement {
private:
    std::unique_ptr<Expression> expression_;

public:
    ExpressionStatement(std::unique_ptr<Expression> expression)
        : expression_{std::move(expression)} {}
    virtual void accept(IStatementVisitor &visitor) override {
        visitor.visitExpressionStatement(*this);
    }
    Expression &expression() { return *expression_; }
};

class DefineStatement : public Statement {
private:
    const std::string ruleName_;
    std::unique_ptr<Expression> pattern_;
    std::unique_ptr<Expression> replacement_;

public:
    DefineStatement(std::string ruleName, std::unique_ptr<Expression> pattern,
                    std::unique_ptr<Expression> replacement)
        : ruleName_{ruleName}, pattern_{std::move(pattern)},
          replacement_{std::move(replacement)} {}

    virtual void accept(IStatementVisitor &visitor) override {
        visitor.visitDefineStatement(*this);
    }

    std::string ruleName() const { return ruleName_; }
    Expression &pattern() const { return *pattern_.get(); }
    Expression &replacement() const { return *replacement_.get(); }
};

#endif
