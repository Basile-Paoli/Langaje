//
// Created by Basile Paoli on 10/5/24.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

astNode *parse(TokenList *tokenList, error *err) {
    if (tokenList->nb_tokens == 0) {
        err->value = ERR_SYNTAX;
        err->message = strdup("Empty program");
        return NULL;
    }
    Token first = tokenList->tokens[0];
    if (first.type == TOKEN_KEYWORD && strcmp(first.value, "def") == 0) {
        return parseVarDeclarationInstruction(tokenList, err);
    }
    if (isAssignment(tokenList, err)) {
        return parseAssignmentInstruction(tokenList, err);
    }
    return parseExpressionInstruction(tokenList, err);
}


