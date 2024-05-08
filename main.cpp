#include "lexer/lexer.h"
#include "lexer/lexer_errors.h"
#include "parser/parser.h"
#include "parser/parser_errors.h"
#include "lexer/token.h"
#include "runtime/interpreter.h"
#include "runtime/runtime_errors.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int main (int argc, char *argv[]) {
    if (argc < 2) exit(1);

    std::string src{argv[1]};
    Lexer lexer(src);

    std::vector<Token> tokens;
    try {
        tokens = lexer.tokenize();

        printf("\nTokens:\n");
        for (Token token : tokens) {
            printf("%s(%s, off: %d, l: %d)\n", TokenTypesToString[token.type()].data(), token.literal().data(), token.sourceOffset(), token.length());
        }
        Parser parser{tokens};
        std::unique_ptr<Expression> root = parser.parse();
        Interpreter interpreter(std::move(root));
        Value output = interpreter.interpret();
        std::cout << output.toString() << std::endl;
    }
    catch(const LexerException& le) {
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Lexer Error: " << le.what(sv) << std::endl;

        exit(le.error_code());
    }
    catch(const ParserException& pe) {
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Parser Error: " << pe.what(sv) << std::endl;

        exit(pe.error_code());
    }
    catch (const RuntimeException& re){
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Runtime Error: " << re.what(sv) << std::endl;

        exit(re.error_code());
    }

    return 0;
}
