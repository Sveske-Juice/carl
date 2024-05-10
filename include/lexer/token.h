#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <string>
#include <string_view>

static std::string TokenTypesToString[] = {
    "String",
    "Number",

    "Identifier",
    "Definition",

    "Plus",
    "Minus",
    "Star",
    "Slash",
    "Modulus",
    "Exponent",

    "Left Parenthesis",
    "Right Parenthesis",
    "Equals",
    "Less",
    "Greater",
    "Equal Equals",
    "Less Equals",
    "Greater Equals",
    "Not Equals",
    "Arrow",

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
    DEFINE,

    // Operands
    PLUS,     // +
    MINUS,    // -
    STAR,     // *
    SLASH,    // /
    MODULUS,  // %
    EXPONENT, // ^

    // Brackets
    LPAREN,  // (
    RPAREN,  // )

    EQUALS,         // =
    LESS,           // <
    GREATER,        // >
    EQUALS_EQUALS,  // ==
    LESS_EQUALS,    // <=
    GREATER_EQUALS, // >=
    BANG_EQUALS,    // !=
    ARROW,          // ->

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
    const uint16_t _sourceOffset;
    const uint8_t _length;
    const std::string _literal;

public:
    Token(TokenType type, uint16_t sourceOffset, uint8_t length,
          std::string literal)
        : _type(type), _sourceOffset(sourceOffset), _length(length),
          _literal(literal) {}

    const TokenType type() const { return _type; }
    const uint16_t sourceOffset() const { return _sourceOffset; }
    const uint8_t length() const { return _length; }
    const uint16_t sourceEnd() const { return _sourceOffset + _length; }
    const std::string literal() const { return _literal; }
};

#endif
