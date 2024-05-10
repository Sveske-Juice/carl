#include "lexer/lexer.h"
#include "lexer/lexer_errors.h"
#include "parser/parser.h"
#include "parser/parser_errors.h"
#include "runtime/interpreter.h"
#include "runtime/runtime_errors.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#define PROMPT "> "
#define OUTPUT_PADDING "\t# "

int main(int argc, char *argv[]) {
    // Repl mode
    if (argc < 2) {
        Interpreter interpreter;
        while (true) {
            std::cout << PROMPT;
            std::string input;
            std::getline(std::cin, input);

            if (input == "quit" || input == "q" || input == ":q" || input == "exit")
                return 0;

            try {
                // Lexing
                Lexer lexer{input};
                auto tokens = lexer.tokenize();

                // Parsing
                Parser parser{tokens};
                auto statements = parser.parse();

                // Interpret
                std::optional<Value> result = interpreter.interpret(std::move(statements));
                if (result.has_value()) {
                    std::cout << OUTPUT_PADDING << result->toString() << std::endl;
                }
            } catch (const LexerException &le) {
                std::string_view sv{input.begin(), input.end()};
                std::cerr << "Lexer Error: " << le.what(sv) << std::endl;
            } catch (const ParserException &pe) {
                std::string_view sv{input.begin(), input.end()};
                std::cerr << "Parser Error: " << pe.what(sv) << std::endl;
            } catch (const RuntimeException &re) {
                std::string_view sv{input.begin(), input.end()};
                std::cerr << "Runtime Error: " << re.what(sv) << std::endl;
            }
        }

        return 0;
    }
    // One line mode
    std::string src{argv[1]};
    Lexer lexer(src);

    std::vector<Token> tokens;
    try {
        tokens = lexer.tokenize();

        printf("\nTokens:\n");
        for (Token token : tokens) {
            printf("%s(%s, off: %d, l: %d)\n",
                   TokenTypesToString[token.type()].data(),
                   token.literal().data(), token.sourceOffset(),
                   token.length());
        }
        Parser parser{tokens};
        auto statements = parser.parse();
        Interpreter interpreter(std::move(statements));

        auto lastOutput = interpreter.interpret();
        if (lastOutput.has_value()) {
            lastOutput.value().dispose();
            std::cout << lastOutput.value().toString() << std::endl;
        }
    } catch (const LexerException &le) {
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Lexer Error: " << le.what(sv) << std::endl;

        exit(le.error_code());
    } catch (const ParserException &pe) {
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Parser Error: " << pe.what(sv) << std::endl;

        exit(pe.error_code());
    } catch (const RuntimeException &re) {
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Runtime Error: " << re.what(sv) << std::endl;

        exit(re.error_code());
    }

    return 0;
}
