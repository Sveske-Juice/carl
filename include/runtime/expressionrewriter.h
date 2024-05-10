#ifndef EXPRESSIONREWRITER_H
#define EXPRESSIONREWRITER_H

#include "parser/iexpression_visitor.h"
#include "parser/expression.h"

#include <memory>
#include <vector>
#include <unordered_map>

enum ExpressionRewriterState {
    PROCCESING_PATTERN,
    PROCESSING_SOURCE,
    RESOLVING_REPLACEMNT
};

class ExpressionRewriter : public IExpressionVisitor {
    private:
        std::unordered_map<std::string, Expression*> symbolMap;
        ExpressionRewriterState state{PROCCESING_PATTERN};
        std::vector<Expression*> patternPostfixStack;
        std::vector<Expression*> sourcePostfixStack;
        std::vector<Expression*> replacementPostfixStack;

        // Parent of the part of rootSource to replace with replacement
        Expression* replacementParent{nullptr};
        Expression* replacementNode{nullptr};
        std::unique_ptr<Expression> replacement;
        bool match{false};

        void checkForMatch();
        void resolveReplacement();
        void push(Expression& expression);

    public:
        std::unique_ptr<Expression> substitute(std::unique_ptr<Expression> rootSource, std::unique_ptr<Expression> pattern, std::unique_ptr<Expression> _replacement);
        virtual void visitLiteralExpression(LiteralExpression& expression) override;
        virtual void visitBinaryExpression(BinaryExpression& expression) override;
        virtual void visitUnaryExpression(UnaryExpression& expression) override;

};

#endif
