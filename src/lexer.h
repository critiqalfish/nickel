#pragma once
#include "common.h"
#include <stdio.h>

typedef struct {
    const char *sourceContent;
    long location;
    long fileSize;
} Lexer;

int lex(TokenBox *tb, FILE *source);
static int eatTokens(Lexer *lx, TokenBox *tb);
static int lookAheadChars(Lexer *lx, const char *chars);
static void readWholeFile(Lexer *lx, FILE *source);