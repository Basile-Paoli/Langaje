//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_VAR_DECLARATION_H
#define LANGAJE_VAR_DECLARATION_H


#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

astNode *parseVarDeclarationInstruction(TokenList *tokenList, int *currentTokrn, error *err);

astNode *parseVarDeclaration(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_VAR_DECLARATION_H
