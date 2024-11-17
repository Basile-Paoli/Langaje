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
        assignErrorMessage(err, "Expected '(' after 'while'");
        return addPositionToError(err, tokenList->tokens[*currentToken]);
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
        assignErrorMessage(err, "Expected '(' after 'for'");
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        err->value = ERR_SYNTAX;
        assignErrorMessage(err, "Expected identifier after 'for ('");
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    char *identifier = strdup(tokenList->tokens[*currentToken].value);

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IN) {
        err->value = ERR_SYNTAX;
        assignErrorMessage(err, "Expected 'in' after identifier in 'for ('");
        return addPositionToError(err, tokenList->tokens[*currentToken]);
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
        assignErrorMessage(err, "Expected 1 to 3 expressions separated by commas after 'in' in 'for ('");
        free(identifier);
        freeChildren(children, childrenCount);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
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
        assignErrorMessage(err, "Expected ')' after 'for (identifier in expressions'");
        free(identifier);
        freeChildren(children, childrenCount);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
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


astNode *parseBreakInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(*currentToken < tokenList->nb_tokens);
    assert(tokenList->tokens[*currentToken].type == TOKEN_BREAK);

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_SEMICOLON) {
        err->value = ERR_SYNTAX;
        assignErrorMessage(err, "Expected ';'");
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    return newBreakNode();
}

astNode *parseContinueInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(*currentToken < tokenList->nb_tokens);
    assert(tokenList->tokens[*currentToken].type == TOKEN_CONTINUE);

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_SEMICOLON) {
        err->value = ERR_SYNTAX;
        assignErrorMessage(err, "Expected ';'");
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    return newContinueNode();
}

