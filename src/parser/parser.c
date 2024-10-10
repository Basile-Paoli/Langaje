//
// Created by Basile Paoli on 10/5/24.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "var_declaration.h"
#include "expression.h"
#include "assignment.h"

int isAssignment(TokenList *tokenList, error *err) {
    if (tokenList->nb_tokens < 3) {
        return 0;
    }
    Token first = tokenList->tokens[0];
    Token second = tokenList->tokens[1];
    if (first.type == TOKEN_IDENTIFIER && second.type == TOKEN_EQUAL) {
        return 1;
    } else {
        return 0;
    }
}

ParseResult *appendInstruction(ParseResult *parseResult, astNode *instruction) {
    if (parseResult->instructions == NULL) {
        parseResult->instructions = malloc(sizeof(astNode *));
        parseResult->instructions[0] = instruction;
        parseResult->instructionsCount = 1;
    } else {
        parseResult->instructions = realloc(parseResult->instructions,
                                            sizeof(astNode *) * (parseResult->instructionsCount + 1));
        parseResult->instructions[parseResult->instructionsCount] = instruction;
        parseResult->instructionsCount++;
    }
    return parseResult;
}

void freeParseResult(ParseResult *parseResult) {
    for (int i = 0; i < parseResult->instructionsCount; i++) {
        freeAstNode(parseResult->instructions[i]);
    }
    free(parseResult->instructions);
    free(parseResult);
}


ParseResult *parse(TokenList *tokenList, error *err) {
    ParseResult *parseResult = malloc(sizeof(ParseResult));
    parseResult->instructions = NULL;
    parseResult->instructionsCount = 0;
    int currentToken = 0;
    while (currentToken < tokenList->nb_tokens) {
        astNode *instruction = parseInstruction(tokenList, &currentToken, err);
        if (instruction == NULL) {
            freeParseResult(parseResult);
            return NULL;
        }
        parseResult = appendInstruction(parseResult, instruction);
    }
    return parseResult;
}

astNode *parseInstruction(TokenList *tokenList, int *currentToken, error *err) {

    assert(*currentToken < tokenList->nb_tokens);

    Token first = tokenList->tokens[*currentToken];
    if (first.type == TOKEN_KEYWORD && strcmp(first.value, "def") == 0) {
        return parseVarDeclarationInstruction(tokenList, currentToken, err);
    }
    if (isAssignment(tokenList, err)) {
        return parseAssignmentInstruction(tokenList, currentToken, err);
    }
    return parseExpressionInstruction(tokenList, currentToken, err);
}


