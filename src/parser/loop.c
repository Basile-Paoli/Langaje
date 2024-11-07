#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "loop.h"
#include "expression.h"
#include "parser.h"
#include "../ast/node_initializers.h"


astNode *parseWhileInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(*currentToken < tokenList->nb_tokens);
    assert(tokenList->tokens[*currentToken].type == TOKEN_WHILE);

    ++*currentToken;
    if (tokenList->tokens[*currentToken].type != TOKEN_LPAREN) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected '(' after 'while'");
        return NULL;
    }

    astNode *condition = parseParenthesisExpression(tokenList, currentToken, err);

    if (err->value != ERR_SUCCESS) {
        return NULL;
    }
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    astNode *instruction = NULL;

    if (tokenList->tokens[*currentToken].type != TOKEN_LBRACE) {
        instruction = parseInstruction(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(condition);
            return NULL;
        }
    } else {
        InstructionBlock *block = parseInstructionBlockWithBraces(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(condition);
            return NULL;
        }
        instruction = newInstructionBlockNode(block);
    }

    return newWhileNode(condition, instruction);
}

astNode *parseForInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(*currentToken < tokenList->nb_tokens);
    assert(tokenList->tokens[*currentToken].type == TOKEN_FOR);

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }
    if (tokenList->tokens[*currentToken].type != TOKEN_LPAREN) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected '(' after 'for'");
        return NULL;
    }

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    //for (i in 5) or for (i in 0,5) or for (i in 0,5,1)

    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected identifier after 'for ('");
        return NULL;
    }

    char *identifier = strdup(tokenList->tokens[*currentToken].value);

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IN) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected 'in' after identifier in 'for ('");
        return NULL;
    }

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    int childrenCount;
    astNode **children = parseExpressionsSeparatedByCommas(tokenList, currentToken, &childrenCount, err);
    if (err->value != ERR_SUCCESS) {
        free(identifier);
        return NULL;
    }

    if (childrenCount < 1 || childrenCount > 3) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected 1 to 3 expressions separated by commas after 'in' in 'for ('");
        free(identifier);
        freeChildren(children, childrenCount);
        return NULL;
    }

    if (err->value != ERR_SUCCESS) {
        free(identifier);
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_RPAREN) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected ')'");
        free(identifier);
        freeChildren(children, childrenCount);
        return NULL;
    }
    ++*currentToken;

    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    astNode *instruction = NULL;

    if (tokenList->tokens[*currentToken].type != TOKEN_LBRACE) {
        instruction = parseInstruction(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            free(identifier);
            freeChildren(children, childrenCount);
            return NULL;
        }
    } else {
        InstructionBlock *block = parseInstructionBlockWithBraces(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            free(identifier);
            freeChildren(children, childrenCount);
            return NULL;
        }
        instruction = newInstructionBlockNode(block);
    }

    children = appendChild(children, childrenCount, instruction);
    return newForNode(identifier, children, childrenCount + 1);
}

