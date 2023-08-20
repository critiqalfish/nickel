#pragma once
#include "common.h"
#include <stdio.h>

typedef struct {
    const char *sourceContent;
    long location;
    long fileSize;
} Lexer;

int lex(TokenBox *tb, FILE *source);
static void readWholeFile(Lexer *lx, FILE *source);
static int charsUntilWhitespace(Lexer *lx);