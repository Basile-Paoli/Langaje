//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_VAR_DECLARATION_H
#define LANGAJE_VAR_DECLARATION_H


#include "../ast/ast.h"
#include "../lexer/token.h"

astNode *parseVarDeclarationInstruction(TokenList *tokenList);

astNode *parseVarDeclaration(TokenList *tokenList, int *currentToken);

#endif //LANGAJE_VAR_DECLARATION_H
