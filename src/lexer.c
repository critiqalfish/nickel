#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lex(TokenBox *tb, FILE *source) {
    Lexer lx;
    lx.location = 0;
    readWholeFile(&lx, source);
    //printf("%s\n", lx.sourceContent);
    printf("chars until whitespace: %d\n", charsUntilWhitespace(&lx));


    return 0;
}

static void readWholeFile(Lexer *lx, FILE *source) {
    fseek(source, 0, SEEK_END);
    long fsize = ftell(source);
    fseek(source, 0, SEEK_SET);

    lx->fileSize = fsize;

    char *fileContents = (char *) malloc(fsize + 1);
    fread(fileContents, 1, fsize, source);

    lx->sourceContent = fileContents;
}

static int charsUntilWhitespace(Lexer *lx) {
    int length = 0;
    for (long i = lx->location; i < lx->fileSize && lx->sourceContent[i] != ' '; i++) {
        length++;
    }

    char substring[length + 1];
    strncpy(substring, lx->sourceContent + lx->location, length);
    substring[length] = '\0';
    printf("substr: '%s'\n", substring);

    return length;
}