#ifndef __BUILTINS___H
#define __BUILTINS___H
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../interpreter/interpreter.h"

#include <stdlib.h>
#include <stdio.h>

void __builtinToMap__(hm* functionStack, error* err);

function* declare__print__(error* err);
void call__print__(hmStack* fStack, error* err);

function* declare__strlen__(error* err);
void call__strlen__(hmStack* fStack, error* err);

function* declare__arrlen__(error* err);
void call__arrlen__(hmStack* fStack, error* err);

function* declare__randint__(error* err);
void call__randint__(hmStack* fStack, error* err);

function* declare__randfloat__(error* err);
void call__randfloat__(hmStack* fStack, error* err);

function* declare__system__(error* err);
void call__system__(hmStack* fStack, error* err);

function* declare__input__(error* err);
void call__input__(hmStack* fStack, error* err);

function* declare__randchoice__(error* err);
void call__randchoice__(hmStack* fStack, error* err);

function* declare__fread__(error* err);
void call__fread__(hmStack* fStack, error* err);

function* declare__fwrite__(error* err);
void call__fwrite__(hmStack* fStack, error* err);

function* declare__split__(error* err);
void call__split__(hmStack* fStack, error* err);

function* declare__range__(error* err);
void call__range__(hmStack* fStack, error* err);

#endif