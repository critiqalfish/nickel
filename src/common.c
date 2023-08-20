#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void initTokenBox(TokenBox *tb) {
    size_t initialSize = 1;
    tb->tokens = (TokenType *) malloc(initialSize * sizeof(TokenType));
    if (tb->tokens == NULL) {
        fprintf(stderr, "Memory allocation error in initTokenBox!\n");
        exit(1);
    }
    tb->used = 0;
    tb->size = initialSize;
}

void insertToken(TokenBox *tb, TokenType element) {
    if (tb->used == tb->size) {
        tb->tokens = (TokenType *) realloc(tb->tokens, tb->size * 1.5 * sizeof(TokenType));
        if (sizeof(tb->tokens) == tb->size * sizeof(TokenType)) {
            fprintf(stderr, "Memory allocation error in insertToken!\n");
            exit(1);
        }
        tb->size *= 1.5;
    }
    tb->tokens[tb->used++] = element;
}

void freeTokenBox(TokenBox *tb) {
    free(tb->tokens);
    tb->tokens = NULL;
    tb->used = tb->size = 0;
}