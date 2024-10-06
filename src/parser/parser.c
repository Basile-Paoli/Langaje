//
// Created by Basile Paoli on 10/5/24.
//

#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "var_declaration.h"


astNode *parse(TokenList *tokenList, error *err) {
    if (tokenList->nb_tokens == 0) {
        err->value = ERR_SYNTAX;
        sprintf(err->message, "Empty program");
        return NULL;
    }
    Token first = tokenList->tokens[0];
    if (first.type == TOKEN_KEYWORD && strcmp(first.value, "def") == 0) {
        return parseVarDeclarationInstruction(tokenList, err);
    }
}


