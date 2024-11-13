#ifndef __BUILTINS___H
#define __BUILTINS___H
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"


void __builtinToMap__(hm* functionStack, error* err);

function* declare__example(error* err);


#endif