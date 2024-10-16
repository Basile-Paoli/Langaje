//
// Created by user on 10/6/24.
//

#include <string.h>
#include "errors.h"

void *endOfInputError(error *err) {
    err->value = ERR_END_OF_INPUT;
    err->message = strdup("Unexpected end of input");
    return NULL;
}
