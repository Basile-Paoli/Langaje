#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Basile on 02/10/2024.
//

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

#define PRINT_SPACE 5


void printAST(astNode *root, int space) {
    if (root == NULL) {
        return;
    }

    space += PRINT_SPACE;

    printAST(root->right, space);

    printf("\n");
    for (int i = PRINT_SPACE; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->value.token);

    printAST(root->left, space);
}

void freeAST(astNode *root) {
    if (root == NULL) {
        return;
    }

    freeAST(root->left);
    freeAST(root->right);
    free(root);
}


int main(int argc, char *argv[]) {
    astNode *root = malloc(sizeof(astNode));
    root->value.token = 1;
    //Ignore type
    root->left = malloc(sizeof(astNode));
    root->right = malloc(sizeof(astNode));
    root->right->value.token = 3;
    root->right->left = NULL;
    root->right->right = NULL;
    root->left->value.token = 2;
    root->left->left = NULL;
    root->left->right = malloc(sizeof(astNode));
    root->left->right->value.token = 4;
    root->left->right->left = NULL;
    root->left->right->right = malloc(sizeof(astNode));
    root->left->right->right->value.token = 5;
    root->left->right->right->left = NULL;
    root->left->right->right->right = NULL;


    printAST(root, 0);

    freeAST(root);
}

#pragma clang diagnostic pop