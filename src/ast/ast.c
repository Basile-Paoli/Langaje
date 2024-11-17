#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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


astNode *newInitializationNode(char *name, int typed, initType type) {
    astNode *node = malloc(sizeof(astNode));
    node->type = INITIALIZATION;
    node->value.initialization.name = malloc(sizeof(char) * (strlen(name) + 1));
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


astNode *newInstructionBlockNode(InstructionBlock *block) {
    astNode *node = malloc(sizeof(astNode));
    node->type = BLOCK;
    node->value.block = block;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}


astNode *newConditionNode(astNode *condition, astNode *ifBlock, astNode *elseBlock) {
    astNode *node = malloc(sizeof(astNode));
    node->type = CONDITION;
    node->children = malloc(sizeof(astNode *) * 3);
    node->children[0] = condition;
    node->children[1] = ifBlock;
    node->children[2] = elseBlock;
    node->childrenCount = 3;
    return node;
}


astNode *newWhileNode(astNode *condition, astNode *block) {
    astNode *node = malloc(sizeof(astNode));
    node->type = WHILE_LOOP;
    node->children = malloc(sizeof(astNode *) * 2);
    node->children[0] = condition;
    node->children[1] = block;
    node->childrenCount = 2;
    return node;
}

astNode *newForNode(char *variable, astNode **children, int childrenCount) {
    astNode *node = malloc(sizeof(astNode));
    node->type = FOR_LOOP;
    node->value.variable = variable;
    node->children = children;
    node->childrenCount = childrenCount;
    return node;
}


astNode *newBreakNode() {
    astNode *node = malloc(sizeof(astNode));
    node->type = BREAK;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}

astNode *newContinueNode() {
    astNode *node = malloc(sizeof(astNode));
    node->type = CONTINUE;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}


astNode *newArrayNode(int size, astNode **values) {
    astNode *node = malloc(sizeof(astNode));
    node->type = ARRAY;
    node->childrenCount = size;
    node->children = values;
    return node;
}

astNode *newFunctionCallNode(char *name, astNode **arguments, int argumentsCount) {
    astNode *node = malloc(sizeof(astNode));
    node->type = FUNCTION_CALL;
    node->value.functionCall.name = strdup(name);
    node->children = arguments;
    node->childrenCount = argumentsCount;
    return node;
}


astNode *newFunctionDeclarationNode(
        char *name,
        functionParameter *parameters,
        int parameterCount,
        int voidReturn,
        initType returnType,
        astNode *block
) {
    astNode *node = malloc(sizeof(astNode));
    node->type = FUNCTION_DECLARATION;
    node->value.functionDeclaration.name = strdup(name);
    node->value.functionDeclaration.parameters = parameters;
    node->value.functionDeclaration.parametersCount = parameterCount;
    node->value.functionDeclaration.voidReturn = voidReturn;
    node->value.functionDeclaration.returnType = returnType;
    node->children = malloc(sizeof(astNode *));
    node->children[0] = block;
    node->childrenCount = 1;
    return node;
}

astNode *newEmptyReturnNode() {
    astNode *node = malloc(sizeof(astNode));
    node->type = RETURN;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}

astNode *newReturnValueNode(astNode *value) {
    astNode *node = malloc(sizeof(astNode));
    node->type = RETURN;
    node->children = malloc(sizeof(astNode *));
    node->children[0] = value;
    node->childrenCount = 1;
    return node;
}


astNode *newMemoryDumpNode() {
    astNode *node = malloc(sizeof(astNode));
    node->type = MEMORY_DUMP;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}

astNode *newBreakpointNode() {
    astNode *node = malloc(sizeof(astNode));
    node->type = BREAKPOINT;
    node->children = NULL;
    node->childrenCount = 0;
    return node;
}

void freeInitType(initType type) {
    if (type.type == _array) {
        freeInitType(*type.elementsType);
        free(type.elementsType);
        type.elementsType = NULL;
    }
}

void freeFunctionParameter(functionParameter parameter) {
    free(parameter.name);
    parameter.name = NULL;
    freeInitType(parameter.type);
}

void freeAstNode(astNode *node) {
    if (node == NULL) return;

    if (node->type == VARIABLE) {
        free(node->value.variable);
        node->value.variable = NULL;
    } else if (node->type == INITIALIZATION) {
        free(node->value.initialization.name);
        node->value.initialization.name = NULL;
    } else if (node->type == VALUE && node->value.value.type == _string) {
        free(node->value.value.value._string);
        node->value.value.value._string = NULL;
    } else if (node->type == BLOCK) {
        freeInstructionBlock(node->value.block);
        node->value.block = NULL;
    } else if (node->type == CONDITION) {
        freeAstNode(node->children[0]);
        freeAstNode(node->children[1]);
        freeAstNode(node->children[2]);
    } else if (node->type == FUNCTION_DECLARATION) {
        free(node->value.functionDeclaration.name);
        node->value.functionDeclaration.name = NULL;
        for (int i = 0; i < node->value.functionDeclaration.parametersCount; i++) {
            freeFunctionParameter(node->value.functionDeclaration.parameters[i]);
        }
        free(node->value.functionDeclaration.parameters);
        node->value.functionDeclaration.parameters = NULL;
    } else if (node->type == FUNCTION_CALL) {
        free(node->value.functionCall.name);
        node->value.functionCall.name = NULL;
    }
    for (int i = 0; i < node->childrenCount; i++) {
        freeAstNode(node->children[i]);
        node->children[i] = NULL;
    }
    free(node->children);
    node->children = NULL;
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

void printType(initType type) {
    switch (type.type) {
        case _int:
            printf("int");
            break;
        case _float:
            printf("float");
            break;
        case _char:
            printf("char");
            break;
        case _string:
            printf("string");
            break;
        case _array:
            printf("array(%d) of ", type.arraySize);
            printType(*type.elementsType);
            break;

    }
}

void printFunctionDeclaration(astNode *node) {
    printf("Function declaration: %s\n", node->value.functionDeclaration.name);
    for (int i = 0; i < node->value.functionDeclaration.parametersCount; i++) {
        for (int j = 0; j < PRINT_SPACE; j++) {
            printf(" ");
        }
        printf("Parameter %d: %s ", i, node->value.functionDeclaration.parameters[i].name);
        printType(node->value.functionDeclaration.parameters[i].type);
        printf("\n");
    }
    for (int i = 0; i < PRINT_SPACE; i++) {
        printf(" ");
    }
    if (node->value.functionDeclaration.voidReturn) {
        printf("Return type: void\n");
    } else {
        printf("Return type: ");
        printType(node->value.functionDeclaration.returnType);
        printf("\n");
    }
}

void printAstNode(astNode *node, int depth) {
    for (int i = 0; i < depth * PRINT_SPACE; i++) {
        printf(" ");
    }
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
            printf("Initialization: %s ", node->value.initialization.name);
            if (node->value.initialization.typed) {
                printType(node->value.initialization.type);
            }
            printf("\n");
            break;
        case BLOCK:
            printf("Block\n");
            printInstructionBlock(node->value.block, depth + 1);
            break;
        case CONDITION:
            printf("Condition\n");
            break;
        case WHILE_LOOP:
            printf("While\n");
            break;
        case FOR_LOOP:
            printf("For %s\n", node->value.variable);
            break;
        case FOREACH_LOOP:
            printf("Foreach %s\n", node->value.variable);
            break;
        case BREAK:
            printf("Break\n");
            break;
        case CONTINUE:
            printf("Continue\n");
            break;
        case ARRAY:
            printf("Array\n");
            break;
        case FUNCTION_CALL:
            printf("Function call: %s\n", node->value.functionCall.name);
            break;
        case FUNCTION_DECLARATION: {
            printFunctionDeclaration(node);
            break;
        }
        case RETURN:
            printf("Return\n");
            break;
        case BREAKPOINT:
            printf("Breakpoint\n");
            break;
        case MEMORY_DUMP:
            printf("Memory dump\n");
            break;
    }
}


void printInstructionBlock(InstructionBlock *block, int depth) {
    for (int i = 0; i < block->instructionsCount; i++) {
        printAST(block->instructions[i], depth);
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
        case EQUAL:
            return "EQUAL";
        case NOT_EQUAL:
            return "NOT_EQUAL";
        case GREATER:
            return "GREATER";
        case GREATER_EQUAL:
            return "GREATER_EQUAL";
        case LESS:
            return "LESS";
        case LESS_EQUAL:
            return "LESS_EQUAL";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case NOT:
            return "NOT";
        case UNARY_MINUS:
            return "UNARY_MINUS";
        case UNARY_PLUS:
            return "UNARY_PLUS";
        case SUBSCRIPT:
            return "SUBSCRIPT";
    }
}


void printAST(astNode *root, int depth) {
    if (root == NULL) return;
    printAstNode(root, depth);
    for (int i = 0; i < root->childrenCount; i++) {
        printAST(root->children[i], depth + 1);
    }
}

InstructionBlock *newInstructionBlock(int capacity) {
    assert(capacity > 0);
    InstructionBlock *parseResult = malloc(sizeof(InstructionBlock));
    parseResult->instructions = malloc(sizeof(astNode *) * capacity);
    parseResult->instructionsCount = 0;
    parseResult->capacity = capacity;
    return parseResult;
}

InstructionBlock *appendInstruction(InstructionBlock *parseResult, astNode *instruction) {
    assert(parseResult != NULL);
    if (parseResult->instructionsCount == parseResult->capacity) {
        parseResult->capacity *= 2;
        parseResult->instructions = realloc(parseResult->instructions,
                                            sizeof(astNode *) * parseResult->capacity);
    }
    parseResult->instructions[parseResult->instructionsCount] = instruction;
    parseResult->instructionsCount++;
    return parseResult;
}

void freeInstructionBlock(InstructionBlock *block) {
    if (block == NULL) return;
    for (int i = 0; i < block->instructionsCount; i++) {
        freeAstNode(block->instructions[i]);
        block->instructions[i] = NULL;
    }
    free(block->instructions);
    block->instructions = NULL;
    free(block);
}

/*
* copyAstNode is a recursive function that copies an astNode and all its children.
* It is only used to copy nodes part of an expression, so it does not handle other
* cases like blocks or function declarations.
*/
astNode *copyAstNode(astNode *node) {
    if (node == NULL) return NULL;
    astNode *copy = malloc(sizeof(astNode));
    copy->type = node->type;
    copy->childrenCount = node->childrenCount;
    switch (node->type) {
        case VARIABLE:
            copy->value.variable = strdup(node->value.variable);
            break;
        case OPERATOR:
            copy->value.operator = node->value.operator;
            break;
        case VALUE:
            copy->value.value = node->value.value;
            break;
        case INITIALIZATION:
            copy->value.initialization.name = strdup(node->value.initialization.name);
            copy->value.initialization.typed = node->value.initialization.typed;
            copy->value.initialization.type = node->value.initialization.type;
            break;
        case FUNCTION_CALL:
            copy->value.functionCall.name = strdup(node->value.functionCall.name);
            break;
        default:
            //This function is only used in the case of expressions so other cases are not handled
            break;
    }
    if (node->childrenCount > 0) {
        copy->children = malloc(sizeof(astNode *) * node->childrenCount);
        for (int i = 0; i < node->childrenCount; i++) {
            copy->children[i] = copyAstNode(node->children[i]);
        }
    } else {
        copy->children = NULL;
    }
    return copy;
}
