#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../ast/ast.h"
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../functions/functions.h"
#include <stdlib.h>

void subsituteValue(astNode* value, hmStack* stack);
void calculateNode(astNode** values, astNode* node,hmStack* stack,int valuesAmount);
int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hmStack* stack);
astNode* computeNode(astNode* node, hmStack* stack);
int runInstructionBlock(InstructionBlock* program, hmStack* stack);
void debug(char key[][255], int arrSize, hmStack* stack);
#endif