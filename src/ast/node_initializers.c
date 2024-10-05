//
// Created by user on 10/5/24.
//
#include <stdlib.h>
#include <assert.h>
#include "node_initializers.h"

operator tokenToOperator(TokenType type) {
    switch (type) {
        case TOKEN_ADDITION:
            return ADDITION;
        case TOKEN_SUBTRACTION:
            return SUBTRACTION;
        case TOKEN_MULTIPLICATION:
            return MULTIPLICATION;
        case TOKEN_DIVISION:
            return DIVISION;
        case TOKEN_EQUAL:
            return ASSIGNMENT;
        default:
            assert(0);
    }
}


astNode *newBinaryOperatorNode(TokenType token, astNode *left, astNode *right) {
    astNode *node = newOperatorNode(tokenToOperator(token));
    node->type = OPERATOR;
    node->children = malloc(2 * sizeof(astNode *));
    node->children[0] = left;
    node->children[1] = right;
    return node;
}

astNode *numberTokenToNode(Token token) {
    return NULL;
}

astNode *identifierTokenToNode(Token token) {
    return NULL;
}

astNode *stringTokenToNode(Token token) {
    return NULL;
}
