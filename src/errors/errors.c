//
// Created by user on 10/6/24.
//

#include <string.h>
#include "errors.h"

void *endOfInstructionError(error *err) {
    err->value = ERR_SYNTAX;
    err->message = strdup("End of instruction expected");
    return NULL;
}
