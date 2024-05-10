#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <string>
#include <unordered_map>
#include <vector>

static inline const std::unordered_map<std::string, TokenType> keywords{
    {"not", TokenType::NOT},     {"and", TokenType::AND},
    {"or", TokenType::OR},       {"if", TokenType::IF},
    {"else", TokenType::ELSE},   {"while", TokenType::WHILE},
    {"for", TokenType::FOR},     {"true", TokenType::TRUE},
    {"false", TokenType::FALSE}, {"define", TokenType::DEFINE},
    {"apply", TokenType::APPLY}
};

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> tokenize();
    static std::string tokensToString(const std::vector<Token>& tokens);

private:
    const std::string source;

    uint16_t start{0};
    uint16_t currentCharacterIndex{0};

    bool isAlpha(char c) const;
    bool isAtEndOfFile() const;
    char peek(size_t ahead = 0) const;
    char consume();
    bool match(char expected);
};

#endif
