#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "parser.h"
#include "var_declaration.h"
#include "expression.h"
#include "condition.h"
#include "loop.h"
#include "function_declaration.h"

void *addPositionToError(error *err, Token token) {
    char *newMessage = malloc(strlen(err->message) + 100);
    sprintf(newMessage, "%s (line %d, column %d)", err->message, token.line, token.column);
    assignErrorMessage(err, newMessage);
    free(newMessage);
    return NULL;
}

InstructionBlock *parse(TokenList *tokenList, error *err) {
    int currentToken = 0;
    return parseInstructions(tokenList, &currentToken, err);
}

InstructionBlock *parseInstructions(TokenList *tokenList, int *currentToken, error *err) {
    InstructionBlock *block = newInstructionBlock(10);
    while (*currentToken < tokenList->nb_tokens) {
        astNode *instruction = parseFunctionOrInstruction(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeInstructionBlock(block);
            return NULL;
        }
        block = appendInstruction(block, instruction);
    }
    return block;
}

InstructionBlock *parseInstructionBlockWithBraces(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_LBRACE);
    (*currentToken)++;

    InstructionBlock *block = newInstructionBlock(10);
    while (*currentToken < tokenList->nb_tokens && tokenList->tokens[*currentToken].type != TOKEN_RBRACE) {
        astNode *instruction = parseInstruction(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            freeInstructionBlock(block);
            return NULL;
        }
        block = appendInstruction(block, instruction);
    }
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        freeInstructionBlock(block);
        return endOfInputError(err);
    }
    if (tokenList->tokens[*currentToken].type != TOKEN_RBRACE) {
        err->value = ERR_SYNTAX;
        assignErrorMessage(err, "Expected '}'");
        freeInstructionBlock(block);
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }
    (*currentToken)++;

    return block;
}


astNode *parseFunctionOrInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(*currentToken < tokenList->nb_tokens);

    Token first = tokenList->tokens[*currentToken];

    if (first.type == TOKEN_FUNCTION_DECLARATION) {
        return parseFunctionDeclaration(tokenList, currentToken, err);
    } else {
        return parseInstruction(tokenList, currentToken, err);
    }
}

astNode *parseInstruction(TokenList *tokenList, int *currentToken, error *err) {

    assert(*currentToken < tokenList->nb_tokens);

    Token first = tokenList->tokens[*currentToken];

    switch (first.type) {
        case TOKEN_DEF:
            return parseVarDeclarationInstruction(tokenList, currentToken, err);
        case TOKEN_IF:
            return parseIfInstruction(tokenList, currentToken, err);
        case TOKEN_WHILE:
            return parseWhileInstruction(tokenList, currentToken, err);
        case TOKEN_FOR:
            return parseForInstruction(tokenList, currentToken, err);
        case TOKEN_BREAK:
            return parseBreakInstruction(tokenList, currentToken, err);
        case TOKEN_CONTINUE:
            return parseContinueInstruction(tokenList, currentToken, err);
        case TOKEN_FUNCTION_RETURN:
            return parseReturnInstruction(tokenList, currentToken, err);
        case TOKEN_BREAKPOINT:
            ++*currentToken;
            return newBreakpointNode();
        case TOKEN_MEMORY_DUMP:
            ++*currentToken;
            return newMemoryDumpNode();

        default:
            return parseExpressionInstruction(tokenList, currentToken, err);
    }
}


