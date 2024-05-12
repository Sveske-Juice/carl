#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "lexer/lexer.h"
#include "parser/ast_printer.h"
#include "parser/parser.h"
#include "runtime/expressionrewriter.h"
#include "runtime/interpreter.h"

TEST_CASE("A + 0 -> A") {
    Lexer lexer{"define zero -> \"A\" + 0 = \"A\";apply zero : 2 + 0;"};
    Parser parser{lexer.tokenize()};
    auto ast = parser.parse();
    Interpreter interpreter{std::move(ast)};

    auto result = interpreter.interpret();
    CHECK(interpreter.result == "2");
}

TEST_CASE("A + B -> B + A") {
    Lexer lexer{"define abrule -> \"A\" + \"B\" = \"B\" + \"A\";apply abrule : 2 + 5;"};
    Parser parser{lexer.tokenize()};
    auto ast = parser.parse();
    Interpreter interpreter{std::move(ast)};

    auto result = interpreter.interpret();
    CHECK(interpreter.result == "(5+2)");
}

TEST_CASE("square") {
    Lexer lexer{"define square -> (\"A\" + \"B\")*(\"A\" + \"B\") = \"A\"*\"A\" + 2*\"A\"*\"B\";apply square : (69+420)*(69+420);"};
    Parser parser{lexer.tokenize()};
    auto ast = parser.parse();
    Interpreter interpreter{std::move(ast)};

    auto result = interpreter.interpret();
    CHECK(interpreter.result == "((69*69)+((2*69)*420))");
}
