#include "runtime/expressionrewriter.h"
#include "lexer/token.h"
#include "parser/ast_printer.h"
#include "parser/expression.h"
#include <cassert>
#include <exception>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <vector>

std::unique_ptr<Expression>
ExpressionRewriter::substitute(std::unique_ptr<Expression> rootSource,
                               std::unique_ptr<Expression> pattern,
                               std::unique_ptr<Expression> replacement_) {

    this->replacement = std::move(replacement_);

    // Create reverse polish notation sequence for the pattern to search for
    state = PROCCESING_PATTERN;
    pattern->accept(*this);

    state = PROCESSING_SOURCE;

    rootSource->accept(*this);

    // No substitution needed when no matches are found
    if (!match)
        return rootSource;

    state = RESOLVING_REPLACEMNT;

    // Resolve all symbols in this->replacement to the corresponding found in the
    // source
    this->replacement->accept(*this);

    if (replacementParent == nullptr)
        return std::move(this->replacement);

    switch (replacementParent->expressionType()) {
    case EXPR_PLUS:
    case EXPR_MINUS:
    case EXPR_MULTIPLICATION:
    case EXPR_DIVISION:
    case EXPR_MODULU: {
        BinaryExpression *parent =
            static_cast<BinaryExpression *>(replacementParent);

        // Figure out which side of operator the nodeToReplace is on
        bool rhs = &parent->right() == replacementNode ? true : false;
        if (rhs)
            parent->setRight(std::move(this->replacement));
        else
            parent->setLeft(std::move(this->replacement));

        break;
    }
    case EXPR_NEGATE: {
        UnaryExpression *parent =
            static_cast<UnaryExpression *>(replacementParent);

        parent->setOperand(std::move(this->replacement));
        break;
    }

    default:
        // Should no be possible to have any terminals as parent
        throw std::exception();
    }

    return std::move(rootSource);
}

// Called each time the rpn stack is modified, which means that a match might
// have been found
void ExpressionRewriter::checkForMatch() {
    if (sourcePostfixStack.size() < patternPostfixStack.size())
        return;

    const int begin = sourcePostfixStack.size() - patternPostfixStack.size();
    for (int sourceIdx = begin; sourceIdx < begin + patternPostfixStack.size();
         sourceIdx++) {
        const int patternIdx = sourceIdx - begin;
        switch (patternPostfixStack[patternIdx]->expressionType()) {
        case EXPR_STRING: {
            LiteralExpression *symbolExpr = static_cast<LiteralExpression *>(
                patternPostfixStack[patternIdx]);
            symbolMap[symbolExpr->literal()] = sourcePostfixStack[sourceIdx];

            // Variable Literal matches everything (constants, subexpressions
            // etc.)
            continue;
        }

        case EXPR_NUMBER: {
            if (sourcePostfixStack[sourceIdx]->expressionType() != EXPR_NUMBER)
                return;

            LiteralExpression *patternLiteral =
                static_cast<LiteralExpression *>(
                    patternPostfixStack[patternIdx]);
            LiteralExpression *sourceLiteral =
                static_cast<LiteralExpression *>(sourcePostfixStack[sourceIdx]);

            if (sourceLiteral->literal() != patternLiteral->literal())
                return;

            continue;
        }

        case EXPR_PLUS:
        case EXPR_MINUS:
        case EXPR_MULTIPLICATION:
        case EXPR_DIVISION:
        case EXPR_MODULU: {
            if (sourcePostfixStack[sourceIdx]->expressionType() !=
                patternPostfixStack[patternIdx]->expressionType())
                return;

            continue;
        }

        case EXPR_NEGATE: {
            // Verify that they have the same operator
            UnaryExpression *patternUnaryOp =
                static_cast<UnaryExpression *>(patternPostfixStack[patternIdx]);

            UnaryExpression *sourceUnaryOp =
                dynamic_cast<UnaryExpression *>(sourcePostfixStack[sourceIdx]);
            if (!sourceUnaryOp)
                return;
            if (sourceUnaryOp->op().type() != patternUnaryOp->op().type())
                return;

            continue;
        }

        default:
            throw std::exception(); // TODO: handle
        }
    }

    // If we get to here, it means that the pattern matches at this point
    replacementNode = sourcePostfixStack.back();
    replacementParent = replacementNode->parent;
    match = true;
    state = RESOLVING_REPLACEMNT;
}

void ExpressionRewriter::resolveReplacement() {
    Expression *node = replacementPostfixStack.back();
    if (node->expressionType() != EXPR_STRING)
        return;

    LiteralExpression *nodeStr = static_cast<LiteralExpression *>(node);
    auto correspondingExpression = symbolMap.find(nodeStr->literal());
    assert(correspondingExpression != symbolMap.end());
    if (node->parent == nullptr) {
        this->replacement = std::unique_ptr<Expression>(correspondingExpression->second);
        return;
    }
    switch (node->parent->expressionType()) {
    case EXPR_PLUS:
    case EXPR_MINUS:
    case EXPR_MULTIPLICATION:
    case EXPR_DIVISION:
    case EXPR_MODULU: {
        BinaryExpression *parent =
            static_cast<BinaryExpression *>(node->parent);

        // Figure out which side of operator the nodeToReplace is on
        bool rhs = &parent->right() == node ? true : false;
        auto value = std::unique_ptr<Expression>(correspondingExpression->second);

        if (rhs)
            parent->setRight(std::move(value));
        else
            parent->setLeft(std::move(value));

        break;
    }
    case EXPR_NEGATE: {
        UnaryExpression *parent = static_cast<UnaryExpression *>(node->parent);

        parent->setOperand(
            std::unique_ptr<Expression>(correspondingExpression->second));
        break;
    }

    default:
        // Should no be possible to have any terminals as parent
        throw std::exception();
    }
}

inline void ExpressionRewriter::push(Expression &expression) {
    switch (state) {
    case PROCCESING_PATTERN:
        patternPostfixStack.push_back(std::addressof(expression));
        break;

    case PROCESSING_SOURCE:
        sourcePostfixStack.push_back(std::addressof(expression));
        checkForMatch();
        break;

    case RESOLVING_REPLACEMNT:
        replacementPostfixStack.push_back((std::addressof(expression)));
        resolveReplacement();
        break;
    }
}

void ExpressionRewriter::visitLiteralExpression(LiteralExpression &expression) {
    push(expression);
}
void ExpressionRewriter::visitBinaryExpression(BinaryExpression &expression) {
    expression.left().accept(*this);
    expression.right().accept(*this);
    push(expression);
}
void ExpressionRewriter::visitUnaryExpression(UnaryExpression &expression) {
    expression.operand().accept(*this);
    push(expression);
}
