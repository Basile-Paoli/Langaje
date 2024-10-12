//
// Created by Basile Paoli on 10/5/24.
//

#include <string.h>
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

ParseResult *newParseResult(int capacity) {
    assert(capacity > 0);
    ParseResult *parseResult = malloc(sizeof(ParseResult));
    parseResult->instructions = malloc(sizeof(astNode *) * capacity);
    parseResult->instructionsCount = 0;
    parseResult->capacity = capacity;
    return parseResult;
}

ParseResult *appendInstruction(ParseResult *parseResult, astNode *instruction) {
    assert(parseResult != NULL);
    if (parseResult->instructionsCount == parseResult->capacity) {
        parseResult->capacity *= 2;
        parseResult->instructions = realloc(parseResult->instructions, sizeof(astNode *) * parseResult->capacity);
    }
    parseResult->instructions[parseResult->instructionsCount] = instruction;
    parseResult->instructionsCount++;
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
    ParseResult *parseResult = newParseResult(10);
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


