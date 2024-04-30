#include "lexer/lexer.h"
#include "lexer/token.h"

#include <cstdio>
#include <exception>
#include <string_view>
#include <vector>

Lexer::Lexer(std::string_view source) : source{source} {}

bool Lexer::isAtEndOfFile() const {
    return currentCharacterIndex >= source.size();
}
char Lexer::consume() { return source[currentCharacterIndex++]; }
char Lexer::peek(size_t ahead) const {
    // Check for EOF
    if ((currentCharacterIndex + ahead) >= source.size())
        return '\0';
    return source[currentCharacterIndex + ahead];
}

bool Lexer::match(char expected) {
    if (isAtEndOfFile())
        return false;
    if (source[currentCharacterIndex] != expected)
        return false;

    // It's a match - consume
    consume();
    return true;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEndOfFile()) {
        start = currentCharacterIndex;

        // Start lex token
        const char c = consume();

        switch (c) {
        case '(':
            tokens.push_back(Token(TokenType::LPAREN, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case ')':
            tokens.push_back(Token(TokenType::RPAREN, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '[':
            tokens.push_back(Token(TokenType::LSQUARE, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case ']':
            tokens.push_back(Token(TokenType::RSQUARE, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '{':
            tokens.push_back(Token(TokenType::LCURLY, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '}':
            tokens.push_back(Token(TokenType::RCURLY, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case ',':
            tokens.push_back(Token(TokenType::COMMA, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '.':
            tokens.push_back(Token(TokenType::DOT, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '-':
            tokens.push_back(Token(TokenType::MINUS, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '+':
            tokens.push_back(Token(TokenType::PLUS, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case ';':
            tokens.push_back(Token(TokenType::SEMICOLON, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '*':
            tokens.push_back(Token(TokenType::STAR, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case ':':
            tokens.push_back(Token(TokenType::COLON, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '/':
            tokens.push_back(Token(TokenType::SLASH, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;
        case '^':
            tokens.push_back(Token(TokenType::EXPONENT, start,
                                   start - currentCharacterIndex,
                                   std::string_view{&c, 1}));
            break;

        // Ignore comments
        case '#':
            while (peek() != '\n' && !isAtEndOfFile())
                consume();
            break;

        // One or two character tokens
        case '!':
            if (match('='))
                tokens.push_back(Token(TokenType::BANG_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            else
                tokens.push_back(Token(TokenType::BANG, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            break;
        case '=':
            if (match('='))
                tokens.push_back(Token(TokenType::EQUALS_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            else
                tokens.push_back(Token(TokenType::EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            break;

        case '<':
            if (match('='))
                tokens.push_back(Token(TokenType::LESS_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            else
                tokens.push_back(Token(TokenType::LESS, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            break;

        case '>':
            if (match('='))
                tokens.push_back(Token(TokenType::GREATER_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            else
                tokens.push_back(Token(TokenType::GREATER, start,
                                       start - currentCharacterIndex,
                                       std::string_view{&c, 1}));
            break;

        case ' ':
        case '\r':
        case '\t':
        case '\n':
            break;

        case '"': {
            while (peek() != '"' && !isAtEndOfFile())
                consume();

            // Check for unterminated string
            if (isAtEndOfFile())
                throw std::exception(); // TODO: prober err handling

            // Consume last quote
            consume();
            consume();

            // Create string literal
            auto stringStart = source.begin() + start + 1;
            auto stringEnd = stringStart + (start - currentCharacterIndex);
            tokens.push_back(Token(TokenType::STRING, start,
                                   start - currentCharacterIndex,
                                   std::string_view{stringStart, stringEnd}));
            break;
        }
        default:
            // TODO: lex numbers
            break;
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, start,
                           start - currentCharacterIndex, std::string_view{"\0"}));
    return tokens;
}
