//
// Created by Basile on 02/10/2024.
//

#ifndef LANGAJE_AST_H
#define LANGAJE_AST_H

typedef enum {
    //Placeholder
    TOKEN
} astNodeType;

typedef union {
    //Placeholder
    int token;
} astNodeValue;

typedef struct astNode {
    astNodeType type;
    astNodeValue value;
    struct astNode *left;
    struct astNode *right;
} astNode;
#endif //LANGAJE_AST_H
