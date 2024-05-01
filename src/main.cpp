#include "lexer/lexer.h"
#include "lexer/lexer_errors.h"
#include "lexer/token.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int main (int argc, char *argv[]) {
    std::string src{"!= ; ? \t == # this is a comment \n\"This is my nice string\" ! (55) {!=} 4.42 not and if myidentifier myvariable while \"this is an unterminated string \" 5."};
    Lexer lexer(src);

    std::vector<Token> tokens;
    try {
        tokens = lexer.tokenize();
    }
    catch(const LexerException& le) {
        std::string_view sv{src.begin(), src.end()};
        std::cerr << "Lexer Error: " << le.what(sv) << std::endl;

        exit(le.error_code());
    }

    printf("\nTokens:\n");
    for (Token token : tokens) {
        printf("%s(%s)\n", TokenTypesToString[token.type()].data(), token.literal().data());
    }
    return 0;
}
