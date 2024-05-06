#include "lexer/token.h"
#include "parser/ast_printer.h"
#include "parser/expression.h"
#include "parser/parser.h"
#include "parser/parser_errors.h"
#include <catch2/catch_test_macros.hpp>
#include <exception>
#include <iostream>

TEST_CASE("Parser Literals", "[Parser:Literals]") {
    // TODO: should not rely on the printer to verify the results.

    SECTION("numbers") {
        AstPrinter printer;
        std::vector<Token> lexemes = {
            Token(TokenType::NUMBER, 0, 1, "69"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == "69");
    }

    SECTION("strings") {
        AstPrinter printer;
        std::string str = "MY PERFECT STRING";
        std::vector<Token> lexemes = {
            Token(TokenType::STRING, 0, 1, str),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == str);
    }
}

TEST_CASE("Parser Unary", "[Parser:Unary]") {

    SECTION("Single Unary 1") {
        AstPrinter printer;
        std::vector<Token> lexemes = {
            Token(TokenType::MINUS, 0, 1, "-"),
            Token(TokenType::NUMBER, 0, 1, "69"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == "-69");
    }

    SECTION("Multiple Unary") {
        for (int i = 1; i < 69; i++) {
            AstPrinter printer;
            std::vector<Token> lexemes;
            for (int j = 0; j < i; j++) {
                lexemes.push_back(Token(TokenType::BANG, 0, 1, "!"));
            }
            lexemes.push_back(Token(TokenType::TRUE, 0, 1, "true"));
            lexemes.push_back(Token(TokenType::END_OF_FILE, 0, 1, "\0"));

            Parser parser{lexemes};
            std::unique_ptr<Expression> expression = parser.parse();
            std::string out = printer.print(*expression);

            std::string correct;
            for (int j = 0; j < i; j++) {
                correct += '!';
            }
            correct += "true";

            CHECK(out == correct);
        }
        for (int i = 1; i < 69; i++) {
            AstPrinter printer;
            std::vector<Token> lexemes;
            for (int j = 0; j < i; j++) {
                lexemes.push_back(Token(TokenType::MINUS, 0, 1, "-"));
            }
            lexemes.push_back(Token(TokenType::TRUE, 0, 1, "69"));
            lexemes.push_back(Token(TokenType::END_OF_FILE, 0, 1, "\0"));

            Parser parser{lexemes};
            std::unique_ptr<Expression> expression = parser.parse();
            std::string out = printer.print(*expression);

            std::string correct;
            for (int j = 0; j < i; j++) {
                correct += '-';
            }
            correct += "69";

            CHECK(out == correct);
        }
    }
}

TEST_CASE("Parser Binary", "[Parser:Binary]") {
    SECTION("Plus") {
        AstPrinter printer;
        std::vector<Token> lexemes = {
            Token(TokenType::NUMBER, 0, 1, "69"),
            Token(TokenType::PLUS, 0, 1, "+"),
            Token(TokenType::NUMBER, 0, 1, "420"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == "(69 + 420)");
    }
    SECTION("Minus") {
        AstPrinter printer;
        std::vector<Token> lexemes = {
            Token(TokenType::NUMBER, 0, 1, "69"),
            Token(TokenType::MINUS, 0, 1, "-"),
            Token(TokenType::NUMBER, 0, 1, "420"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == "(69 - 420)");
    }
    SECTION("Mult") {
        AstPrinter printer;
        std::vector<Token> lexemes = {
            Token(TokenType::NUMBER, 0, 1, "69"),
            Token(TokenType::STAR, 0, 1, "*"),
            Token(TokenType::NUMBER, 0, 1, "420"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == "(69 * 420)");
    }
    SECTION("Divide") {
        AstPrinter printer;
        std::vector<Token> lexemes = {
            Token(TokenType::NUMBER, 0, 1, "69"),
            Token(TokenType::SLASH, 0, 1, "/"),
            Token(TokenType::NUMBER, 0, 1, "420"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        std::unique_ptr<Expression> expression = parser.parse();
        std::string out = printer.print(*expression);

        CHECK(out == "(69 / 420)");
    }
}

TEST_CASE("Parser Exceptions", "[Parser:Exceptions]") {
    SECTION("Unexpected Token") {
        std::vector<Token> lexemes = {
            Token(TokenType::PLUS, 0, 1, "+"),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};
        REQUIRE_THROWS_AS(parser.parse(), UnexpectedToken);
    }
    SECTION("Missing Closing Bracket") {
        std::vector<Token> lexemes = {
            Token(TokenType::LPAREN, 0, 1, "("),
            Token(TokenType::END_OF_FILE, 0, 1, "\0"),
        };
        Parser parser{lexemes};

        // For some reason if i use CHECK_THROWS_AS here catch2 throws an std::exception
        // maybe this is a bug with catch2?
        try {
            parser.parse();
            FAIL();
        } catch(const ParserException& e){
            SUCCEED();
        }
    }
}

