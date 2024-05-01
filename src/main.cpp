#include "lexer/lexer.h"
#include "lexer/token.h"

#include <cstdio>
#include <string_view>
#include <vector>

int main (int argc, char *argv[]) {
    std::string src{"!= ; \t == # this is a comment \n\"This is my nice string\" ! () {!=}"};
    Lexer lexer(src);
    std::vector<Token> tokens = lexer.tokenize();
    printf("\nTokens:\n");
    for (Token token : tokens) {
        printf("%s(%s)\n", TokenTypesToString[token.type()].data(), token.literal().data());
    }
    return 0;
}
