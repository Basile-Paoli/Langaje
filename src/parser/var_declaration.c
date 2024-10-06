//
// Created by user on 10/5/24.
//

#include "var_declaration.h"
#include "expression.h"
#include "../ast/node_initializers.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

//To be implemented
varType typeFromString(const char *string) {
    return FLOAT;
}


astNode *parseVarDeclarationInstruction(TokenList *tokenList, error *err) {
    // Assert first token is def
    assert(strcmp(tokenList->tokens[0].value, "def") == 0);


    int currentToken = 1;
    astNode *initNode = parseVarDeclaration(tokenList, &currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (currentToken >= tokenList->nb_tokens) {
        freeAstNode(initNode);
        return endOfInstructionError(err);
    }

    if (tokenList->tokens[currentToken].type == TOKEN_SEMICOLON) {
        if (initNode->value.initialization.typed == 0) {
            err->value = ERR_SYNTAX;
            sprintf(err->message, "Cannot declare a variable without a type or an initialization");
            freeAstNode(initNode);
            return NULL;
        }
        return initNode;
    }

    if (tokenList->tokens[currentToken].type != TOKEN_EQUAL) {
        err->value = ERR_SYNTAX;
        sprintf(err->message, "Expected '=' or ';' , got %s",
                tokenList->tokens[currentToken].value);
        freeAstNode(initNode);
        return NULL;
    }

    ++currentToken;
    astNode *expression = parseExpression(tokenList, &currentToken, err);
    if (err->value != ERR_SUCCESS) {
        freeAstNode(initNode);
        return NULL;
    }

    if (currentToken >= tokenList->nb_tokens) {
        freeAstNode(expression);
        freeAstNode(initNode);
        return endOfInstructionError(err);
    }

    if (tokenList->tokens[currentToken].type != TOKEN_SEMICOLON) {
        err->value = ERR_SYNTAX;
        sprintf(err->message, "Expected ';' , got %s",
                tokenList->tokens[currentToken].value);

        freeAstNode(expression);
        freeAstNode(initNode);

        return NULL;
    }

    return newBinaryOperatorNode(TOKEN_EQUAL, initNode, expression);
}


astNode *parseVarDeclaration(TokenList *tokenList, int *currentToken, error *err) {
    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInstructionError(err);
    }


    int typed = 0;
    varType type = VOID;

    if (tokenList->tokens[*currentToken].type == TOKEN_KEYWORD) {
        type = typeFromString(tokenList->tokens[*currentToken].value);
        typed = 1;
        ++*currentToken;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInstructionError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        err->value = ERR_SYNTAX;
        sprintf(err->message, "Expected an identifier, got %s",
                tokenList->tokens[*currentToken].value);
        return NULL;
    }

    char *name = strdup(tokenList->tokens[*currentToken].value);
    ++*currentToken;
    astNode *initNode = newInitializationNode(name, typed, type);

    return initNode;
}
