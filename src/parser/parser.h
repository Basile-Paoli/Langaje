//
// Created by Basile Paoli on 10/5/24.
//

#ifndef LANGAJE_PARSER_H
#define LANGAJE_PARSER_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

typedef struct {
    astNode **instructions;
    int instructionsCount;
} ParseResult;

ParseResult *parse(TokenList *tokenList, error *err);

astNode *parseInstruction(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_PARSER_H
