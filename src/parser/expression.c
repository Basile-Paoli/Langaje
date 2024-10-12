//
// Created by user on 10/5/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "expression.h"
#include "../ast/node_initializers.h"


astNode *parseExpressionInstruction(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        freeAstNode(node);
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_SEMICOLON) {
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected semicolon, found ") + strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected semicolon, found %s", tokenList->tokens[*currentToken].value);
        freeAstNode(node);
        return NULL;
    }
    ++*currentToken;
    return node;
}

astNode *parseExpression(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseTerm(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           (tokenList->tokens[*currentToken].type == TOKEN_ADDITION ||
            tokenList->tokens[*currentToken].type == TOKEN_SUBTRACTION)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseTerm(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}


astNode *parseTerm(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseFactor(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens && (
            tokenList->tokens[*currentToken].type == TOKEN_MULTIPLICATION ||
            tokenList->tokens[*currentToken].type == TOKEN_DIVISION)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;

        astNode *right = parseFactor(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}


astNode *parseFactor(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseExponent(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }
    // Nothing to do while exponentiation is not implemented
    return node;
}

astNode *parseParenthesisExpression(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_LPAREN);
    ++*currentToken;
    astNode *node = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_RPAREN) {
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected closing parenthesis, found ") +
                strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected closing parenthesis, found %s",
                tokenList->tokens[*currentToken].value);
        freeAstNode(node);
        return NULL;
    }

    ++*currentToken;

    return node;
}

astNode *parseExponent(TokenList *tokenList, int *currentToken, error *err) {
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }
    switch (tokenList->tokens[*currentToken].type) {
        case TOKEN_NUMBER: {
            return intTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_IDENTIFIER: {
            return identifierTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_STRING: {
            return stringTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_LPAREN: {
            return parseParenthesisExpression(tokenList, currentToken, err);
        }
        default: {
            err->value = ERR_SYNTAX;
            err->message = malloc(strlen("Unexpected token ") + strlen(tokenList->tokens[*currentToken].value) + 1);
            sprintf(err->message, "Unexpected token %s", tokenList->tokens[*currentToken].value);
            return NULL;
        }
    }
}
