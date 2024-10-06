//
// Created by user on 10/6/24.
//


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assignment.h"
#include "expression.h"
#include "../ast/node_initializers.h"

astNode *parseAssignmentInstruction(TokenList *tokenList, error *err) {
    assert(tokenList->tokens[0].type == TOKEN_IDENTIFIER);
    assert(tokenList->tokens[1].type == TOKEN_EQUAL);

    int currentToken = 2;

    astNode *expression = parseExpression(tokenList, &currentToken, err);

    if (err->value != ERR_SUCCESS) {
        return NULL;
    }
    if (currentToken >= tokenList->nb_tokens) {
        freeAstNode(expression);
        return endOfInstructionError(err);
    }

    if (tokenList->tokens[currentToken].type != TOKEN_SEMICOLON) {
        freeAstNode(expression);
        err->value = ERR_SYNTAX;
        err->message = malloc(strlen("Expected ';' , found") + strlen(tokenList->tokens[currentToken].value) + 1);
        sprintf(err->message, "Expected ';' , found %s", tokenList->tokens[currentToken].value);
        return NULL;
    }

    astNode *variable = newVariableNode(tokenList->tokens[0].value);

    return newBinaryOperatorNode(TOKEN_EQUAL, variable, expression);
}