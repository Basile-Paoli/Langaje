//
// Created by Basile Paoli on 10/5/24.
//

#ifndef LANGAJE_PARSER_H
#define LANGAJE_PARSER_H

#include "../ast/ast.h"
#include "../lexer/token.h"

astNode *parse(TokenList *tokenList);

#endif //LANGAJE_PARSER_H
