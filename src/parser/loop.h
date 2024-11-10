#ifndef LANGAJE_LOOP_H
#define LANGAJE_LOOP_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

astNode *parseWhileInstruction(TokenList *tokenList, int *currentToken, error *err);

astNode *parseForInstruction(TokenList *tokenList, int *currentToken, error *err);

astNode *parseBreakInstruction(TokenList *tokenList, int *currentToken, error *err);

astNode *parseContinueInstruction(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_LOOP_H
