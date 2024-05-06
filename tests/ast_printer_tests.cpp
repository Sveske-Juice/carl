#include "lexer/token.h"
#include "parser/ast_printer.h"
#include "parser/expression.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("Ast Printer Test", "[AST Printer]") {
    AstPrinter printer;

    SECTION("Literals") {
        LiteralExpression l("69");
        CHECK(printer.print(l) == "69");
    }

    SECTION("Tree Structure Verification") {
        std::unique_ptr<Expression> l1 = std::make_unique<LiteralExpression>("69");
        std::unique_ptr<Expression> l2 = std::make_unique<LiteralExpression>("55");
        std::unique_ptr<Expression> l3 = std::make_unique<LiteralExpression>("2");
        std::unique_ptr<Expression> opInner = std::make_unique<BinaryExpression>(std::move(l2), Token(TokenType::STAR, 0, 1, "*"), std::move(l3));
        BinaryExpression op(std::move(l1), Token(TokenType::PLUS, 0, 1, "+"), std::move(opInner));

        CHECK(printer.print(op) == "(69 + (55 * 2))");
    }

    SECTION("Tree Structure Verification 2") {
        std::unique_ptr<Expression> l1 = std::make_unique<LiteralExpression>("true");
        std::unique_ptr<Expression> u1 = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(l1));
        std::unique_ptr<Expression> u2 = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(u1));
        std::unique_ptr<Expression> u3 = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(u2));
        std::unique_ptr<Expression> u4 = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(u3));

        CHECK(printer.print(*u4) == "!!!!true");
    }
}
