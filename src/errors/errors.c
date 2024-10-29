//
// Created by user on 10/6/24.
//

#include <string.h>
#include <stdlib.h>
#include "errors.h"

void *endOfInputError(error *err) {
    err->value = ERR_END_OF_INPUT;
    err->message = strdup("Unexpected end of input");
    return NULL;
}

/*
 *
 * ASSIGN MESSAGE TO ERROR
 * Specify before the type of error (ERR_SYNTAX...)
 * assignErrorMessage(err, "enter message");
 *
 * */
int assignErrorMessage(error *err, char *msg){
    err->message = malloc(strlen(getNameTypeError(err->value)) + strlen(msg) + 1);
    strcpy(err->message, getNameTypeError(err->value));
    strcat(err->message, msg);

    return 0;
}

/*
 * Get the name of the error with the index of enum
 */
char* getNameTypeError(int errorNum){
    switch (errorNum) {
        case 1:
            return "Syntax error: ";
        case 2:
            return "Unexpected end of input: ";
        case 3:
            return "Type mismatch: ";
        case 4:
            return "Index out of bounds: ";
        case 5:
            return "Item not found: ";
        case 6:
            return "Unknown operator: ";
        case 7:
            return "Already exists: ";
        case 8:
            return "Pointer error: ";
        default:
            return "Unknown error: ";
    }
}