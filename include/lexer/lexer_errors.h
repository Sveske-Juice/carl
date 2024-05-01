#ifndef LEXER_ERRORS_H
#define LEXER_ERRORS_H

#include "lexer/token.h"

#include <cstdint>
#include <exception>
#include <string_view>
#include <cassert>
#include <format>
#include <tuple>

// Base Lexer exception
class LexerException : public std::exception {
    protected:
        const uint16_t sourceOffset_;
        const uint16_t length_;
        static inline std::tuple<uint16_t, uint16_t> calculateLineNumber(const std::string_view source, const uint16_t sourceOffset) {
            assert((source.size() >= sourceOffset) && "Could not calculate line number since the offset exceeds the bounds of the source string given");

            uint16_t lineNumber{0};
            uint16_t offsetToLineNumber{0};
            for (uint16_t offset = 0; offset != sourceOffset; offset++) {
                if (source[offset] == '\n') {
                    lineNumber++;
                    offsetToLineNumber = offset;
                }
            }
            return std::make_tuple(lineNumber, sourceOffset - offsetToLineNumber);
        }
        std::string locationPrefix(std::string_view source) const {
            uint16_t startLineNumber;
            uint16_t startCharacterIndex;
            std::tie(startLineNumber, startCharacterIndex) = LexerException::calculateLineNumber(source, sourceOffset_);

            uint16_t endLineNumber;
            uint16_t endCharacterIndex;
            std::tie(endLineNumber, endCharacterIndex) = LexerException::calculateLineNumber(source, sourceOffset_ + length_);
            return std::format("{}:{} - {}:{}", startLineNumber, startCharacterIndex, endLineNumber, endCharacterIndex);
        }
    public:
        LexerException(const uint16_t sourceOffset, const uint16_t length) : sourceOffset_{sourceOffset}, length_{length} {}
        LexerException(const Token& token) : sourceOffset_{token.sourceOffset()}, length_{token.length()} {}

        virtual const std::string what(std::string_view source) const noexcept = 0;
        virtual const uint8_t error_code() const noexcept = 0;
};


class UnterminatedString : public LexerException {
    public:
        UnterminatedString(const uint16_t sourceOffset, const uint16_t length) : LexerException(sourceOffset, length) {}
        UnterminatedString(const Token& token) : LexerException(token) {}

        const uint8_t error_code() const noexcept override {
            return 10;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Unterminated String"};
        }
};

class IncompleteDelimiter : public LexerException {
    public:
        IncompleteDelimiter(const uint16_t sourceOffset, const uint16_t length) : LexerException(sourceOffset, length) {}
        IncompleteDelimiter(const Token& token) : LexerException(token) {}

        const uint8_t error_code() const noexcept override {
            return 11;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Unfinished delimiter"};
        }
};

class UnrecognizedCharacter : public LexerException {
    public:
        UnrecognizedCharacter(const uint16_t sourceOffset, const uint16_t length) : LexerException(sourceOffset, length) {}
        UnrecognizedCharacter(const Token& token) : LexerException(token) {}

        const uint8_t error_code() const noexcept override {
            return 11;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Unrecognized Character"};
        }
};

#endif
