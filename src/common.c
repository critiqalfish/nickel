#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void initTokenBox(TokenBox *tb) {
    size_t initialSize = 1;
    tb->tokens = (Token *) malloc(initialSize * sizeof(Token));
    if (tb->tokens == NULL) {
        fprintf(stderr, "Memory allocation error in initTokenBox!\n");
        exit(1);
    }
    tb->used = 0;
    tb->size = initialSize;
}

void insertToken(TokenBox *tb, Token token) {
    if (tb->used == tb->size) {
        Token *newTokens = (Token *) realloc(tb->tokens, tb->size * 2 * sizeof(Token));
        if (newTokens == NULL) {
            fprintf(stderr, "Memory allocation error in insertToken!\n");
            exit(1);
        }
        tb->tokens = newTokens;
        tb->size *= 2;
    }
    tb->tokens[tb->used++] = token;
}

void freeTokenBox(TokenBox *tb) {
    free(tb->tokens);
    tb->tokens = NULL;
    tb->used = tb->size = 0;
}

int checkBounds(int length, int iteration, int ahead) {
    if (iteration + ahead < length) return 1;
    else return 0;
}