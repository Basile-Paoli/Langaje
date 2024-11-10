#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "functions/functions.h"

#include "interpreter/interpreter.h"


#include <time.h>


#define BASE_MEMORY_STACK_SIZE 16

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    for (int i = 0; i < 30; i++) printf("=");
    printf("\n");

    // Reading the input file
    char *input = read_file(argv[1]);
    
    /*---------- LEXER ----------*/
    Lexer *l = new_lexer();
    if (l == NULL) {printf("[ERROR][LEXER]: Error while creating lexer"); return 1;}

    char *lang = get_lang(input); // Get the #LANG_
    if (lang == NULL) lang = "CLASSIC"; // If none is found, we use the default one

    char *langFile = (char *)calloc(strlen(lang) + 6, sizeof(char));
    if (langFile == NULL) {printf("[ERROR][LEXER]: Error while creating langFile"); return 1;}
    sprintf(langFile, "lang/%s.lang", lang);

    // We read the lang file
    if (readLexerFile(l, langFile) != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}

    //print_lexer(l);

    // Tokenization
    TokenList *tl = tokenizer(input, l);
    if (tl == NULL) return 1;

    // Replace sugar syntax
    tl = replaceSugar(tl, l);
    if (tl == NULL) return 1;

    print_tokenList(tl); // Print the token list
    return 0;
    /*---------- PARSER ----------*/
    error err;
    err.value = ERR_SUCCESS;
    InstructionBlock *pr = parse(tl, &err);
    if (err.value != ERR_SUCCESS) {
        printf("Error: %s\n", err.message);
        return 1;
    }
    printInstructionBlock(pr, 0);

    hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);

    error err_run;
    err_run.value = ERR_SUCCESS;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //Push base hm
    hm* hashmap = hm_create();
    hmStackPush(stack,hashmap);
    int runInstructionResult = runInstructionBlock(pr, stack, &err_run);
    hmStackPop(stack);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken to execute : %f seconds\n", cpu_time_used);
    if(runInstructionResult == 1){
        // Print the error msg
        printf("%s\n", err_run.message);
        return 1;
    }

    hmStackDestroy(stack);
    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}