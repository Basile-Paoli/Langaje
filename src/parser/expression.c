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

/**
 * Parses an expression
 * Starts with the lowest precedence operator (logical or)
 * Each following function parses a different operator with higher precedence
 * See grammar in README.md for more details
 */

astNode *parseExpression(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseLogicalAnd(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           tokenList->tokens[*currentToken].type == TOKEN_OR) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseLogicalAnd(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}

astNode *parseLogicalAnd(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseEquality(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           tokenList->tokens[*currentToken].type == TOKEN_AND) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseMultiplication(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}

astNode *parseEquality(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseComparison(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           (tokenList->tokens[*currentToken].type == TOKEN_EQUAL_EQUAL)
           || tokenList->tokens[*currentToken].type == TOKEN_NOT_EQUAL) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseMultiplication(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}

astNode *parseComparison(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseAddition(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           (tokenList->tokens[*currentToken].type == TOKEN_GREATER
            || tokenList->tokens[*currentToken].type == TOKEN_GREATER_EQUAL
            || tokenList->tokens[*currentToken].type == TOKEN_LESS
            || tokenList->tokens[*currentToken].type == TOKEN_LESS_EQUAL)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseMultiplication(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}

astNode *parseAddition(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseMultiplication(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           (tokenList->tokens[*currentToken].type == TOKEN_ADDITION
            || tokenList->tokens[*currentToken].type == TOKEN_SUBTRACTION)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseMultiplication(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}


astNode *parseMultiplication(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseUnaryOperators(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens && (
            tokenList->tokens[*currentToken].type == TOKEN_MULTIPLICATION ||
            tokenList->tokens[*currentToken].type == TOKEN_DIVISION)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;

        astNode *right = parseUnaryOperators(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}


astNode *parseUnaryOperators(TokenList *tokenList, int *currentToken, error *err) {

    if (tokenList->tokens[*currentToken].type == TOKEN_SUBTRACTION ||
        tokenList->tokens[*currentToken].type == TOKEN_NOT) {
        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }
        astNode *right = parseUnaryOperators(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            return NULL;
        }
        return newUnaryOperatorNode(operator.type, right);
    }

    astNode *node = parsePrimary(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    return node;
}


/**
 * Parses a primary expression
 * A primary expression can be any hardcoded value(int, float, bool, string, array etc)
   an identifier (variable, function call) or an expression in parenthesis
 * See grammar in README.md for more details
 */
astNode *parsePrimary(TokenList *tokenList, int *currentToken, error *err) {
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

astNode *parseParenthesisExpression(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_LPAREN);
    ++*currentToken;
    astNode *node = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
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
