//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_EXPRESSION_H
#define LANGAJE_EXPRESSION_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

astNode *parseExpression(TokenList *tokenList, int *currentToken, error *err);

astNode *parseLogicalOr(TokenList *tokenList, int *currentToken, error *err);

astNode *parseLogicalAnd(TokenList *tokenList, int *currentToken, error *err);

astNode *parseEquality(TokenList *tokenList, int *currentToken, error *err);

astNode *parseComparison(TokenList *tokenList, int *currentToken, error *err);

astNode *parseAddition(TokenList *tokenList, int *currentToken, error *err);

astNode *parseMultiplication(TokenList *tokenList, int *currentToken, error *err);

astNode *parseUnaryOperators(TokenList *tokenList, int *currentToken, error *err);

astNode *parsePrimary(TokenList *tokenList, int *currentToken, error *err);

astNode *parseExpressionInstruction(TokenList *tokenList, int *currentToken, error *err);

astNode *parseParenthesisExpression(TokenList *tokenList, int *currentToken, error *err);

astNode *parseBracketExpression(TokenList *tokenList, int *currentToken, error *err);

astNode *parseArray(TokenList *tokenList, int *currentToken, error *err);

astNode *parseIdentifier(TokenList *tokenList, int *currentToken, error *err);

astNode *parseFunctionCall(TokenList *tokenList, int *currentToken, error *err);

astNode **parseExpressionsSeparatedByCommas(TokenList *tokenList, int *currentToken, int *nbExpressions, error *err);

#endif //LANGAJE_EXPRESSION_H
