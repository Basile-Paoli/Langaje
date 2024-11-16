#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "expression.h"
#include "../ast/node_initializers.h"
#include "parser.h"


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
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }
    ++*currentToken;
    return node;
}

/**
 * Parses an expression
 * Starts with the lowest precedence operator (assignment)
 * Each following function parses a different operator with higher precedence
 * See grammar in README.md for more details
 */

astNode *parseExpression(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseLogicalOr(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }
    if (*currentToken >= tokenList->nb_tokens || tokenList->tokens[*currentToken].type != TOKEN_EQUAL) {
        return node;
    }

    if (node->type != VARIABLE && node->type != OPERATOR && node->value.operator != SUBSCRIPT) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Invalid left-hand side in assignment");
        freeAstNode(node);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    astNode *right = parseLogicalOr(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        freeAstNode(node);
        return NULL;
    }
    return newBinaryOperatorNode(TOKEN_EQUAL, node, right);

}

astNode *parseLogicalOr(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseLogicalAnd(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens &&
           tokenList->tokens[*currentToken].type == TOKEN_OR) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

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
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

        astNode *right = parseEquality(tokenList, currentToken, err);
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
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

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
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

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
            || tokenList->tokens[*currentToken].type == TOKEN_SUBSTRACTION)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

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
    astNode *node = parseExponentiation(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens && (
            tokenList->tokens[*currentToken].type == TOKEN_MULTIPLICATION ||
            tokenList->tokens[*currentToken].type == TOKEN_DIVISION ||
            tokenList->tokens[*currentToken].type == TOKEN_MODULO)) {

        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

        astNode *right = parseExponentiation(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }

        node = newBinaryOperatorNode(operator.type, node, right);
    }

    return node;
}


astNode *parseExponentiation(TokenList *tokenList, int *currentToken, error *err) {
    astNode *node = parseUnaryOperators(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    while (*currentToken < tokenList->nb_tokens && tokenList->tokens[*currentToken].type == TOKEN_POWER) {
        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            return endOfInputError(err);
        }

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

    if (tokenList->tokens[*currentToken].type == TOKEN_SUBSTRACTION ||
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

    switch (tokenList->tokens[*currentToken].type) {
        case TOKEN_INT: {
            return intTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_FLOAT: {
            return floatTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_IDENTIFIER: {
            return parseIdentifier(tokenList, currentToken, err);
        }
        case TOKEN_STRING: {
            return stringTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_LPAREN: {
            return parseParenthesisExpression(tokenList, currentToken, err);
        }
        case TOKEN_LBRACKET: {
            return parseArray(tokenList, currentToken, err);
        }
        default: {
            err->value = ERR_SYNTAX;
            err->message = malloc(strlen("Unexpected token ") + strlen(tokenList->tokens[*currentToken].value) + 1);
            sprintf(err->message, "Unexpected token %s", tokenList->tokens[*currentToken].value);
            return addPositionToError(err, tokenList->tokens[*currentToken]);
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
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;

    return node;
}


astNode *parseBracketExpression(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_LBRACKET);
    ++*currentToken;
    astNode *node = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_RBRACKET) {
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected closing bracket, found ") +
                strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected closing bracket, found %s",
                tokenList->tokens[*currentToken].value);
        freeAstNode(node);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;

    return node;
}


astNode *parseArray(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_LBRACKET);
    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }
    if (tokenList->tokens[*currentToken].type == TOKEN_RBRACKET) {
        return newArrayNode(0, NULL);
    }

    astNode *firstVal = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }
    astNode **values = newChildren(firstVal);
    int valueCount = 1;

    while (*currentToken < tokenList->nb_tokens && tokenList->tokens[*currentToken].type == TOKEN_COMMA) {
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            freeChildren(values, valueCount);
            return endOfInputError(err);
        }

        astNode *val = parseExpression(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeChildren(values, valueCount);
            return NULL;
        }

        values = appendChild(values, valueCount++, val);
    }

    if (*currentToken >= tokenList->nb_tokens) {
        freeChildren(values, valueCount);
        return endOfInputError(err);
    }
    if (tokenList->tokens[*currentToken].type != TOKEN_RBRACKET) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected ']' at the end of array expression");
        freeChildren(values, valueCount);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;

    return newArrayNode(valueCount, values);
}


astNode *parseIdentifier(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_IDENTIFIER);
    astNode *node;
    if (*currentToken + 1 < tokenList->nb_tokens && tokenList->tokens[*currentToken + 1].type == TOKEN_LPAREN) {
        node = parseFunctionCall(tokenList, currentToken, err);
    } else {
        node = identifierTokenToNode(tokenList->tokens[*currentToken]);
        ++*currentToken;
    }


    while (*currentToken < tokenList->nb_tokens && tokenList->tokens[*currentToken].type == TOKEN_LBRACKET) {
        astNode *index = parseBracketExpression(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(node);
            return NULL;
        }
        node = newSubscriptNode(node, index);
    }

    return node;
}


astNode *parseFunctionCall(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_IDENTIFIER);
    assert(tokenList->tokens[*currentToken + 1].type == TOKEN_LPAREN);
    char *name = tokenList->tokens[*currentToken].value;
    *currentToken += 2;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type == TOKEN_RPAREN) {
        ++*currentToken;
        return newFunctionCallNode(name, NULL, 0);
    }

    int nbArgs = 0;
    astNode **args = NULL;
    args = parseExpressionsSeparatedByCommas(tokenList, currentToken, &nbArgs, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        freeChildren(args, nbArgs);
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_RPAREN) {
        freeChildren(args, nbArgs);
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected closing parenthesis, found ") +
                strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected closing parenthesis, found %s",
                tokenList->tokens[*currentToken].value);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    return newFunctionCallNode(name, args, nbArgs);

}

astNode **parseExpressionsSeparatedByCommas(TokenList *tokenList, int *currentToken, int *nbExpressions, error *err) {
    astNode **expressions = NULL;
    *nbExpressions = 0;


    astNode *first = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    expressions = newChildren(first);
    ++*nbExpressions;

    while (*currentToken < tokenList->nb_tokens && tokenList->tokens[*currentToken].type == TOKEN_COMMA) {
        ++*currentToken;
        if (*currentToken >= tokenList->nb_tokens) {
            freeChildren(expressions, *nbExpressions);
            return endOfInputError(err);
        }

        astNode *expr = parseExpression(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeChildren(expressions, *nbExpressions);
            return NULL;
        }

        expressions = appendChild(expressions, *nbExpressions, expr);
        ++*nbExpressions;
    }

    return expressions;
}