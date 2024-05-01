#ifndef LEXER_ERRORS_H
#define LEXER_ERRORS_H

#include "lexer/token.h"
#include <cstdint>
#include <exception>

class LexerException : public std::exception {
    private:
        const uint16_t sourceOffset_;
        const uint16_t length_;
    public:
        LexerException(const uint16_t sourceOffset, const uint16_t length) : sourceOffset_{sourceOffset}, length_{length} {}
        LexerException(const Token& token) : sourceOffset_{token.sourceOffset()}, length_{token.length()} {}

        virtual const char *what() const noexcept = 0;
        virtual const uint8_t error_code() const noexcept = 0;
};


class UnterminatedStringException : public LexerException {
    public:
        UnterminatedStringException(const uint16_t sourceOffset, const uint16_t length) : LexerException(sourceOffset, length) {}
        UnterminatedStringException(const Token& token) : LexerException(token) {}

        const uint8_t error_code() const noexcept override {
            return 10;
        }

        const char * what() const noexcept override {
            return "Unterminated String";
        }
};

#endif
