#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Basile on 02/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define PRINT_SPACE 5


astNode *newVariableNode(char *variable) {
    astNode *node = malloc(sizeof(astNode));
    node->type = VARIABLE;
    node->value.variable = strdup(variable);
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}


astNode *newOperatorNode(operator operator) {
    astNode *node = malloc(sizeof(astNode));
    node->type = OPERATOR;
    node->value.operator = operator;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}


astNode *newInitializationNode(char *name, int typed, varType type) {
    astNode *node = malloc(sizeof(astNode));
    node->type = INITIALIZATION;
    node->value.initialization.name = malloc(sizeof(char) * strlen(name));
    strcpy(node->value.initialization.name, name);
    node->value.initialization.typed = typed;
    node->value.initialization.type = type;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}


astNode *newValueNode(var value) {
    astNode *node = malloc(sizeof(astNode));
    node->type = VALUE;
    node->value.value = value;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}


void freeAstNode(astNode *node) {
    if (node == NULL) return;

    if (node->type == VARIABLE) {
        free(node->value.variable);
    } else if (node->type == INITIALIZATION) {
        free(node->value.initialization.name);
    }
    for (int i = 0; i < node->childrenCount; i++) {
        freeAstNode(node->children[i]);
    }
    free(node->children);
    free(node);
}

void printValue(var value) {
    switch (value.type) {
        case _int:
            printf("%d", value.value._int);
            break;

        case _float:
            printf("%f", value.value._float);
            break;

        case _char:
            printf("%c", value.value._char);
            break;

        case _string:
            printf("%s", value.value._string);
            break;
        case _array:
            printf("[");
            for (int i = 0; i < value.value._array->length; i++) {
                printValue(value.value._array->values[i]);
                if (i < value.value._array->length - 1) {
                    printf(", ");
                }
            }
            printf("]");
            break;

    }
    printf("\n");
}

void printAstNode(astNode *node) {
    switch (node->type) {
        case VARIABLE:
            printf("Variable: %s\n", node->value.variable);
            break;
        case OPERATOR:
            printf("Operator: %s\n", operatorToString(node->value.operator));
            break;
        case VALUE:
            printValue(node->value.value);
            break;
        case INITIALIZATION:
            printf("Initialization: %s\n", node->value.initialization.name);
            break;
    }
}

const char *operatorToString(operator operator) {
    switch (operator) {
        case ADDITION:
            return "ADDITION";
        case SUBTRACTION:
            return "SUBTRACTION";
        case MULTIPLICATION:
            return "MULTIPLICATION";
        case DIVISION:
            return "DIVISION";
        case MODULUS:
            return "MODULUS";
        case EXPONENTIATION:
            return "EXPONENTIATION";
        case ASSIGNMENT:
            return "ASSIGNMENT";
    }
}


void printAST(astNode *root, int depth) {
    for (int i = 0; i < depth * PRINT_SPACE; i++) {
        printf(" ");
    }
    printAstNode(root);
    for (int i = 0; i < root->childrenCount; i++) {
        printAST(root->children[i], depth + 1);
    }
}

void testPrintAst() {
    astNode *root = newOperatorNode(ADDITION);
    root->children = malloc(sizeof(astNode *) * 2);
    root->children[0] = newVariableNode("a");
    astNode *mul = newOperatorNode(MULTIPLICATION);
    mul->children = malloc(sizeof(astNode *) * 2);
    mul->children[0] = newVariableNode("b");
    mul->children[1] = newVariableNode("c");
    mul->childrenCount = 2;
    root->children[1] = mul;
    root->childrenCount = 2;
    printAST(root, 0);
    freeAstNode(root);
}

#pragma clang diagnostic pop