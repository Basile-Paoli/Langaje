//
// Created by Basile Paoli on 10/5/24.
//

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "var_declaration.h"
#include "expression.h"
#include "condition.h"


InstructionBlock *parse(TokenList *tokenList, error *err) {
    int currentToken = 0;
    return parseInstructions(tokenList, &currentToken, err);
}

InstructionBlock *parseInstructions(TokenList *tokenList, int *currentToken, error *err) {
    InstructionBlock *block = newInstructionBlock(10);
    while (*currentToken < tokenList->nb_tokens) {
        astNode *instruction = parseInstruction(tokenList, currentToken, err);
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
        err->message = strdup("Expected '}'");
        freeInstructionBlock(block);
        return NULL;
    }
    (*currentToken)++;

    return block;
}

astNode *parseInstruction(TokenList *tokenList, int *currentToken, error *err) {

    assert(*currentToken < tokenList->nb_tokens);

    Token first = tokenList->tokens[*currentToken];

    if (first.type == TOKEN_DEF) {
        return parseVarDeclarationInstruction(tokenList, currentToken, err);
    }
    if (first.type == TOKEN_IF) {
        return parseIfInstruction(tokenList, currentToken, err);
    }

    return parseExpressionInstruction(tokenList, currentToken, err);
}


