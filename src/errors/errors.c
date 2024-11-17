//
// Created by user on 10/6/24.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

void *endOfInputError(error *err) {
    err->value = ERR_END_OF_INPUT;
    assignErrorMessage(err, "The input has ended unexpectedly");
    return NULL;
}

/*
 *
 * ASSIGN MESSAGE TO ERROR
 * Specify before the type of error (ERR_SYNTAX...)
 * assignErrorMessage(err, "enter message");
 *
 * */
int assignErrorMessage(error *err, char *msg) {
    //err->message = malloc(sizeof(char) * (strlen(getNameTypeError(err->value)) + strlen(msg) + 50));

    err->message = (char *)realloc(err->message, strlen(getNameTypeError(err->value)) + strlen(msg) + strlen(err->message) + 1);
    //strcpy(err->message,"");
    strcat(err->message, getNameTypeError(err->value));
    strcat(err->message, msg);
    strcat(err->message, "\n");

    return 0;
}

void printError(error *err) {
    printf("\x1B[31m%s\n", err->message);
    printf("\x1B[0m");
    free(err->message);
    free(err);
}

/*
 * Get the name of the error with the index of enum
 */
char *getNameTypeError(errorValue error) {
    switch (error) {
        case ERR_SYNTAX:
            return "Syntax error: ";
        case ERR_END_OF_INPUT:
            return "Unexpected end of input: ";
        case ERR_TYPE:
            return "Type mismatch: ";
        case ERR_OUT_OF_BOUNDS:
            return "Index out of bounds: ";
        case ERR_NOT_FOUND:
            return "Item not found: ";
        case ERR_UNKNOWN_OPERATOR:
            return "Unknown operator: ";
        case ERR_ALREADY_EXISTS:
            return "Already exists: ";
        case ERR_NULL_POINTER:
            return "Pointer error: ";
        case ERR_MEMORY:
            return "Memory error: ";
        case ERR_FILE:
            return "File error: ";
        case ERR_ARGS:
            return "Arguments error: ";
        default:
            return "Unknown error: ";
    }
}