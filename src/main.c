#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "functions/functions.h"

#include "interpreter/interpreter.h"

#define BASE_MEMORY_STACK_SIZE 16

int main() {

    for (int i = 0; i < 30; i++) printf("=");
    printf("\n");

    /*---------- LEXER ----------*/
    Lexer *l = new_lexer();
    if (l == NULL) {printf("[ERROR][LEXER]: Error while creating lexer"); return 1;}
    
    if (readLexerFile(l, "lang/CLASSIC.lang") != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}
    //if (readLexerFile(l, "lang/FR.lang") != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}
    //if (readLexerFile(l, "lang/MEOW.lang") != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}

    //print_lexer(l);

    char *input = read_file("test.txt");
    //char *input = read_file("fr.txt");
    //char *input = read_file("meow.txt");

    TokenList *tl = tokenizer(input, l);
    if (tl == NULL) return 1;

    print_tokenList(tl);

    error err;
    err.value = ERR_SUCCESS;
    InstructionBlock *pr = parse(tl, &err);
    if (err.value != ERR_SUCCESS) {
        printf("Error: %s\n", err.message);
        return 1;
    }
    printInstructionBlock(pr, 0);


    hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);

    runInstructionBlock(pr,stack);

    hmStackDestroy(stack);
    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}