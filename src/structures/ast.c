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
    node->value.variable = malloc(sizeof(char) * strlen(variable));
    strcpy(node->value.variable, variable);
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


void freeAstNode(astNode *node) {
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

void printAstNode(astNode *node) {
    switch (node->type) {
        case VARIABLE:
            printf("Variable: %s\n", node->value.variable);
            break;
        case OPERATOR:
            printf("Operator: %s\n", operatorToString(node->value.operator));
            break;
        case VALUE:
//            printf("Value: %d\n", node->value.value);
            break;
        case INITIALIZATION:
            printf("Initialization: %s\n", node->value.initialization.name);
            break;
    }
}

char *operatorToString(operator operator) {
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

int main() {
    // a + b * c
    astNode *root = newOperatorNode(ADDITION);
    root->children = malloc(sizeof(astNode *) * 2);
    root->children[0] = newVariableNode("a");
    root->children[1] = newOperatorNode(MULTIPLICATION);
    root->childrenCount = 2;
    root->children[1]->children = malloc(sizeof(astNode *) * 2);
    root->children[1]->children[0] = newVariableNode("b");
    root->children[1]->children[1] = newVariableNode("c");
    root->children[1]->childrenCount = 2;
    printAST(root, 0);
    freeAstNode(root);
    return 0;
}

#pragma clang diagnostic pop