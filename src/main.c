#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "lexer.h"
#include "parser.h"

#define DEBUG 1

void printAST(TreeNode *node, int level) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    switch (node->type) {
        case nPROGRAM:
            printf("PROGRAM\n");
            break;
        case nFUNCTIONMAIN:
            printf("FUNCTION MAIN -> %zu\n", node->childrenCount);
            break;
        case nFUNCTIONDEC:
            printf("FUNCTION DECL\n");
            break;
        case nFUNCTIONCAL:
            printf("FUNCTION CALL\n");
            break;
        case nLINE:
            printf("LINE -> %zu\n", node->childrenCount);
            break;
        case nSTATEMENT:
            printf("STATEMENT\n");
            break;
        case nKEYWORD:
            printf("KEYWORD\n");
            break;
        case nOPERAND:
            printf("OPERAND %s\n", node->node.operand.value);
            break;
        case nOPERATOR:
            printf("OPERATOR");
            switch (node->node.operator_.type) {
                case oPLUS:
                    printf(" +\n");
                    break;
                case oMINUS:
                    printf(" -\n");
                    break;
                case oMULT:
                    printf(" *\n");
                    break;
                case oDIV:
                    printf(" /\n");
                    break;
                default:
                    printf("\n");
            }
            break;
        default:
            printf("Unknown NodeType\n");
    }

    for (size_t i = 0; i < node->childrenCount; i++) {
        if (node->children != NULL) {
            printAST(node->children[i], level + 1);
        }
    }
}

int main(int argc, char *argv[]) {
    // ARGUMENTS
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

    // LEXING
    printf("\nLexing some tokens...\n");
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

    // PARSING
    printf("Growing an abstract syntax tree...\n");
    TreeNode pt;
    int parseStatus = parse(&pt, &tb);
    #if DEBUG
    printf("\n");
    printAST(&pt, 0);
    printf("\n");
    #endif
    if (parseStatus) {
        printf("Parser encountered an error!\n");
        return 1;
    }
    printf("Parsing was successful!\n");

    // CLEANUP
    freeTokenBox(&tb);
    fclose(source);
    return 0;
}