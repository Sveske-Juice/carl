#ifndef RUNTIME_ERRORS_H
#define RUNTIME_ERRORS_H

#include "carl_constants.h"
#include "lexer/token.h"
#include <cassert>
#include <cstdint>
#include <exception>
#include <fmt/core.h>
#include <format>
#include <string_view>
#include <tuple>

class RuntimeException : public std::exception {
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
            std::tie(startLineNumber, startCharacterIndex) = RuntimeException::calculateLineNumber(source, sourceOffset_);

            uint16_t endLineNumber;
            uint16_t endCharacterIndex;
            std::tie(endLineNumber, endCharacterIndex) = RuntimeException::calculateLineNumber(source, sourceOffset_ + length_);
            return std::format("{}:{} - {}:{}", startLineNumber, startCharacterIndex, endLineNumber, endCharacterIndex);
        }
    public:
        RuntimeException(const uint16_t sourceOffset, const uint16_t length) : sourceOffset_{sourceOffset}, length_{length} {}
        RuntimeException(const Token& token) : sourceOffset_{token.sourceOffset()}, length_{token.length()} {}

        virtual const std::string what(std::string_view source) const noexcept = 0;
        virtual const RuntimeError error_code() const noexcept = 0;
};

class TypeMismatch : public RuntimeException {
    private:
        const std::string reason_;
    public:
        TypeMismatch(const uint16_t sourceOffset, const uint16_t length) : RuntimeException(sourceOffset, length) {}
        TypeMismatch(const Token& token, std::string reason) : RuntimeException(token), reason_{reason} {}

        virtual const std::string what(std::string_view source) const noexcept override {
            return fmt::format("{} {}", locationPrefix(source), reason_);
        }
        virtual const RuntimeError error_code() const noexcept override {
            return RuntimeError::TYPE_MISMATCH;
        }
};

class NoRuleFound : public RuntimeException {
    private:
        const std::string reason_;
    public:
        NoRuleFound(const uint16_t sourceOffset, const uint16_t length) : RuntimeException(sourceOffset, length) {}
        NoRuleFound(const Token& token, std::string reason) : RuntimeException(token), reason_{reason} {}

        virtual const std::string what(std::string_view source) const noexcept override {
            return fmt::format("{} {}", locationPrefix(source), reason_);
        }
        virtual const RuntimeError error_code() const noexcept override {
            return RuntimeError::NO_RULE_FOUND;
        }
};


#endif
