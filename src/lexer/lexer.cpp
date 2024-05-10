#include "lexer/lexer.h"
#include "lexer/lexer_errors.h"
#include "lexer/token.h"

#include <cctype>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

Lexer::Lexer(std::string source) : source{source} {}

inline bool Lexer::isAtEndOfFile() const {
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
                                   currentCharacterIndex - start,
                                   std::string{"("}));
            break;
        case ')':
            tokens.push_back(Token(TokenType::RPAREN, start,
                                   currentCharacterIndex - start,
                                   std::string{")"}));
            break;
        case ',':
            tokens.push_back(Token(TokenType::COMMA, start,
                                   currentCharacterIndex - start,
                                   std::string{","}));
            break;
        case '.':
            tokens.push_back(Token(TokenType::DOT, start,
                                   currentCharacterIndex - start,
                                   std::string{"."}));
            break;
        case '+':
            tokens.push_back(Token(TokenType::PLUS, start,
                                   currentCharacterIndex - start,
                                   std::string{"+"}));
            break;
        case ';':
            tokens.push_back(Token(TokenType::SEMICOLON, start,
                                   currentCharacterIndex - start,
                                   std::string{";"}));
            break;
        case '*':
            tokens.push_back(Token(TokenType::STAR, start,
                                   currentCharacterIndex - start,
                                   std::string{"*"}));
            break;
        case ':':
            tokens.push_back(Token(TokenType::COLON, start,
                                   currentCharacterIndex - start,
                                   std::string{":"}));
            break;
        case '/':
            tokens.push_back(Token(TokenType::SLASH, start,
                                   currentCharacterIndex - start,
                                   std::string{"/"}));
            break;
        case '^':
            tokens.push_back(Token(TokenType::EXPONENT, start,
                                   currentCharacterIndex - start,
                                   std::string{"^"}));
            break;
        case '%':
            tokens.push_back(Token(TokenType::MODULUS, start,
                                   currentCharacterIndex - start,
                                   std::string{"%"}));
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
                                       currentCharacterIndex - start,
                                       std::string{"!="}));
            else
                tokens.push_back(Token(TokenType::BANG, start,
                                       currentCharacterIndex - start,
                                       std::string{"!"}));
            break;
        case '=':
            if (match('='))
                tokens.push_back(Token(TokenType::EQUALS_EQUALS, start,
                                       currentCharacterIndex - start,
                                       std::string{"=="}));
            else
                tokens.push_back(Token(TokenType::EQUALS, start,
                                       currentCharacterIndex - start,
                                       std::string{"="}));
            break;
        case '-':
            if (match('>'))
                tokens.push_back(
                    (Token(TokenType::ARROW, start,
                           currentCharacterIndex - start, std::string{"->"})));
            else
                tokens.push_back(Token(TokenType::MINUS, start,
                                       currentCharacterIndex - start,
                                       std::string{"-"}));
            break;

        case '<':
            if (match('='))
                tokens.push_back(Token(TokenType::LESS_EQUALS, start,
                                       currentCharacterIndex - start,
                                       std::string{"<="}));
            else
                tokens.push_back(Token(TokenType::LESS, start,
                                       currentCharacterIndex - start,
                                       std::string{"<"}));
            break;

        case '>':
            if (match('='))
                tokens.push_back(Token(TokenType::GREATER_EQUALS, start,
                                       currentCharacterIndex - start,
                                       std::string{">="}));
            else
                tokens.push_back(Token(TokenType::GREATER, start,
                                       currentCharacterIndex - start,
                                       std::string{">"}));
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
                throw UnterminatedString(start, currentCharacterIndex - start);

            // Consume last quote
            consume();

            // Create string literal
            auto stringStart = source.begin() + start + 1;
            size_t stringLen = currentCharacterIndex - start - 1;
            auto stringEnd = source.begin() + start + stringLen;
            std::string literal{stringStart, stringEnd};

            tokens.push_back(
                Token(TokenType::STRING, start, stringLen + 1, literal));
            break;
        }
        default:
            // Lex numbers
            if (std::isdigit(c)) {
                while (std::isdigit(peek()))
                    consume(); // Consume all numbers

                // Look for fractional part
                if (peek() == '.') {
                    consume(); // Consume delimiter

                    // A delimiter was added but no fractional part was
                    // provided, ex: '5.'
                    if (!std::isdigit(peek()))
                        throw IncompleteDelimiter(start, currentCharacterIndex -
                                                             start);

                    while (std::isdigit(peek()))
                        consume(); // Consume fractional part
                }

                size_t numberLen = currentCharacterIndex - start;
                std::string numberLiteral{source.begin() + start,
                                          source.begin() + start + numberLen};
                tokens.push_back(
                    Token(TokenType::NUMBER, start, numberLen, numberLiteral));
            }
            // Lex keywords & identifiers (variables, functions etc.)
            else if (isAlpha(c)) {
                while (isAlpha(peek()))
                    consume();

                size_t identifierLen = currentCharacterIndex - start;
                std::string identifier{source.begin() + start,
                                       source.begin() + start + identifierLen};

                // Check if it's a reserved keyword
                auto it = keywords.find(identifier);
                if (it != keywords.end()) {
                    tokens.push_back(
                        Token(it->second, start, identifierLen, identifier));
                } else {
                    tokens.push_back(Token(TokenType::IDENTIFIER, start,
                                           identifierLen, identifier));
                }
            } else {
                // Uhm what is this character??
                throw UnrecognizedCharacter(start,
                                            currentCharacterIndex - start);
            }
            break;
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, start,
                           currentCharacterIndex - start, std::string{"\0"}));
    return tokens;
}

inline bool Lexer::isAlpha(char c) const { return std::isalpha(c) || c == '_' || c == '-'; }

std::string Lexer::tokensToString(const std::vector<Token> &tokens) {
    std::string output;
    for (Token token : tokens) {
        output += std::format("{}('{}')\n", TokenTypesToString[token.type()],
                              token.literal());
    }
    return output;
}
