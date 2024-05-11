#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"
#include "parser/expression.h"
#include "parser/statement.h"

#include <memory>
#include <vector>

class Parser {
    private:
        const std::vector<Token> lexemes;
        size_t currentIndex{0};
        void consume();
        bool matchAny(std::initializer_list<TokenType> tokenTypes);
        bool isOfType(TokenType typeToCheck, std::initializer_list<TokenType> types) const;
        bool isAtEnd() const;
        Token previous() const;
        Token peek() const;

        std::unique_ptr<Statement> command();
        std::unique_ptr<Statement> applyStatement();

        std::unique_ptr<Statement> defineStatement();

        std::unique_ptr<Statement> statement();
        std::unique_ptr<Statement> expressionStatement();

        std::unique_ptr<Expression> expression();
        std::unique_ptr<Expression> equality();
        std::unique_ptr<Expression> comparison();
        std::unique_ptr<Expression> term();
        std::unique_ptr<Expression> factor();
        std::unique_ptr<Expression> unary();
        std::unique_ptr<Expression> primary();

    public:
        Parser(const std::vector<Token> _lexemes) : lexemes{std::move(_lexemes)} {}
        std::vector<std::unique_ptr<Statement>> parse();
};

#endif
