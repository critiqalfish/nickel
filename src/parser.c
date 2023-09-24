#include "parser.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse(TreeNode *pt, TokenBox *tb) {
    pt->type = nPROGRAM;
    long location = 0;

    while (location < tb->used) {
        // FUNCTION RETURN TYPE
        if (tb->tokens[location].type == tINT) {
            location++;
            // FUNCTION DECLARATION
            if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tFUNCTIONARROW) {
                location++;
                // MAIN
                if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tKEYWORD && strcmp(tb->tokens[location].value, "main") == 0 && tb->tokens[location - 2].type == tINT) {
                    TreeNode *mainFunc = createNode(nFUNCTIONMAIN);
                }
                // OTHER DECLARATION
                else if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tIDENTIFIER) {
                    TreeNode *otherFunc = createNode(nFUNCTIONDEC);
                }
                location++;
                // FUNCTION PARAMETERS
                if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tLPAREN) {
                    int nestEnd = getNested(tb, location);
                    if (nestEnd != -1) {
                        location = nestEnd;
                    } else return 1;
                    location++;
                }
                // FUNCTION BODY
                if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tLBRACE) {
                    int nestEnd = getNested(tb, location);
                    if (nestEnd != -1) {
                        parseFunctionBody(tb, location);
                        location = nestEnd;
                    } else return 1;
                    location++;
                }
            }
        }
        // NOTHING
        else location++;
    }

    return 0;
}

TreeNode *createNode(NodeType type) {
    TreeNode *node = (TreeNode*) malloc(sizeof(TreeNode));
    node->type = type;
    node->children = NULL;
    return node;
}

void addChildNode(TreeNode *parent, TreeNode *child) {
    if (parent->children == NULL) {
        parent->children = (TreeNode**) malloc(sizeof(TreeNode*));
        if (parent->children == NULL) {
            printf("malloc failed");
            return;
        }
        parent->childrenCount = 0;
    }
    else {
        parent->children = (TreeNode**) realloc(parent->children, (parent->childrenCount + 1) * sizeof(TreeNode*));
        if (parent->children == NULL) {
            printf("realloc failed");
            return;
        }
    }
    parent->childrenCount++;
    parent->children[parent->childrenCount - 1] = child;
}

int getNested(TokenBox *tb, long location) {
    int nestingLevel = 0;
    long startNest = location;
    long endNest = -1;
    
    TokenType left = tb->tokens[location].type;
    TokenType right;
    if (left == tLPAREN) right = tRPAREN;
    else if (left == tLBRACE) right = tRBRACE;

    while (location < tb->used) {
        if (tb->tokens[location].type == left) {
            nestingLevel++;
        }
        else if (tb->tokens[location].type == right) {
            nestingLevel--;
            if (nestingLevel == 0) {
                endNest = location;
                return endNest;
            }
        }
        location++;
    }
    return -1;
}

void parseFunctionBody(TokenBox *tb, long location) {

}