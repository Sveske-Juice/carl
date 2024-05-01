#include "lexer/lexer.h"
#include "lexer/lexer_errors.h"
#include "lexer/token.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

int main (int argc, char *argv[]) {
    std::string src{"!= ; \t == # this is a comment \n\"This is my nice string\" ! (55) {!=} 4.42 not and if myidentifier myvariable while \"this is an unterminated string "};
    Lexer lexer(src);

    std::vector<Token> tokens;
    try {
        tokens = lexer.tokenize();
    }
    catch(const LexerException& le) {
        std::cerr << "Error occured while lexing: " << le.what() << std::endl;
        exit(le.error_code());
    }

    printf("\nTokens:\n");
    for (Token token : tokens) {
        printf("%s(%s)\n", TokenTypesToString[token.type()].data(), token.literal().data());
    }
    return 0;
}
