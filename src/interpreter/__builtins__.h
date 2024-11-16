#ifndef __BUILTINS___H
#define __BUILTINS___H
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../interpreter/interpreter.h"

#include <stdlib.h>
#include <stdio.h>

void __builtinToMap__(hm* functionStack, error* err);
void display__print(var* value, error* err, int level);

void call__print__(hmStack* fStack, error* err);
void call__strlen__(hmStack* fStack, error* err);
void call__arrlen__(hmStack* fStack, error* err);
void call__randint__(hmStack* fStack, error* err);
void call__randfloat__(hmStack* fStack, error* err);
void call__system__(hmStack* fStack, error* err);
void call__input__(hmStack* fStack, error* err);
void call__randchoice__(hmStack* fStack, error* err);
void call__fread__(hmStack* fStack, error* err);
void call__fwrite__(hmStack* fStack, error* err);
void call__split__(hmStack* fStack, error* err);
void call__range__(hmStack* fStack, error* err);
void call__append__(hmStack* fStack, error* err);
void call__pop__(hmStack* fStack, error* err);

#endif