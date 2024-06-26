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
    std::unique_ptr<Expression> borrowPattern() { return std::move(pattern_); }
    std::unique_ptr<Expression> borrowReplacement() {return std::move(replacement_); }
};

class ApplyStatement : public Statement {
private:
    std::string ruleName_;
    std::unique_ptr<Expression> expression_;

public:
    ApplyStatement(std::string ruleName, std::unique_ptr<Expression> expression)
        : ruleName_{ruleName}, expression_{std::move(expression)} {}

    virtual void accept(IStatementVisitor& visitor) override {
        visitor.visitApplyStatement(*this);
    }

    std::string ruleName() const { return ruleName_; }
    std::unique_ptr<Expression> borrowExpression() { return std::move(expression_); }
};

class ShowStatement : public Statement {
    private:
        std::unique_ptr<Expression> expression_;
    public:
        ShowStatement(std::unique_ptr<Expression> expression) : expression_{std::move(expression)} {}

        virtual void accept(IStatementVisitor &visitor) override {
            visitor.visitShowStatement(*this);
        }

        Expression &expression() const { return *expression_.get(); }
};

#endif
