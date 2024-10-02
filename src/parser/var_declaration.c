//
// Created by user on 10/5/24.
//

#include "var_declaration.h"
#include "expression.h"
#include "../ast/node_initializers.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

varType typeFromString(const char *string, error *err) {
    if (strcmp(string, "int") == 0) {
        return _int;
    } else if (strcmp(string, "float") == 0) {
        return _float;
    } else if (strcmp(string, "char") == 0) {
        return _char;
    } else if (strcmp(string, "string") == 0) {
        return _string;
    } else {
        err->value = ERR_SYNTAX;
        err->message = malloc(strlen("Unknown type ") + strlen(string) + 1);
        sprintf(err->message, "Unknown type %s", string);
        return _int;
    }
}


astNode *parseVarDeclarationInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(strcmp(tokenList->tokens[*currentToken].value, "def") == 0);


    astNode *initNode = parseVarDeclaration(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        freeAstNode(initNode);
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type == TOKEN_SEMICOLON) {
        if (initNode->value.initialization.typed == 0) {
            err->value = ERR_SYNTAX;
            err->message = strdup("Cannot declare a variable without a type or an initialization");
            freeAstNode(initNode);
            return NULL;
        }
        return initNode;
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_EQUAL) {
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected '=' or ';' , got ") + strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected '=' or ';' , got %s",
                tokenList->tokens[*currentToken].value);
        freeAstNode(initNode);
        return NULL;
    }

    ++*currentToken;
    astNode *expression = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        freeAstNode(initNode);
        return NULL;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        freeAstNode(expression);
        freeAstNode(initNode);
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_SEMICOLON) {
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected ';' , got ") + strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected ';' , got %s",
                tokenList->tokens[*currentToken].value);

        freeAstNode(expression);
        freeAstNode(initNode);

        return NULL;
    }
    ++*currentToken;

    return newBinaryOperatorNode(TOKEN_EQUAL, initNode, expression);
}


astNode *parseVarDeclaration(TokenList *tokenList, int *currentToken, error *err) {
    assert(strcmp(tokenList->tokens[*currentToken].value, "def") == 0);
    ++*currentToken;

    int typed = 0;
    varType type;

    if (tokenList->tokens[*currentToken].type == TOKEN_KEYWORD) {
        type = typeFromString(tokenList->tokens[*currentToken].value, err);
        if (err->value != ERR_SUCCESS) {
            return NULL;
        }
        typed = 1;
        ++*currentToken;
    }

    if (*currentToken >= tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        err->value = ERR_SYNTAX;
        err->message = malloc(
                strlen("Expected an identifier, got ") + strlen(tokenList->tokens[*currentToken].value) + 1);
        sprintf(err->message, "Expected an identifier, got %s",
                tokenList->tokens[*currentToken].value);
        return NULL;
    }

    char *name = strdup(tokenList->tokens[*currentToken].value);
    ++*currentToken;
    astNode *initNode = newInitializationNode(name, typed, type);


    return initNode;
}