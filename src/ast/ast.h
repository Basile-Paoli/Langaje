//
// Created by Basile on 02/10/2024.
//

#ifndef LANGAJE_AST_H
#define LANGAJE_AST_H

#include "../types/types.h"

typedef enum {
    VARIABLE,
    OPERATOR,
    VALUE,
    INITIALIZATION,
    BLOCK,
    CONDITION,
    LOOP,
} astNodeType;
typedef enum operator {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    MODULUS,
    EXPONENTIATION,
    ASSIGNMENT,
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AND,
    OR,
    NOT,
} operator;

typedef struct initializationNode {
    char *name;
    int typed;
    varType type;
} initializationNode;

typedef union {
    operator operator;
    var value;
    char *variable;
    initializationNode initialization;
    struct InstructionBlock *block;
} astNodeValue;

typedef struct astNode {
    astNodeType type;
    astNodeValue value;
    struct astNode **children;
    int childrenCount;
} astNode;

typedef struct InstructionBlock {
    astNode **instructions;
    int instructionsCount;
    int capacity;
} InstructionBlock;

void printAstNode(astNode *node, int depth);

void printAST(astNode *root, int depth);

void printInstructionBlock(InstructionBlock *block, int depth);

const char *operatorToString(operator operator);


astNode *newVariableNode(char *variable);

astNode *newOperatorNode(operator operator);

astNode *newInitializationNode(char *name, int typed, varType type);

astNode *newValueNode(var value);

astNode *newInstructionBlockNode(InstructionBlock *block);

astNode *newConditionNode(astNode *condition, astNode *ifBlock, astNode *elseBlock);

void freeAstNode(astNode *node);

void testPrintAst();

InstructionBlock *newInstructionBlock(int capacity);

InstructionBlock *appendInstruction(InstructionBlock *parseResult, astNode *instruction);

void freeInstructionBlock(InstructionBlock *parseResult);

#endif
