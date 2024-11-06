#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "functions/functions.h"

#include "interpreter/interpreter.h"


#include <time.h>


#define BASE_MEMORY_STACK_SIZE 16

int main() {

    for (int i = 0; i < 30; i++) printf("=");
    printf("\n");

    /*---------- LEXER ----------*/
    Lexer *l = new_lexer();
    if (l == NULL)return 1;

    add_lexer_rule(l, new_lexer_rule("#LANG_([A-Z])+", TOKEN_PREPROCESSEUR_LANG));
    add_lexer_rule(l, new_lexer_rule("#include", TOKEN_PREPROCESSEUR_INCLUDE));
    add_lexer_rule(l, new_lexer_rule("[0-9]+\\.[0-9]+", TOKEN_FLOAT));
    add_lexer_rule(l, new_lexer_rule("[0-9]+", TOKEN_INT));
    add_lexer_rule(l, new_lexer_rule("\"[^\"]*\"", TOKEN_STRING));

    if (readLexerFile(l, "lang/CLASSIC.lang") != 0) {
        printf("[ERROR][LEXER]: Error while lexing");
        return 1;
    }
    //if (readLexerFile(l, "lang/FR.lang") != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}
    //if (readLexerFile(l, "lang/MEOW.lang") != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}

    add_lexer_rule(l, new_lexer_rule("[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_IDENTIFIER));

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

//    hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);
//
//
//    error err_run;
//    err_run.value = ERR_SUCCESS;
//    clock_t start, end;
//    double cpu_time_used;
//    start = clock();
//    int runInstructionResult = runInstructionBlock(pr, stack, &err_run);
//    end = clock();
//
//    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
//
//    printf("Time taken to execute : %f seconds\n", cpu_time_used);
//    if(runInstructionResult == 1){
//        // Print the error msg
//        printf("%s\n", err_run.message);
//        return 1;
//    }
//    printf("RESULT INSRCUTION BLOCK : %d", runInstructionResult);
//
//    hmStackDestroy(stack);
    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}