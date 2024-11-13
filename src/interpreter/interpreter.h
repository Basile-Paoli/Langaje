#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../ast/ast.h"
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../functions/functions.h"
#include <stdlib.h>
#include <string.h>

#define MAX_UNCHANGED_LOOP_DEPTH 4096

var* subsituteValue(astNode* value, hmStack* stack, error *err);
astNode* calculateNode(astNode** values, astNode* node,hmStack* stack,int valuesAmount, error *err);
int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hmStack* stack, error *err);
astNode* computeNode(astNode* node, hmStack* stack, hm* functionMap, error *err);
int runInstructionBlock(InstructionBlock* program, hmStack* stack, hm* functionMap, error *err);
void debug(char key[][255], int arrSize, hmStack* stack, error *err);
void displayHashmap(hmStack* stack, error* err);
int declareFunction(astNode* node,hmStack* stack,hm* functionMap,error* err);
int runWhileLoop(astNode* node,hmStack* stack,hm* functionMap,error* err);
int runForLoop(astNode *node, hmStack *stack, hm* functionMap, error *err);
var* declareArray(astNode* node, initType* type, hmStack* stack, error *err);
void displayFunctionMap(hm* functionMap, error* err);
astNode* runFunction(astNode* node, hmStack* stack, hm* functionMap, error* err);
astNode* runBuiltinFunction(astNode*, hmStack* stack, hm* functionMap, function* fun, error* err);

#endif