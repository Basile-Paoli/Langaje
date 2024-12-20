//
// Created by Basile Paoli on 10/5/24.
//

#ifndef LANGAJE_PARSER_H
#define LANGAJE_PARSER_H

#include "../ast/ast.h"
#include "../lexer/token.h"
#include "../errors/errors.h"


void *addPositionToError(error *err, Token token);

InstructionBlock *parse(TokenList *tokenList, error *err);

InstructionBlock *parseInstructions(TokenList *tokenList, int *currentToken, error *err);

InstructionBlock *parseInstructionBlockWithBraces(TokenList *tokenList, int *currentToken, error *err);

astNode *parseFunctionOrInstruction(TokenList *tokenList, int *currentToken, error *err);

astNode *parseInstruction(TokenList *tokenList, int *currentToken, error *err);

#endif //LANGAJE_PARSER_H
