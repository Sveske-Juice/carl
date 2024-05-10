#include <catch2/catch_test_macros.hpp>

#include "parser/ast_printer.h"
#include "runtime/expressionrewriter.h"

// TODO: dont rely on printer to verify results
TEST_CASE("Pean arithmetic axioms", "[ExpreRewriter:Peano]") {
    SECTION("Addition") {
        ExpressionRewriter rewriter;
        AstPrinter printer;
        // Pattern
        // A + 0
        auto pA = std::make_unique<LiteralExpression>(TokenType::STRING, "A");
        auto p0 = std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
        auto pattern = std::make_unique<BinaryExpression>(
            std::move(pA), Token(TokenType::PLUS, 0, 1, "+"), std::move(p0));

        // Replacement
        // A
        auto replacement = std::make_unique<LiteralExpression>(TokenType::STRING, "A");

        // Concrete tests to test the rule/pattern
        SECTION("Add1") {
            // 5 + 0 -> 5
            auto s5 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "5");
            auto s0 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
            auto src = std::make_unique<BinaryExpression>(
                std::move(s5), Token(TokenType::PLUS, 0, 1, "+"),
                std::move(s0));
            auto result = rewriter.substitute(std::move(src), std::move(pattern), std::move(replacement));
            CHECK(printer.print(*result.get()) == "5");
        }
    }
}

TEST_CASE("Expression Rewriter variable replacements",
          "[ExprRewriter:VarReplace]") {
    // Pattern
    // A + B
    std::unique_ptr<LiteralExpression> pA =
        std::make_unique<LiteralExpression>(TokenType::STRING, "A");
    std::unique_ptr<LiteralExpression> pB =
        std::make_unique<LiteralExpression>(TokenType::STRING, "B");
    std::unique_ptr<Expression> pattern = std::make_unique<BinaryExpression>(
        std::move(pA), Token(TokenType::PLUS, 0, 1, "+"), std::move(pB));

    // Replacement
    // B + A
    auto rA = std::make_unique<LiteralExpression>(TokenType::STRING, "B");
    auto rB = std::make_unique<LiteralExpression>(TokenType::STRING, "A");
    auto replacement = std::make_unique<BinaryExpression>(
        std::move(rA), Token(TokenType::PLUS, 0, 1, "+"), std::move(rB));

    // Source
    // (4+6)/2
    auto s4 = std::make_unique<LiteralExpression>(TokenType::NUMBER, "4");
    auto s6 = std::make_unique<LiteralExpression>(TokenType::NUMBER, "6");
    auto plusOp = std::make_unique<BinaryExpression>(
        std::move(s4), Token(TokenType::PLUS, 0, 1, "+"), std::move(s6));

    auto s2 = std::make_unique<LiteralExpression>(TokenType::NUMBER, "2");
    auto source = std::make_unique<BinaryExpression>(
        std::move(plusOp), Token(TokenType::SLASH, 0, 1, "/"), std::move(s2));

    // Applied Source
    // B + A
    ExpressionRewriter expressionrewriter;
    AstPrinter printer;
    std::unique_ptr<Expression> applied = expressionrewriter.substitute(
        std::move(source), std::move(pattern), std::move(replacement));
    CHECK(printer.print(*applied.get()) == "((6 + 4) / 2)");
}
