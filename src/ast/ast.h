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
} astNodeType;
typedef enum operator {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    MODULUS,
    EXPONENTIATION,
    ASSIGNMENT,
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
} astNodeValue;

typedef struct astNode {
    astNodeType type;
    astNodeValue value;
    struct astNode **children;
    int childrenCount;
} astNode;

void printAstNode(astNode *node);

void printAST(astNode *root, int depth);

const char *operatorToString(operator operator);

astNode *newVariableNode(char *variable);

astNode *newOperatorNode(operator operator);

astNode *newInitializationNode(char *name, int typed, varType type);

astNode *newValueNode(var value);

void freeAstNode(astNode *node);

void testPrintAst();

#endif
