#include "lexer.h"
#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lex(TokenBox *tb, FILE *source) {
    Lexer lx;
    lx.location = 0;
    readWholeFile(&lx, source);
    //printf("%s\n", lx.sourceContent);
    
    int eatStatus = eatTokens(&lx, tb);

    return eatStatus;
}

static int eatTokens(Lexer *lx, TokenBox *tb) {
    while (lx->location <= lx->fileSize) {
        //printf("\n%ld: %c", lx->location, lx->sourceContent[lx->location]);
        if (lx->sourceContent[lx->location] == ' ' || lx->sourceContent[lx->location] == '\n' || lx->sourceContent[lx->location] == '\t') {
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == ';') {
            Token token = {tSEMICOL};
            insertToken(tb, token);
            lx->location++;
        }
        else if (!lookAheadChars(lx, "<- ")) {
            Token token = {tFUNCTIONARROW};
            insertToken(tb, token);
            lx->location += 3;
        }
        else if (lx->sourceContent[lx->location] == '(') {
            Token token = {tLPAREN};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == ')') {
            Token token = {tRPAREN};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == '{') {
            Token token = {tLBRACE};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == '}') {
            Token token = {tRBRACE};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == '+') {
            Token token = {tPLUS};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == '-') {
            Token token = {tMINUS};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == '*') {
            Token token = {tASTER};
            insertToken(tb, token);
            lx->location++;
        }
        else if (lx->sourceContent[lx->location] == '/') {
            Token token = {tDIV};
            insertToken(tb, token);
            lx->location++;
        }
        else if (!lookAheadChars(lx, "int")) {
            Token token = {tINT};
            insertToken(tb, token);
            lx->location += 3;
        }
        else if (lx->sourceContent[lx->location] == '"') {
            long start = lx->location;
            lx->location++;

            while (lx->location < lx->fileSize && lx->sourceContent[lx->location] != '"' || lx->sourceContent[lx->location] == '"' && lx->sourceContent[lx->location - 1] == '\\') {
                lx->location++;
            }

            if (lx->location < lx->fileSize && lx->sourceContent[lx->location] == '"') {
                Token token = {tSTRINGLIT};
                token.value = strndup(&lx->sourceContent[start], lx->location - start + 1);
                insertToken(tb, token);
                lx->location++;
            }
            else {
                return 1;
            }
        }
        else if (isdigit(lx->sourceContent[lx->location])) {
            long start = lx->location;

            while (lx->location < lx->fileSize && isdigit(lx->sourceContent[lx->location])) {
                lx->location++;
            }
            
            Token token = {tINTLIT};
            token.value = strndup(&lx->sourceContent[start], lx->location - start);
            insertToken(tb, token);
        }
        else if (isalpha(lx->sourceContent[lx->location])) {
            long start = lx->location;

            while (lx->location < lx->fileSize && isalpha(lx->sourceContent[lx->location])) {
                lx->location++;
            }

            Token token = {};
            token.value = strndup(&lx->sourceContent[start], lx->location - start);
            if (strcmp(token.value, "main") == 0 || strcmp(token.value, "back") == 0) {
                token.type = tKEYWORD;
            }
            else {
                token.type = tIDENTIFIER;
            }
            insertToken(tb, token);
        }
        else {
            lx->location++;
        }
    }
    return 0;
}

static int lookAheadChars(Lexer *lx, const char* chars) {
    int length = strlen(chars);

    if (lx->location + length - 1 > lx->fileSize) {
        return 1;
    }

    for (long i = 0; i < length; i++) {
        if (lx->sourceContent[lx->location + i] != chars[i]) {
            return 1;
        }
    }
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