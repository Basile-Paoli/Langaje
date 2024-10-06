//
// Created by user on 10/6/24.
//

#ifndef LANGAJE_ASSIGNMENT_H
#define LANGAJE_ASSIGNMENT_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

astNode *parseAssignmentInstruction(TokenList *tokenList, error *err);

#endif //LANGAJE_ASSIGNMENT_H
