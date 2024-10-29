//
// Created by user on 10/6/24.
//

#ifndef LANGAJE_ERRORS_H
#define LANGAJE_ERRORS_H

typedef enum {
    ERR_SUCCESS,
    ERR_SYNTAX,
    ERR_END_OF_INPUT,
    ERR_TYPE,
    ERR_OUT_OF_BOUNDS,
    ERR_NOT_FOUND,
    ERR_UNKNOWN_OPERATOR,
    ERR_ALREADY_EXISTS,
    ERR_NULL_POINTER,
} errorValue;

typedef struct {
    errorValue value;
    char *message;
} error;

void *endOfInputError(error *err);
int assignErrorMessage(error *err, char *msg);
char* getNameTypeError(errorValue error);

#endif //LANGAJE_ERRORS_H
