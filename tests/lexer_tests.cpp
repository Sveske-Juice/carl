#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string>

#include "lexer/lexer.h"
#include "lexer/token.h"

TEST_CASE("EOF") {
    Lexer lexer{std::string{}};
    auto t = lexer.tokenize();
    REQUIRE(t.size() == 1);
    REQUIRE(t[0].type() == TokenType::END_OF_FILE);
}

TEST_CASE("Testing single tokens", "[Single tokens]") {
    SECTION("Plus") {
        std::string src{"+"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == PLUS);
    }
    SECTION("Minus") {
        std::string src{"-"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == MINUS);
    }
    SECTION("Star") {
        std::string src{"*"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == STAR);
    }
    SECTION("Slash") {
        std::string src{"/"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == SLASH);
    }
    SECTION("Modulus") {
        std::string src{"%"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == MODULUS);
    }
    SECTION("Exponent") {
        std::string src{"^"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == EXPONENT);
    }
    // TODO: brackets (too lazy)
    SECTION("Colon") {
        std::string src{":"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == COLON);
    }
    SECTION("Semicolon") {
        std::string src{";"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == SEMICOLON);
    }
    SECTION("Comma") {
        std::string src{","}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == COMMA);
    }
    SECTION("Dot") {
        std::string src{"."}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == DOT);
    }
    SECTION("Bang") {
        std::string src{"!"}; Lexer lexer{src}; auto t = lexer.tokenize(); TokenType type = t[0].type();
        CHECK(type == BANG);
    }
}

