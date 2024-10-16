#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../ast/ast.h"
#include "../types/types.h"
#include "../hashtables/hashtables.h"

int run(InstructionBlock* instructions);

#endif