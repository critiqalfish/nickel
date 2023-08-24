#include <stdio.h>
#include "common.h"
#include "lexer.h"

#define DEBUG 1

int main(int argc, char* argv[]) {
    printf("Nickel v0\n");
    if (argc != 2) {
        fprintf(stderr, "\nIncorrect usage!\nCorrect usage: nickel input.nc\n");
        return 1;
    }

    const char *filename = argv[1];
    FILE *source = fopen(filename, "rb");
    if (source == NULL) {
        fprintf(stderr, "\nInput file \"%s\" does not exist!\n", filename);
        return 1;
    }

    printf("\nBeginning lexing process...\n");

    TokenBox tb;
    initTokenBox(&tb);

    int lexStatus = lex(&tb, source);

    #if DEBUG
    printf("\ntokens: %zu\n", tb.used);
    for (int i = 0; i < tb.used; i++) {
        printf("token %d: type %d, value: %s\n", i, tb.tokens[i].type, tb.tokens[i].value);
    }
    printf("\n");
    #endif

    if (lexStatus) {
        printf("Lexer encountered an error!\n");
        return 1;
    }

    printf("Lexing was successful!\n");

    freeTokenBox(&tb);
    fclose(source);
    return 0;
}