#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <string_view>

static std::string_view TokenTypesToString[] = {
    "String",
    "Number",

    "Identifier",

    "Plus",
    "Minus",
    "Star",
    "Slash",
    "Modulus",
    "Exponent",

    "Left Squarebracket",
    "Right Squarebracket",
    "Left Curlybracket",
    "Right Curlybracket",
    "Left Parenthesis",
    "Right Parenthesis",
    "Equals",
    "Less",
    "Greater",
    "Equal Equals",
    "Less Equals",
    "Greater Equals",
    "Not Equals",

    "Colon",
    "Semicolon",
    "Comma",
    "Dot",
    "Bang",

    "NOT",
    "AND",
    "OR",
    "If",
    "ELSE",
    "WHILE",
    "FOR",
    "True",
    "False",

    "End Of File"
};

enum TokenType {
    STRING, // "stuff enclosed in quotes"
    NUMBER, // numbers

    IDENTIFIER, // variable, function, etc.

    // Operands
    PLUS,     // +
    MINUS,    // -
    STAR,     // *
    SLASH,    // /
    MODULUS,  // %
    EXPONENT, // ^

    // Brackets
    LSQUARE, // [
    RSQUARE, // ]
    LCURLY,  // {
    RCURLY,  // }
    LPAREN,  // (
    RPAREN,  // )

    // Comparison
    EQUALS,         // =
    LESS,           // <
    GREATER,        // >
    EQUALS_EQUALS,  // ==
    LESS_EQUALS,    // <=
    GREATER_EQUALS, // >=
    BANG_EQUALS,    // !=

    // Characters
    COLON,     // :
    SEMICOLON, // ;
    COMMA,     // ,
    DOT,       // .
    BANG,      // !

    // Token Keywords
    NOT,   // not
    AND,   // and
    OR,    // or
    IF,    // if
    ELSE,  // else
    WHILE, // while
    FOR,   // for
    TRUE,  // true
    FALSE, // false

    // Control Characters
    END_OF_FILE
};

class Token {
private:
    const TokenType _type;
    const uint16_t _sourceBegin;
    const uint8_t _length;
    const std::string_view _literal;

public:
    Token(TokenType type, uint16_t sourceOffset, uint8_t length,
          std::string_view literal)
        : _type(type), _sourceBegin(sourceOffset), _length(length),
          _literal(literal) {}

    const TokenType type() const { return _type; }
    const uint16_t sourceOffset() const { return _sourceBegin; }
    const uint8_t length() const { return _length; }
    const uint16_t sourceEnd() const { return _sourceBegin + _length; }
    const std::string_view literal() const { return _literal; }
};

#endif
