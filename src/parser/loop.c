#include <assert.h>
#include <string.h>
#include "loop.h"
#include "expression.h"
#include "parser.h"


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
