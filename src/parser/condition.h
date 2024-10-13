//
// Created by user on 10/13/24.
//

#ifndef LANGAJE_CONDITION_H
#define LANGAJE_CONDITION_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

astNode *parseIfInstruction(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_CONDITION_H
