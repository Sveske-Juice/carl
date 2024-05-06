#ifndef PARSER_ERRORS
#define PARSER_ERRORS

#include "carl_constants.h"
#include "lexer/token.h"
#include <cassert>
#include <cstdint>
#include <exception>
#include <format>
#include <string>
#include <tuple>

// Base Lexer exception
class ParserException : public std::exception {
    protected:
        const uint16_t sourceOffset_;
        const uint16_t length_;
        const Token* token_;
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
            std::tie(startLineNumber, startCharacterIndex) = ParserException::calculateLineNumber(source, sourceOffset_);

            uint16_t endLineNumber;
            uint16_t endCharacterIndex;
            std::tie(endLineNumber, endCharacterIndex) = ParserException::calculateLineNumber(source, sourceOffset_ + length_);
            return std::format("{}:{} - {}:{}", startLineNumber, startCharacterIndex, endLineNumber, endCharacterIndex);
        }
    public:
        ParserException(const uint16_t sourceOffset, const uint16_t length) : sourceOffset_{sourceOffset}, length_{length}, token_{nullptr} {}
        ParserException(const Token& token) : sourceOffset_{token.sourceOffset()}, length_{token.length()}, token_{&token} {}

        virtual const std::string what(std::string_view source) const noexcept = 0;
        virtual const ParserError error_code() const noexcept = 0;
};

class UnexpectedToken : public ParserException {
    public:
        UnexpectedToken(const uint16_t sourceOffset, const uint16_t length) : ParserException(sourceOffset, length) {}
        UnexpectedToken(const Token& token) : ParserException(token) {}

        const ParserError error_code() const noexcept override {
            return ParserError::UNEXPECTED_TOKEN;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Unexpected Token '" + token_->literal() + "'"};
        }
};

class MissingClosingBracket : public ParserException {
    public:
        MissingClosingBracket(const uint16_t sourceOffset, const uint16_t length) : ParserException(sourceOffset, length) {}
        MissingClosingBracket(const Token& token) : ParserException(token) {}

        const ParserError error_code() const noexcept override {
            return ParserError::MISSING_CLOSING_BRACKET;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{"Missing Closing Bracket at: " + locationPrefix(source)};
        }
};

#endif
