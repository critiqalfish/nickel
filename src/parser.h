#pragma once
#include "common.h"
#include <stddef.h>

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
            struct TreeNode *body;
        } functionDec;
        struct {
            const char *name;
            struct TreeNode *params;
        } functionCal;
        struct {
            StatementType type;
            struct TreeNode *children;
        } statement;
    } node;
    struct TreeNode **children;
    size_t childrenCount;
} TreeNode;

int parse(TreeNode *pt, TokenBox *tb);
TreeNode *createNode(NodeType type);
void addChildNode(TreeNode *parent, TreeNode *child);
int getNested(TokenBox *tb, long location);
void parseFunctionBody(TokenBox *tb, long location);