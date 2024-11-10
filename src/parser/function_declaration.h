#ifndef LANGAJE_FUNCTION_DECLARATION_H
#define LANGAJE_FUNCTION_DECLARATION_H

#include "../ast/ast.h"
#include "../lexer/token.h"

astNode *parseReturnInstruction(TokenList *tokenList, int *currentToken, error *err);

astNode *parseFunctionDeclaration(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_FUNCTION_DECLARATION_H
