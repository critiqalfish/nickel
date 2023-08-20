#include <stdio.h>
#include "common.h"
#include "lexer.h"

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

    freeTokenBox(&tb);
    fclose(source);
    return 0;
}