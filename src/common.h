#pragma once
#include <stddef.h>

typedef enum {
    tINT,
    tINTLIT,
    tFUNCTIONARROW,
    tMAIN,
    tLPAREN,
    tRPAREN,
    tLBRACE,
    tRBRACE,
    tIDENTIFIER,
    tSTRINGLIT,
    tSEMICOL,
    tBACK
} TokenType;

typedef struct {
    TokenType *tokens;
    size_t used;
    size_t size;
} TokenBox;

void initTokenBox(TokenBox *tb);
void insertToken(TokenBox *tb, TokenType element);
void freeTokenBox(TokenBox *tb);