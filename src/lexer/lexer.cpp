#include "lexer/lexer.h"
#include "lexer/token.h"

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <string>
#include <vector>

Lexer::Lexer(std::string source) : source{source} {}

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
                                   std::string{"("}));
            break;
        case ')':
            tokens.push_back(Token(TokenType::RPAREN, start,
                                   start - currentCharacterIndex,
                                   std::string{")"}));
            break;
        case '[':
            tokens.push_back(Token(TokenType::LSQUARE, start,
                                   start - currentCharacterIndex,
                                   std::string{"["}));
            break;
        case ']':
            tokens.push_back(Token(TokenType::RSQUARE, start,
                                   start - currentCharacterIndex,
                                   std::string{"]"}));
            break;
        case '{':
            tokens.push_back(Token(TokenType::LCURLY, start,
                                   start - currentCharacterIndex,
                                   std::string{"{"}));
            break;
        case '}':
            tokens.push_back(Token(TokenType::RCURLY, start,
                                   start - currentCharacterIndex,
                                   std::string{"}"}));
            break;
        case ',':
            tokens.push_back(Token(TokenType::COMMA, start,
                                   start - currentCharacterIndex,
                                   std::string{","}));
            break;
        case '.':
            tokens.push_back(Token(TokenType::DOT, start,
                                   start - currentCharacterIndex,
                                   std::string{"."}));
            break;
        case '-':
            tokens.push_back(Token(TokenType::MINUS, start,
                                   start - currentCharacterIndex,
                                   std::string{"-"}));
            break;
        case '+':
            tokens.push_back(Token(TokenType::PLUS, start,
                                   start - currentCharacterIndex,
                                   std::string{"+"}));
            break;
        case ';':
            tokens.push_back(Token(TokenType::SEMICOLON, start,
                                   start - currentCharacterIndex,
                                   std::string{";"}));
            break;
        case '*':
            tokens.push_back(Token(TokenType::STAR, start,
                                   start - currentCharacterIndex,
                                   std::string{"*"}));
            break;
        case ':':
            tokens.push_back(Token(TokenType::COLON, start,
                                   start - currentCharacterIndex,
                                   std::string{":"}));
            break;
        case '/':
            tokens.push_back(Token(TokenType::SLASH, start,
                                   start - currentCharacterIndex,
                                   std::string{"/"}));
            break;
        case '^':
            tokens.push_back(Token(TokenType::EXPONENT, start,
                                   start - currentCharacterIndex,
                                   std::string{"^"}));
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
                                       std::string{"!="}));
            else
                tokens.push_back(Token(TokenType::BANG, start,
                                       start - currentCharacterIndex,
                                       std::string{"!"}));
            break;
        case '=':
            if (match('='))
                tokens.push_back(Token(TokenType::EQUALS_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string{"=="}));
            else
                tokens.push_back(Token(TokenType::EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string{"="}));
            break;

        case '<':
            if (match('='))
                tokens.push_back(Token(TokenType::LESS_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string{"<="}));
            else
                tokens.push_back(Token(TokenType::LESS, start,
                                       start - currentCharacterIndex,
                                       std::string{"<"}));
            break;

        case '>':
            if (match('='))
                tokens.push_back(Token(TokenType::GREATER_EQUALS, start,
                                       start - currentCharacterIndex,
                                       std::string{">="}));
            else
                tokens.push_back(Token(TokenType::GREATER, start,
                                       start - currentCharacterIndex,
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
                throw std::exception(); // TODO: prober err handling

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

                    // A delimiter was added but no fractional part was provided, ex: '5.'
                    if (!std::isdigit(peek()))
                        throw std::exception(); // TODO: err

                    while (std::isdigit(peek()))
                        consume(); // Consume fractional part
                }

                size_t numberLen = currentCharacterIndex - start;
                std::string numberLiteral{source.begin() + start,
                                          source.begin() + start + numberLen};
                tokens.push_back(
                    Token(TokenType::NUMBER, start, numberLen, numberLiteral));
            }
            // Lex identifiers
            else if (std::isalpha(c)) {
            }
            break;
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, start,
                           start - currentCharacterIndex, std::string{"\0"}));
    return tokens;
}
