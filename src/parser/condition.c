//
// Created by user on 10/13/24.
//

#include <assert.h>
#include <string.h>
#include "condition.h"
#include "expression.h"
#include "parser.h"

astNode *parseIfInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(*currentToken < tokenList->nb_tokens);
    assert(tokenList->tokens[*currentToken].type == TOKEN_KEYWORD_IF);

    ++*currentToken;
    if (tokenList->tokens[*currentToken].type != TOKEN_LPAREN) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Expected '(' after 'if'");
        return NULL;
    }

    astNode *condition = parseParenthesisExpression(tokenList, currentToken, err);

    if (err->value != ERR_SUCCESS) {
        return NULL;
    }
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    astNode *thenNode;

    if (tokenList->tokens[*currentToken].type != TOKEN_LBRACE) {
        thenNode = parseInstruction(tokenList, currentToken, err);
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
        thenNode = newInstructionBlockNode(block);
    }

    astNode *elseNode = NULL;

    if (*currentToken == tokenList->nb_tokens || tokenList->tokens[*currentToken].type != TOKEN_KEYWORD_ELSE) {
        return newConditionNode(condition, thenNode, elseNode);
    }

    ++*currentToken;

    if (tokenList->tokens[*currentToken].type != TOKEN_LBRACE) {
        elseNode = parseInstruction(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(condition);
            freeAstNode(thenNode);
            return NULL;
        }
    } else {
        InstructionBlock *block = parseInstructionBlockWithBraces(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeAstNode(condition);
            freeAstNode(thenNode);
            return NULL;
        }
        elseNode = newInstructionBlockNode(block);
    }

    return newConditionNode(condition, thenNode, elseNode);
}
