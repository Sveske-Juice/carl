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

TEST_CASE("Single token") {
    std::string src{"!"};
    Lexer lexer{src};
    auto t = lexer.tokenize();

    REQUIRE(t.size() == 2); // BANG + EOF
    REQUIRE(t[0].type() == TokenType::BANG);
}

