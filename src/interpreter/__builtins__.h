#ifndef __BUILTINS___H
#define __BUILTINS___H
#include "../types/types.h"
#include "../hashtables/hashtables.h"
#include "../errors/errors.h"
#include "../interpreter/interpreter.h"

#include <stdlib.h>
#include <stdio.h>

void __builtinToMap__(hm* functionStack, error* err);

function* declare__print(error* err);
void call__print(hmStack* fStack, error* err);

function* declare__strlen(error* err);
void call__strlen(hmStack* fStack, error* err);

function* declare__arrlen(error* err);
void call__arrlen(hmStack* fStack, error* err);

function* declare__randint(error* err);
void call__randint(hmStack* fStack, error* err);

function* declare__randfloat(error* err);
void call__randfloat(hmStack* fStack, error* err);

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

#endif