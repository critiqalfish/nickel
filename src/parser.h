#pragma once
#include "common.h"
#include <stddef.h>

typedef enum {
    kBACK
} KeywordType;

typedef enum {
    oPLUS,
    oMINUS,
    oMULT,
    oDIV
} OperatorType;

typedef enum {
    nPROGRAM,
    nFUNCTIONMAIN,
    nFUNCTIONDEC,
    nFUNCTIONCAL,
    nLINE,
    nSTATEMENT,
    nKEYWORD,
    nOPERAND,
    nOPERATOR
} NodeType;

struct opPrecedence {
    struct {
        int left;
        int right;
    } plus, minus, mult, div;
};

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
            KeywordType type;
        } keyword;
        struct {
            const char *value;
        } operand;
        struct {
            OperatorType type;
        } operator_;
    } node;
    struct TreeNode **children;
    size_t childrenCount;
} TreeNode;

int parse(TreeNode *pt, TokenBox *tb);
TreeNode *createNode(NodeType type);
void addChildNode(TreeNode *parent, TreeNode *child);
long getNested(TokenBox *tb, long location);
void parseFunctionBody(TreeNode *func, TokenBox *tb, long location, long nestEnd);
void parseLine(TreeNode *line, TokenBox *tb, long lineStart, long lineEnd);
TreeNode *parseExpression(TokenBox *tb, long expStart, long expEnd, int precedence);