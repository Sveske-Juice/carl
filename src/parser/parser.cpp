#include "parser/parser.h"
#include "parser/expression.h"
#include "parser/parser_errors.h"
#include "parser/statement.h"

#include <exception>
#include <initializer_list>
#include <memory>

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    while (!isAtEnd()) {
        statements.push_back(statement());
    }

    return statements;
}

std::unique_ptr<Statement> Parser::statement() {
    std::unique_ptr<Statement> expr = expressionStatement();

    if (!matchAny({TokenType::SEMICOLON})) {
        throw MissingTerminator(previous());
    }

    return std::move(expr);
}

std::unique_ptr<Statement> Parser::expressionStatement() {
    std::unique_ptr<Expression> expr = expression();
    return std::make_unique<ExpressionStatement>(std::move(expr));
}

std::unique_ptr<Expression> Parser::expression() {
    return equality();
}

std::unique_ptr<Expression> Parser::equality() {
    std::unique_ptr<Expression> expression = comparison();
    while (matchAny({TokenType::BANG_EQUALS, TokenType::EQUALS_EQUALS})) {
        Token equialityOp = previous();
        std::unique_ptr<Expression> right = comparison();
        std::unique_ptr<Expression> binOp = std::make_unique<BinaryExpression>(std::move(expression), equialityOp, std::move(right));
        expression = std::move(binOp);
    }
    return expression;
}
std::unique_ptr<Expression> Parser::comparison() {
    std::unique_ptr<Expression> expression = term();
    while (matchAny({TokenType::GREATER, TokenType::GREATER_EQUALS, TokenType::LESS, TokenType::LESS_EQUALS})) {
        Token comparisonOp = previous();
        std::unique_ptr<Expression> right = term();
        std::unique_ptr<Expression> binOp = std::make_unique<BinaryExpression>(std::move(expression), comparisonOp, std::move(right));
        expression = std::move(binOp);
    }
    return expression;

}
std::unique_ptr<Expression> Parser::term() {
    std::unique_ptr<Expression> expression = factor();
    while (matchAny({TokenType::PLUS, TokenType::MINUS})) {
        Token termOp = previous();
        std::unique_ptr<Expression> right = factor();
        std::unique_ptr<Expression> binOp = std::make_unique<BinaryExpression>(std::move(expression), termOp, std::move(right));
        expression = std::move(binOp);
    }
    return expression;
}
std::unique_ptr<Expression> Parser::factor() {
    std::unique_ptr<Expression> expression = unary();
    while (matchAny({TokenType::SLASH, TokenType::STAR})) {
        Token factorOp = previous();
        std::unique_ptr<Expression> right = unary();
        std::unique_ptr<Expression> binOp = std::make_unique<BinaryExpression>(std::move(expression), factorOp, std::move(right));
        expression = std::move(binOp);
    }
    return expression;
}
std::unique_ptr<Expression> Parser::unary() {
    if (matchAny({TokenType::BANG, TokenType::MINUS})) {
        Token unaryOp = previous();
        std::unique_ptr<Expression> operand = unary();
        return std::make_unique<UnaryExpression>(UnaryExpression{unaryOp, std::move(operand)});
    }
    return primary();
}
std::unique_ptr<Expression> Parser::primary() {
    if (matchAny({TokenType::FALSE, TokenType::TRUE, TokenType::NUMBER, TokenType::STRING})) {
        Token literal = previous();
        return std::make_unique<LiteralExpression>(LiteralExpression(literal.type(), literal.literal()));
    }

    if (matchAny({TokenType::LPAREN})) {
        std::unique_ptr<Expression> innerExpression = expression();

        uint16_t length{0};
        do {
            // Consumes token in matchAny()...
            length += peek().length();
        } while(matchAny({TokenType::RPAREN}));

        // Verify it wasn't a EOF token
        Token token = previous();
        if (token.type() != TokenType::RPAREN) {
            throw MissingClosingBracket(peek().sourceOffset(), length);
        }
        return innerExpression;
    }

    throw UnexpectedToken(peek());
}

bool Parser::matchAny(std::initializer_list<TokenType> tokenTypes) {
    for (TokenType type : tokenTypes) {
        if (!isAtEnd() && peek().type() == type) {
            consume();
            return true;
        }
    }
    return false;
}

inline void Parser::consume() {
    this->currentIndex++;
}

inline bool Parser::isAtEnd() const {
    return lexemes[currentIndex].type() == TokenType::END_OF_FILE;
}

inline Token Parser::previous() const {
    return lexemes[currentIndex - 1];
}

inline Token Parser::peek() const {
    return lexemes[currentIndex];
}

inline bool Parser::isOfType(TokenType typeToCheck, std::initializer_list<TokenType> types) const {
    for (TokenType type : types) {
        if (typeToCheck == type)
            return true;
    }

    return false;
}
