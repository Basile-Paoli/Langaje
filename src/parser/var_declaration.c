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

varType typeFromKeyword(const TokenType type, error *err) {
    switch (type) {
        case TOKEN_TYPE_INT:
            return _int;
        case TOKEN_TYPE_FLOAT:
            return _float;
        case TOKEN_TYPE_STRING:
            return _string;
        default:
            err->value = ERR_SYNTAX;
            err->message = strdup("Invalid type keyword");
            return _int;
    }
}

void addArrayToType(initType *type, int size) {
    if (type->type == _array) {
        addArrayToType(type->elementsType, size);
        return;
    } else {
        type->elementsType = malloc(sizeof(initType));
        type->elementsType->type = type->type;
        type->arraySize = size;
        type->type = _array;
        return;
    }
}

initType parseType(TokenList *tokenList, int *currentToken, error *err) {
    initType type;
    type.type = typeFromKeyword(tokenList->tokens[*currentToken].type, err);
    if (err->value != ERR_SUCCESS) {
        return type;
    }

    ++*currentToken;
    if (*currentToken >= tokenList->nb_tokens) {
        endOfInputError(err);
        return type;
    }

    while (tokenList->tokens[*currentToken].type == TOKEN_LBRACKET) {
        ++*currentToken;
        if (*currentToken + 1 >= tokenList->nb_tokens) {
            endOfInputError(err);
            return type;
        }
        if (tokenList->tokens[*currentToken].type == TOKEN_TYPE_INT) {
            addArrayToType(&type, atoi(tokenList->tokens[*currentToken].value));

            ++*currentToken;
            if (tokenList->tokens[*currentToken].type != TOKEN_RBRACKET) {
                err->value = ERR_SYNTAX;
                err->message = strdup("Expected ']'");
                return type;
            }
            ++*currentToken;
        } else if (tokenList->tokens[*currentToken].type == TOKEN_RBRACKET) {
            addArrayToType(&type, 0);
            ++*currentToken;
        } else {
            err->value = ERR_SYNTAX;
            err->message = strdup("Invalid token after '['");
            return type;
        }
    }

    return type;
}

astNode *parseVarDeclarationInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_DEF);


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
        ++*currentToken;
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

int isTypeKeyword(TokenType type) {
    return type == TOKEN_TYPE_INT || type == TOKEN_TYPE_FLOAT || type == TOKEN_TYPE_STRING;
}


astNode *parseVarDeclaration(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_DEF);
    ++*currentToken;

    int typed = 0;
    initType type;

    if (isTypeKeyword(tokenList->tokens[*currentToken].type)) {
        type = parseType(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            return NULL;
        }
        typed = 1;
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
