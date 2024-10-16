#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../ast/ast.h"
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../functions/functions.h"
#include <stdlib.h>

int run(InstructionBlock* program);

#endif