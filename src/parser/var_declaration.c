//
// Created by user on 10/5/24.
//

#include "var_declaration.h"
#include "expression.h"
#include "../ast/node_initializers.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

//To be implemented
varType typeFromString(const char *string) {
    return FLOAT;
}


astNode *parseVarDeclarationInstruction(TokenList *tokenList) {
    // Assert first token is def
    assert(strcmp(tokenList->tokens[0].value, "def") == 0);

    if (tokenList->nb_tokens < 3) {
        // Error Handling
    }

    int currentToken = 1;
    astNode *initNode = parseVarDeclaration(tokenList, &currentToken);

    if (tokenList->tokens[currentToken].type == TOKEN_SEMICOLON) {
        return initNode;
    } else if (tokenList->tokens[currentToken].type != TOKEN_EQUAL) {
        // Error Handling
    }

    ++currentToken;
    astNode *expression = parseExpression(tokenList, &currentToken);

    if (tokenList->tokens[currentToken].type != TOKEN_SEMICOLON) {
        // Error Handling
    }

    return newBinaryOperatorNode(TOKEN_EQUAL, initNode, expression);
}


astNode *parseVarDeclaration(TokenList *tokenList, int *currentToken) {
    int typed = 0;
    varType type = VOID;
    if (tokenList->tokens[*currentToken].type == TOKEN_KEYWORD) {
        type = typeFromString(tokenList->tokens[*currentToken].value);
        typed = 1;
        ++*currentToken;
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        // Error Handling
    }

    char *name = strdup(tokenList->tokens[*currentToken].value);
    ++*currentToken;
    astNode *initNode = newInitializationNode(name, typed, type);
    return initNode;
}
