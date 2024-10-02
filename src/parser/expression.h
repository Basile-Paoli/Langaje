//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_EXPRESSION_H
#define LANGAJE_EXPRESSION_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"


astNode *parseExpression(TokenList *tokenList, int *currentToken, error *err);

astNode *parseTerm(TokenList *tokenList, int *currentToken, error *err);

astNode *parseFactor(TokenList *tokenList, int *currentToken, error *err);

astNode *parseExponent(TokenList *tokenList, int *currentToken, error *err);

astNode *parseExpressionInstruction(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_EXPRESSION_H