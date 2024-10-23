//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_NODE_INITIALIZERS_H
#define LANGAJE_NODE_INITIALIZERS_H

#include "ast.h"
#include "../lexer/token.h"

astNode *newBinaryOperatorNode(TokenType token, astNode *left, astNode *right);

astNode *newUnaryOperatorNode(TokenType token, astNode *child);

astNode *newSubscriptNode(astNode *array, astNode *index);

astNode *intTokenToNode(Token token);

astNode *stringTokenToNode(Token token);

astNode *identifierTokenToNode(Token token);

astNode **newChildren(astNode *firstChild);

astNode **appendChild(astNode **children, int childrenCount, astNode *child);

void freeChildren(astNode **children, int childrenCount);

#endif //LANGAJE_NODE_INITIALIZERS_H
