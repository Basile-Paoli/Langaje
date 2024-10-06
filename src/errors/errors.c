//
// Created by user on 10/6/24.
//

#include <stdio.h>
#include "errors.h"

void *endOfInstructionError(error *err) {
    err->value = ERR_SYNTAX;
    sprintf(err->message, "Unexpected end of instruction");
    return NULL;
}
