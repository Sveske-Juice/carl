#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "parser/ast_printer.h"
#include "runtime/expressionrewriter.h"

// TODO: dont rely on printer to verify results
TEST_CASE("Arithmetic axioms", "[ExpreRewriter:Peano]") {
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
    SECTION("Subtraction") {
        ExpressionRewriter rewriter;
        AstPrinter printer;
        // Pattern
        // A - 0
        auto pA = std::make_unique<LiteralExpression>(TokenType::STRING, "A");
        auto p0 = std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
        auto pattern = std::make_unique<BinaryExpression>(
            std::move(pA), Token(TokenType::MINUS, 0, 1, "-"), std::move(p0));

        // Replacement
        // A
        auto replacement = std::make_unique<LiteralExpression>(TokenType::STRING, "A");

        // Concrete tests to test the rule/pattern
        SECTION("Sub1") {
            // 5 - 0 -> 5
            auto s5 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "5");
            auto s0 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
            auto src = std::make_unique<BinaryExpression>(
                std::move(s5), Token(TokenType::MINUS, 0, 1, "-"),
                std::move(s0));
            auto result = rewriter.substitute(std::move(src), std::move(pattern), std::move(replacement));
            CHECK(printer.print(*result.get()) == "5");
        }
    }
    SECTION("Multiplication") {
        ExpressionRewriter rewriter;
        AstPrinter printer;
        // Pattern
        // A * 0
        auto pA = std::make_unique<LiteralExpression>(TokenType::STRING, "A");
        auto p0 = std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
        auto pattern = std::make_unique<BinaryExpression>(
            std::move(pA), Token(TokenType::STAR, 0, 1, "*"), std::move(p0));

        // Replacement
        // 0
        auto replacement = std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");

        // Concrete tests to test the rule/pattern
        SECTION("Mul1") {
            // 5 * 0 -> 0
            auto s5 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "5");
            auto s0 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
            auto src = std::make_unique<BinaryExpression>(
                std::move(s5), Token(TokenType::STAR, 0, 1, "*"),
                std::move(s0));
            auto result = rewriter.substitute(std::move(src), std::move(pattern), std::move(replacement));
            CHECK(printer.print(*result.get()) == "0");
        }
    }
    SECTION("Division") {
        ExpressionRewriter rewriter;
        AstPrinter printer;
        // Pattern
        // 0 / A
        auto pA = std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
        auto p0 = std::make_unique<LiteralExpression>(TokenType::STRING, "A");
        auto pattern = std::make_unique<BinaryExpression>(
            std::move(pA), Token(TokenType::SLASH, 0, 1, "/"), std::move(p0));

        // Replacement
        // 0
        auto replacement = std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");

        // Concrete tests to test the rule/pattern
        SECTION("Div1") {
            // 0 / 5 -> 0
            auto s5 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "5");
            auto s0 =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "0");
            auto src = std::make_unique<BinaryExpression>(
                std::move(s0), Token(TokenType::SLASH, 0, 1, "/"),
                std::move(s5));
            auto result = rewriter.substitute(std::move(src), std::move(pattern), std::move(replacement));
            CHECK(printer.print(*result.get()) == "0");
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

TEST_CASE("Square", "[ExprRewriter:Square]") {
    // Pattern
    //  ("A" + "B")*("A" + "B")
    auto a1 = std::make_unique<LiteralExpression>(STRING, "A");
    auto b1 = std::make_unique<LiteralExpression>(STRING, "B");
    auto plus1 = std::make_unique<BinaryExpression>(std::move(a1), Token(PLUS, 0, 1, "+"), std::move(b1));

    auto a2 = std::make_unique<LiteralExpression>(STRING, "A");
    auto b2 = std::make_unique<LiteralExpression>(STRING, "B");
    auto plus2 = std::make_unique<BinaryExpression>(std::move(a2), Token(PLUS, 0, 1, "+"), std::move(b2));

    auto mult = std::make_unique<BinaryExpression>(std::move(plus1), Token(STAR, 0, 1, "*"), std::move(plus2));

    // Replacement
    // "A"*"A" + "B"*"B" + 2*"A"*"B"
    auto ra1 = std::make_unique<LiteralExpression>(STRING, "A");
    auto ra2 = std::make_unique<LiteralExpression>(STRING, "A");
    auto rMultA = std::make_unique<BinaryExpression>(std::move(ra1), Token(STAR, 0, 1, "*"), std::move(ra2));

    auto rb1 = std::make_unique<LiteralExpression>(STRING, "B");
    auto rb2 = std::make_unique<LiteralExpression>(STRING, "B");
    auto rMultB = std::make_unique<BinaryExpression>(std::move(rb1), Token(STAR, 0, 1, "*"), std::move(rb2));
    auto rabPlus = std::make_unique<BinaryExpression>(std::move(rMultA), Token(PLUS, 0, 1, "+"), std::move(rMultB));

    auto r2 = std::make_unique<LiteralExpression>(NUMBER, "2");
    auto ra3 = std::make_unique<LiteralExpression>(STRING, "A");
    auto rb3 = std::make_unique<LiteralExpression>(STRING, "B");

    auto rAB = std::make_unique<BinaryExpression>(std::move(ra3), Token(STAR, 0, 1, "*"), std::move(rb3));
    auto rMult = std::make_unique<BinaryExpression>(std::move(r2), Token(STAR, 0, 1, "*"), std::move(rAB));
    auto replacement = std::make_unique<BinaryExpression>(std::move(rMult), Token(PLUS, 0, 1, "+"), std::move(rabPlus));

    // Source
    // (69+420)*(69+420)
    auto l1 = std::make_unique<LiteralExpression>(NUMBER, "69");
    auto l2 = std::make_unique<LiteralExpression>(NUMBER, "420");
    auto sPlus1 = std::make_unique<BinaryExpression>(std::move(l1), Token(PLUS, 0, 1, "+"), std::move(l2));

    auto re1 = std::make_unique<LiteralExpression>(NUMBER, "69");
    auto re2 = std::make_unique<LiteralExpression>(NUMBER, "420");
    auto sPlus2 = std::make_unique<BinaryExpression>(std::move(re1), Token(PLUS, 0, 1, "+"), std::move(re2));

    auto sMult = std::make_unique<BinaryExpression>(std::move(sPlus1), Token(STAR, 0, 1, "*"), std::move(sPlus2));

    // Applied source
    // "A"*"A" + "B"*"B" + 2*"A"*"B"
    ExpressionRewriter rewriter;
    AstPrinter printer;
    auto applied = rewriter.substitute(std::move(sMult), std::move(mult), std::move(replacement));
    CHECK(printer.print(*applied.get()) == "((2 * (69 * 420)) + ((69 * 69) + (420 * 420)))");
}
