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
