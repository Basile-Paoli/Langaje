//
// Created by Basile Paoli on 10/5/24.
//

#ifndef LANGAJE_PARSER_H
#define LANGAJE_PARSER_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"

astNode *parse(TokenList *tokenList, error *err);

#endif //LANGAJE_PARSER_H
