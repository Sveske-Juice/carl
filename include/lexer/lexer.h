#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <string_view>
#include <vector>

class Lexer {
    public:
        Lexer(std::string_view source);
        std::vector<Token> tokenize();

    private:
        const std::string_view source;

        uint16_t start{0};
        uint16_t currentCharacterIndex{0};
        uint16_t currentLine{0};

        bool isAtEndOfFile() const;
        char consume();
        char peek(size_t ahead = 1) const;
        bool match(char expected);
};

#endif
