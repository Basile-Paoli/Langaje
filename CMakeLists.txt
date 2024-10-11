cmake_minimum_required(VERSION 3.26)
project(Langaje C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Langaje
        src/main.c
        src/functions/functions.c
        src/functions/functions.h
        src/types/types.c
        src/types/types.h
        src/errors/errors.h
        src/ast/ast.c
        src/ast/ast.h
        src/lexer/lexer.c
        src/lexer/lexer.h
        src/lexer/token.c
        src/lexer/token.h
        src/structures/dynamic_arrays.c
        src/structures/dynamic_arrays.h)
