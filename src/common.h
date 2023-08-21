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
    TokenType type;
    const char* value;
} Token;

typedef struct {
    Token *tokens;
    size_t used;
    size_t size;
} TokenBox;

void initTokenBox(TokenBox *tb);
void insertToken(TokenBox *tb, Token token);
void freeTokenBox(TokenBox *tb);