#ifndef PARSER_ERRORS
#define PARSER_ERRORS

#include "carl_constants.h"
#include "lexer/token.h"
#include <cassert>
#include <cstdint>
#include <exception>
#include <fmt/core.h>
#include <format>
#include <iostream>
#include <string>
#include <tuple>

// Base Lexer exception
class ParserException : public std::exception {
    protected:
        const uint16_t sourceOffset_;
        const uint16_t length_;
        std::tuple<uint16_t, uint16_t> calculateLineNumber(const std::string_view source, const uint16_t sourceOffset) const {
            std::cout << fmt::format("off: {}, len: {}, size: {}", sourceOffset_, length_, source.size()) << std::endl;
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
            std::tie(startLineNumber, startCharacterIndex) = calculateLineNumber(source, sourceOffset_);

            uint16_t endLineNumber;
            uint16_t endCharacterIndex;
            std::tie(endLineNumber, endCharacterIndex) = ParserException::calculateLineNumber(source, sourceOffset_ + length_);
            return std::format("{}:{} - {}:{}", startLineNumber, startCharacterIndex, endLineNumber, endCharacterIndex);
        }
    public:
        ParserException(const uint16_t sourceOffset, const uint16_t length) : sourceOffset_{sourceOffset}, length_{length} {}

        virtual const std::string what(std::string_view source) const noexcept = 0;
        virtual const ParserError error_code() const noexcept = 0;
};

class SyntaxError : public ParserException {
    private:
        const Token token_;
    public:
        SyntaxError(const Token token) : token_{token}, ParserException(token.sourceOffset(), token.length()) {}

        const ParserError error_code() const noexcept override {
            return ParserError::SYNTAX_ERROR;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Syntax Error" + token_.literal() + "'"};
        }
};

class UnexpectedToken : public ParserException {
    private:
        const Token token_;
    public:
        UnexpectedToken(const Token token) : token_{token}, ParserException(token.sourceOffset(), token.length()) {}

        const ParserError error_code() const noexcept override {
            return ParserError::UNEXPECTED_TOKEN;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Unexpected Token '" + token_.literal() + "'"};
        }
};

class MissingClosingBracket : public ParserException {
    public:
        MissingClosingBracket(const uint16_t sourceOffset, const uint16_t length) : ParserException(sourceOffset, length) {}

        const ParserError error_code() const noexcept override {
            return ParserError::MISSING_CLOSING_BRACKET;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{"Missing Closing Bracket at: " + locationPrefix(source)};
        }
};

class MissingTerminator : public ParserException {
    private:
        const Token token_;
    public:
        MissingTerminator(const Token token) : token_{token}, ParserException(token.sourceOffset(), token.length()) {}

        const ParserError error_code() const noexcept override {
            return ParserError::MISSING_TERMINATOR;
        }

        const std::string what(std::string_view source) const noexcept override {
            return std::string{locationPrefix(source) + " Missing terminator after '" + token_.literal() + "'"};
        }
};
#endif
