#pragma once
#include "common.h"

typedef enum {
    sBACK
} StatementType;

typedef enum {
    nPROGRAM,
    nFUNCTIONMAIN,
    nFUNCTIONDEC,
    nFUNCTIONCAL,
    nSTATEMENT
} NodeType;

typedef struct TreeNode {
    NodeType type;
    union {
        struct {
            struct TreeNode *body;
        } functionMain;
        struct {
            const char *name;
            struct TreeNode *params;
        } functionCal;
        struct {
            StatementType type;
            struct TreeNode *children;
        } statement;
    } node;
} TreeNode;

int parse(TokenBox *tb);