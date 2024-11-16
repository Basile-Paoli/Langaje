#ifndef LANGAJE_CLI_H
#define LANGAJE_CLi_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "../lexer/token.h"
#include "../lexer/lexer.h"

#include "../parser/parser.h"
#include "../functions/functions.h"

#include "../interpreter/interpreter.h"

#include "../errors/errors.h"

#define MAX_INPUT 1024
#define BASE_MEMORY_STACK_SIZE 16

typedef enum {
    PARENTHESES,
    BRACKETS,
    CURLY_BRACKETS
} BracketType;

int cliMode(Lexer *l, error *err);

#endif //LANGAJE_ERRORS_H
