#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../ast/ast.h"
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../functions/functions.h"
#include <stdlib.h>

var subsituteValue(astNode* value, hmStack* stack, error *err);
void calculateNode(astNode** values, astNode* node,hmStack* stack,int valuesAmount, error *err);
int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hmStack* stack, error *err);
astNode* computeNode(astNode* node, hmStack* stack, error *err);
int runInstructionBlock(InstructionBlock* program, hmStack* stack, error *err);
void debug(char key[][255], int arrSize, hmStack* stack, error *err);
var* declareArray(astNode* node, initType* type, hmStack* stack, error *err);
#endif