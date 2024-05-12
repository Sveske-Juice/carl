#include "lexer/token.h"
#include "parser/expression.h"
#include "parser/statement.h"
#include "runtime/carl_object.h"
#include "runtime/interpreter.h"
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <iostream>
#include <memory>

TEST_CASE("Intepreter Literal tests", "[Intepreter:Literal]") {
    // Primitive
    SECTION("True") {
        std::unique_ptr<Expression> l =
            std::make_unique<LiteralExpression>(TokenType::TRUE, "true");
        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(l));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        CHECK(res.value().type == ValueType::VALUE_BOOL);
        CHECK(res.value().number == 1.0);
        res.value().dispose();
    }
    SECTION("False") {
        std::unique_ptr<Expression> l =
            std::make_unique<LiteralExpression>(TokenType::FALSE, "false");
        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(l));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        CHECK(res.value().type == ValueType::VALUE_BOOL);
        CHECK(res.value().number == 0.0);
        res.value().dispose();
    }
    SECTION("Number") {
        std::unique_ptr<Expression> l =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(l));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        CHECK(res.value().type == ValueType::VALUE_NUMBER);
        CHECK(res.value().number == 69.0);
        res.value().dispose();
    }

    // Object literals
    SECTION("Strings") {
        std::unique_ptr<Expression> l =
            std::make_unique<LiteralExpression>(TokenType::STRING, "HEHEHEAW");
        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(l));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        CHECK(res.value().type == ValueType::VALUE_OBJ);

        CHECK(res.value().obj->objType == ObjectType::OBJ_STRING);
        const char *is = reinterpret_cast<ObjString *>(res.value().obj)->chars;
        CHECK(strcmp(is, "HEHEHEAW") == 0);
        res.value().dispose();
    }
}

TEST_CASE("Interpreter Unary Operations", "[Interpreter:Unary]") {
    SECTION("Negate") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<Expression> u = std::make_unique<UnaryExpression>(
            Token(TokenType::MINUS, 0, 1, "-"), std::move(l));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(u));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_NUMBER);
        REQUIRE(res.value().number == -69.0);
    }

    SECTION("Chained negates") {
        for (int i = 1; i < 69; i++) {
            std::unique_ptr<LiteralExpression> l =
                std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
            std::unique_ptr<Expression> uFinal =
                std::make_unique<UnaryExpression>(
                    Token(TokenType::MINUS, 0, 1, "-"), std::move(l));
            for (int j = 0; j < i; j++) {
                uFinal = std::make_unique<UnaryExpression>(
                    Token(TokenType::MINUS, 0, 1, "-"), std::move(uFinal));
            }
            std::unique_ptr<Statement> s =
                std::make_unique<ExpressionStatement>(std::move(uFinal));
            Interpreter interpreter(std::move(s));
            auto res = interpreter.interpret();
            double correct = i % 2 == 0 ? -69 : 69;
            REQUIRE(res.value().type == ValueType::VALUE_NUMBER);
            REQUIRE(res.value().number == correct);
        }
    }

    SECTION("NOT1") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::TRUE, "true");
        std::unique_ptr<Expression> u = std::make_unique<UnaryExpression>(
            Token(TokenType::BANG, 0, 1, "!"), std::move(l));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(u));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_BOOL);
        REQUIRE(res.value().number == 0.0);
    }

    SECTION("NOT2") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::FALSE, "false");
        std::unique_ptr<Expression> u = std::make_unique<UnaryExpression>(
            Token(TokenType::BANG, 0, 1, "!"), std::move(l));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(u));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_BOOL);
        REQUIRE(res.value().number == 1.0);
    }

    SECTION("Chained NUTS") {
        for (int i = 1; i < 69; i++) {
            std::unique_ptr<LiteralExpression> l =
                std::make_unique<LiteralExpression>(TokenType::TRUE, "true");
            std::unique_ptr<Expression> uFinal =
                std::make_unique<UnaryExpression>(
                    Token(TokenType::BANG, 0, 1, "!"), std::move(l));
            for (int j = 0; j < i; j++) {
                uFinal = std::make_unique<UnaryExpression>(
                    Token(TokenType::BANG, 0, 1, "!"), std::move(uFinal));
            }
            std::unique_ptr<Statement> s =
                std::make_unique<ExpressionStatement>(std::move(uFinal));
            Interpreter interpreter(std::move(s));
            auto res = interpreter.interpret();
            double correct = i % 2 == 0 ? 0.0 : 1.0;
            REQUIRE(res.value().type == ValueType::VALUE_BOOL);
            REQUIRE(res.value().number == correct);
        }
    }
}

TEST_CASE("Interpreter Binary Operations", "[Interpreter:Binary]") {
    SECTION("Plus") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(
            std::move(l), Token(TokenType::PLUS, 0, 1, "+"), std::move(r));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(bin));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_NUMBER);
        REQUIRE(res.value().number == 69 + 420);
    }

    SECTION("Minus") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(
            std::move(l), Token(TokenType::MINUS, 0, 1, "-"), std::move(r));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(bin));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_NUMBER);
        REQUIRE(res.value().number == 69 - 420);
    }

    SECTION("Multiplication") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(
            std::move(l), Token(TokenType::STAR, 0, 1, "*"), std::move(r));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(bin));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_NUMBER);
        REQUIRE(res.value().number == 69 * 420);
    }

    SECTION("Division") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "69");
        std::unique_ptr<LiteralExpression> r =
            std::make_unique<LiteralExpression>(TokenType::NUMBER, "420");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(
            std::move(l), Token(TokenType::SLASH, 0, 1, "/"), std::move(r));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(bin));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_NUMBER);
        REQUIRE(res.value().number == (double)69.0 / (double)420.0);
    }

    SECTION("String Concat") {
        std::unique_ptr<LiteralExpression> l =
            std::make_unique<LiteralExpression>(TokenType::STRING, "Hello ");
        std::unique_ptr<LiteralExpression> r =
            std::make_unique<LiteralExpression>(TokenType::STRING, "World!");
        std::unique_ptr<Expression> bin = std::make_unique<BinaryExpression>(
            std::move(l), Token(TokenType::PLUS, 0, 1, "+"), std::move(r));

        std::unique_ptr<Statement> s =
            std::make_unique<ExpressionStatement>(std::move(bin));
        Interpreter interpreter(std::move(s));
        auto res = interpreter.interpret();
        REQUIRE(res.value().type == ValueType::VALUE_OBJ);

        ObjString *resultStr = reinterpret_cast<ObjString *>(res.value().obj);
        REQUIRE(strcmp("Hello World!", resultStr->chars) == 0);
    }
}
