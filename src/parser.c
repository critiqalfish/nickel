#include "parser.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct opPrecedence opPrecedence = {
    {1, 2},  // PLUS
    {1, 2},  // MINUS
    {3, 4},  // MULT
    {3, 4}   // DIV
};

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
                TreeNode *func;
                // MAIN
                if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tKEYWORD && strcmp(tb->tokens[location].value, "main") == 0 && tb->tokens[location - 2].type == tINT) {
                    func = createNode(nFUNCTIONMAIN);
                }
                // OTHER DECLARATION
                else if (checkBounds(tb->used, location, 0) && tb->tokens[location].type == tIDENTIFIER) {
                    func = createNode(nFUNCTIONDEC);
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
                        parseFunctionBody(func, tb, location, nestEnd);
                        addChildNode(pt, func);
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

long getNested(TokenBox *tb, long location) {
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

void parseFunctionBody(TreeNode *func, TokenBox *tb, long location, long nestEnd) {
    long loc = location + 1;

    while (loc < nestEnd) {
        long lineSize = 0;
        do {
            lineSize++;
            loc++;
        } while (loc < nestEnd && tb->tokens[loc].type != tSEMICOL);
        TreeNode *line = createNode(nLINE);
        parseLine(line, tb, loc - lineSize, loc + lineSize);
        addChildNode(func, line);
        loc++;
    }
}

void parseLine(TreeNode *line, TokenBox *tb, long lineStart, long lineEnd) {
    long loc = lineStart;

    while (loc < lineEnd) {
        // FUNCTION CALL
        if (loc == lineStart && tb->tokens[loc].type == tIDENTIFIER) {
            loc++;
        }
        // KEYWORD
        else if (loc == lineStart && tb->tokens[loc].type == tKEYWORD) {
            TreeNode *keyword = createNode(nKEYWORD);
            // BACK
            if (strcmp(tb->tokens[loc].value, "back") == 0) { // MAYBE SEGFAULT 🗿🗿🗿 (gigachad)
                keyword->node.keyword.type = kBACK;
                loc++;
                if (checkBounds(lineEnd, loc, 0) && tb->tokens[loc].type == tLPAREN) {
                    long expEnd = getNested(tb, loc);
                    loc++;
                    addChildNode(keyword, parseExpression(tb, loc, expEnd - 1, 0));
                    addChildNode(line, keyword);
                }
            }
        }
        // VARIABLE DECLARATION
        else if (loc == lineStart && tb->tokens[loc].type == tINT) {
            loc++;
        }
        // VARIABLE ASSIGNMENT
        else if (loc == lineStart && tb->tokens[loc].type == tINT) {
            loc++;
        }
        else {
            loc++;
        }
    }
}

TreeNode *parseExpression(TokenBox *tb, long expStart, long expEnd, int precedence) {
    long loc = expStart;
    TreeNode *left = createNode(nOPERAND);
    left->node.operand.value = tb->tokens[expStart].value;
    
    while (loc < expEnd) {
        if (!checkBounds(expEnd, loc, 1)) {
            break;
        }
        loc += 2;

        OperatorType op;
        int l, r;
        switch (tb->tokens[loc - 1].type) {
            case tPLUS:
                op = oPLUS;
                l = opPrecedence.plus.left;
                r = opPrecedence.plus.right;
                break;
            case tMINUS:
                op = oMINUS;
                l = opPrecedence.minus.left;
                r = opPrecedence.minus.right;
                break;
            case tASTER:
                op = oMULT;
                l = opPrecedence.mult.left;
                r = opPrecedence.mult.right;
                break;
            case tDIV:
                op = oDIV;
                l = opPrecedence.div.left;
                r = opPrecedence.div.right;
                break;
            default:
                break;
        }

        printf("loc: %ld l: %d r: %d precedence: %d operator: %d\n",loc, l, r, precedence, op);
        if (l < precedence || ((loc == expEnd || loc + 2 == expEnd) && precedence == 0)) {
            printf("true\n");
            break;
        }

        TreeNode *right = parseExpression(tb, loc, expEnd, r); // MAYBE SEGFAULT 🗿🗿🗿 (gigachad) (because the + 2)
        TreeNode *opNode = createNode(nOPERATOR);
        opNode->node.operator_.type = op;
        addChildNode(opNode, left);
        addChildNode(opNode, right);
        left = opNode;
    }
    return left;
}