#include "lexer/token.h"
#include "parser/ast_printer.h"
#include "parser/expression.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Ast Printer Test", "[AST Printer]") {
    AstPrinter printer;

    SECTION("Literals") {
        LiteralExpression l("69");
        CHECK(printer.print(l) == "69");
    }

    SECTION("Tree Structure Verification") {
        LiteralExpression l1("69");
        LiteralExpression l2("55");
        LiteralExpression l3("2");
        BinaryExpression opInner(l2, Token(TokenType::STAR, 0, 1, "*"), l3);
        BinaryExpression op(l1, Token(TokenType::PLUS, 0, 1, "+"), opInner);

        CHECK(printer.print(op) == "(69 + (55 * 2))");
    }
}
