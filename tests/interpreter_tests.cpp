#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <iostream>
#include <memory>
#include "lexer/token.h"
#include "parser/expression.h"
#include "runtime/carl_object.h"
#include "runtime/interpreter.h"

TEST_CASE("Intepreter Literal tests", "[Intepreter:Literal]") {
    // Primitive
    SECTION("True") {
        std::unique_ptr<Expression> l = std::make_unique<LiteralExpression>(TokenType::TRUE, "true");
        Interpreter interpreter(std::move(l));
        Value res = interpreter.interpret();
        CHECK(res.type == ValueType::VALUE_BOOL);
        CHECK(res.number == 1.0);
        res.dispose();
    }
    SECTION("False") {
        std::unique_ptr<Expression> l = std::make_unique<LiteralExpression>(TokenType::FALSE, "false");
        Interpreter interpreter(std::move(l));
        Value res = interpreter.interpret();
        CHECK(res.type == ValueType::VALUE_BOOL);
        CHECK(res.number == 0.0);
        res.dispose();
    }
    SECTION("Number") {
        std::unique_ptr<Expression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        Interpreter interpreter(std::move(l));
        Value res = interpreter.interpret();
        CHECK(res.type == ValueType::VALUE_NUMBER);
        CHECK(res.number == 69.0);
        res.dispose();
    }

    // Object literals
    SECTION("Strings") {
        std::unique_ptr<Expression> l = std::make_unique<LiteralExpression>(TokenType::STRING, "HEHEHEAW");
        Interpreter interpreter(std::move(l));
        Value res = interpreter.interpret();
        CHECK(res.type == ValueType::VALUE_OBJ);

        CHECK(res.obj->objType == ObjectType::OBJ_STRING);
        const char* is = reinterpret_cast<ObjString*>(res.obj)->chars;
        CHECK(strcmp(is, "HEHEHEAW") == 0);
        res.dispose();
    }
}

TEST_CASE("Interpreter Unary Operations", "[Interpreter:Unary]") {
    SECTION("Negate") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<Expression> u = std::make_unique<UnaryExpression>(Token(TokenType::MINUS, 0, 1, "-"), std::move(l));

        Interpreter interpreter{std::move(u)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_NUMBER);
        REQUIRE(res.number == -69.0);
    }

    SECTION("Chained negates") {
        for (int i = 1; i < 69; i++) {
            std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
            std::unique_ptr<Expression> uFinal = std::make_unique<UnaryExpression>(Token(TokenType::MINUS, 0, 1, "-"), std::move(l));
            for (int j = 0; j < i; j++) {
                uFinal = std::make_unique<UnaryExpression>(Token(TokenType::MINUS, 0, 1, "-"), std::move(uFinal));
            }
            Interpreter interpreter{std::move(uFinal)};
            Value res = interpreter.interpret();
            double correct = i % 2 == 0 ? -69 : 69;
            REQUIRE(res.type == ValueType::VALUE_NUMBER);
            REQUIRE(res.number == correct);
        }
    }

    SECTION("NOT1") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::TRUE, "true");
        std::unique_ptr<Expression> u = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(l));

        Interpreter interpreter{std::move(u)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_BOOL);
        REQUIRE(res.number == 0.0);
    }

    SECTION("NOT2") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::FALSE, "false");
        std::unique_ptr<Expression> u = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(l));

        Interpreter interpreter{std::move(u)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_BOOL);
        REQUIRE(res.number == 1.0);
    }

    SECTION("Chained NUTS") {
        for (int i = 1; i < 69; i++) {
            std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::TRUE, "true");
            std::unique_ptr<Expression> uFinal = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(l));
            for (int j = 0; j < i; j++) {
                uFinal = std::make_unique<UnaryExpression>(Token(TokenType::BANG, 0, 1, "!"), std::move(uFinal));
            }
            Interpreter interpreter{std::move(uFinal)};
            Value res = interpreter.interpret();
            double correct = i % 2 == 0 ? 0.0 : 1.0;
            REQUIRE(res.type == ValueType::VALUE_BOOL);
            REQUIRE(res.number == correct);
        }
    }
}

TEST_CASE("Interpreter Binary Operations", "[Interpreter:Binary]") {
    SECTION("Plus") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r = std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(std::move(l), Token(TokenType::PLUS, 0, 1, "+"), std::move(r));

        Interpreter interpreter{std::move(bin)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_NUMBER);
        REQUIRE(res.number == 69 + 420);
    }

    SECTION("Minus") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r = std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(std::move(l), Token(TokenType::MINUS, 0, 1, "-"), std::move(r));

        Interpreter interpreter{std::move(bin)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_NUMBER);
        REQUIRE(res.number == 69 - 420);
    }

    SECTION("Multiplication") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r = std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(std::move(l), Token(TokenType::STAR, 0, 1, "*"), std::move(r));

        Interpreter interpreter{std::move(bin)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_NUMBER);
        REQUIRE(res.number == 69 * 420);
    }

    SECTION("Division") {
        std::unique_ptr<LiteralExpression> l = std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r = std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(std::move(l), Token(TokenType::SLASH, 0, 1, "/"), std::move(r));

        Interpreter interpreter{std::move(bin)};
        Value res = interpreter.interpret();
        REQUIRE(res.type == ValueType::VALUE_NUMBER);
        REQUIRE(res.number == (double)69.0 / (double)420.0);
    }
}
