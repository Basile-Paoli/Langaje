//
// Created by user on 10/5/24.
//
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
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
        case TOKEN_EQUAL_EQUAL:
            return EQUAL;
        case TOKEN_NOT_EQUAL:
            return NOT_EQUAL;
        case TOKEN_GREATER:
            return GREATER;
        case TOKEN_LESS:
            return LESS;
        case TOKEN_GREATER_EQUAL:
            return GREATER_EQUAL;
        case TOKEN_LESS_EQUAL:
            return LESS_EQUAL;
        case TOKEN_AND:
            return AND;
        case TOKEN_OR:
            return OR;
        case TOKEN_NOT:
            return NOT;
        default:
            printf("Unknown token type: %d\n", type);
            assert(0);
    }
}


astNode *newBinaryOperatorNode(TokenType token, astNode *left, astNode *right) {
    astNode *node = newOperatorNode(tokenToOperator(token));
    node->type = OPERATOR;
    node->children = malloc(2 * sizeof(astNode *));
    node->children[0] = left;
    node->children[1] = right;
    node->childrenCount = 2;
    return node;
}

astNode *intTokenToNode(Token token) {
    var value = {.value._int = atoi(token.value), .type = _int};
    return newValueNode(value);
}

astNode *identifierTokenToNode(Token token) {
    return newVariableNode(token.value);
}

astNode *stringTokenToNode(Token token) {
    // Remove the quotes
    token.value[strlen(token.value) - 1] = '\0';
    return newValueNode((var) {
            .value._string = strndup(token.value + 1, strlen(token.value) - 2),
            .type = _string
    });
}
