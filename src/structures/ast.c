#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Basile on 02/10/2024.
//

#include <stdio.h>
#include "ast.h"

#define PRINT_SPACE 5


void printAstNode(astNode *node) {
    switch (node->type) {
        case VARIABLE:
            printf("Variable: %s\n", node->value.variable);
            break;
        case OPERATOR:
            printf("Operator: %d\n", node->value.operator);
            break;
        case VALUE:
//            printf("Value: %d\n", node->value.value);
            break;
        case INITIALIZATION:
            printf("Initialization: %s\n", node->value.initialization.name);
            break;
    }
}

void printAST(astNode *root, int depth) {
    for (int i = PRINT_SPACE; i < depth * PRINT_SPACE; i++) {
        printf(" ");
    }
    printAstNode(root);
    for (int i = 0; i < root->childrenCount; i++) {
        printAST(root->children[i], depth + 1);
    }
}


#pragma clang diagnostic pop