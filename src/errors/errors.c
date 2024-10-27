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
    err->message = malloc(strlen(msg) + 1);
    strcpy(err->message, msg);

    return 0;
}

