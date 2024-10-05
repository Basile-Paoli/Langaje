//
// Created by user on 10/5/24.
//

#include "expression.h"
#include "../ast/node_initializers.h"

astNode *parseExpression(TokenList *tokenList, int *currentToken) {
    astNode *node = parseTerm(tokenList, currentToken);
    while (tokenList->tokens[*currentToken].type == TOKEN_ADDITION ||
           tokenList->tokens[*currentToken].type == TOKEN_SUBTRACTION) {
        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseTerm(tokenList, currentToken);
        node = newBinaryOperatorNode(operator.type, node, right);
    }
}


astNode *parseTerm(TokenList *tokenList, int *currentToken) {
    astNode *node = parseFactor(tokenList, currentToken);
    while (tokenList->tokens[*currentToken].type == TOKEN_MULTIPLICATION ||
           tokenList->tokens[*currentToken].type == TOKEN_DIVISION) {
        Token operator = tokenList->tokens[*currentToken];
        ++*currentToken;
        astNode *right = parseFactor(tokenList, currentToken);
        node = newBinaryOperatorNode(operator.type, node, right);
    }
    return node;
}


astNode *parseFactor(TokenList *tokenList, int *currentToken) {
    astNode *node = parseExponent(tokenList, currentToken);
    // Nothing to do while exponentiation is not implemented
    return node;
}

astNode *parseExponent(TokenList *tokenList, int *currentToken) {
    switch (tokenList->tokens[*currentToken].type) {
        case TOKEN_NUMBER: {
            return numberTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_IDENTIFIER: {
            return identifierTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_STRING: {
            return stringTokenToNode(tokenList->tokens[(*currentToken)++]);
        }
        case TOKEN_LPAREN: {
            ++*currentToken;
            astNode *node = parseExpression(tokenList, currentToken);
            if (tokenList->tokens[*currentToken].type != TOKEN_RPAREN) {
                // Handle error
            }
            ++*currentToken;
            return node;
        }
        default: {
            // Handle error
        }
    }
}
