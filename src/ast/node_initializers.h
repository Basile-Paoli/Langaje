//
// Created by user on 10/5/24.
//

#ifndef LANGAJE_NODE_INITIALIZERS_H
#define LANGAJE_NODE_INITIALIZERS_H

#include "ast.h"
#include "../lexer/token.h"

astNode *newBinaryOperatorNode(TokenType token, astNode *left, astNode *right);

astNode *numberTokenToNode(Token token);

astNode *stringTokenToNode(Token token);

astNode *identifierTokenToNode(Token token);

#endif //LANGAJE_NODE_INITIALIZERS_H
