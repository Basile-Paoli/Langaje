//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_EXPRESSION_H
#define LANGAJE_EXPRESSION_H

#include "../ast/ast.h"
#include "../lexer/token.h"

astNode *parseExpression(TokenList *tokenList, int *currentToken);

astNode *parseTerm(TokenList *tokenList, int *currentToken);

astNode *parseFactor(TokenList *tokenList, int *currentToken);

astNode *parseExponent(TokenList *tokenList, int *currentToken);

#endif //LANGAJE_EXPRESSION_H
