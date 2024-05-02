#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"

#include <vector>

class Parser {
    private:
        const std::vector<Token> lexemes;

    public:
        Parser(const std::vector<Token> _lexemes) : lexemes{std::move(_lexemes)} {}
};

#endif
