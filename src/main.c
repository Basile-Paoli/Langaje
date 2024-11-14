#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "functions/functions.h"

#include "interpreter/interpreter.h"
#include "interpreter/__builtins__.h"

#include "cli/cli.h"

#include <time.h>


#define BASE_MEMORY_STACK_SIZE 16

int main(int argc, char **argv) {

    error *err = (error *)malloc(sizeof(error));
    if (err == NULL) {
        err->value = ERR_MEMORY;
        err->message = strdup("Cannot allocate memory for error");
        assignErrorMessage(err, "Error while creating error");
        printError(err);
        return 1;
    }
    err->value = ERR_SUCCESS;
    err->message = strdup("");

    /*---------- LEXER ----------*/
    Lexer *l = new_lexer(err);
    if (l == NULL) {
        printError(err);
        return 1;
    }

    if (argc == 1) return cliMode(l);

    if (argc > 2) {
        err->value = ERR_ARGS;
        assignErrorMessage(err, "Too many arguments. Usage: ./langaje.out <file>.gol");
        return 1;
    }

    for (int i = 0; i < 30; i++) printf("=");
    printf("\n");

    // Reading the input file
    char *mainFile = read_file(argv[1], err);
    if (mainFile == NULL) {
        free_lexer(l); free(mainFile);
        assignErrorMessage(err, "Cannot read file");
        printError(err);
        return 1;
    }

    char* input = include_files(mainFile, err);
    if (input == NULL) {
        free_lexer(l); free(input); free(mainFile);
        assignErrorMessage(err, "Cannot include files");
        printError(err);
        return 1;
    }
    free(mainFile);

    
    /*---------- LEXER ----------*/

    char *lang = get_lang(input, err); // Get the #LANG_
    if (lang == NULL) lang = "CLASSIC"; // If none is found, we use the default one
    if (err->value != ERR_SUCCESS) {
        assignErrorMessage(err, "Cannot get lang");
        printError(err);
        return 1;
    }

    char *langFile = (char *)calloc(strlen(lang) + 11, sizeof(char));
    if (langFile == NULL) {
        free_lexer(l); free(input); free(langFile);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for langFile");
        printError(err);
        return 1;
    }
    sprintf(langFile, "lang/%s.lang", lang);

    // We read the lang file
    if (readLexerFile(l, langFile, err) != 0) {
        free_lexer(l); free(input); free(langFile);
        assignErrorMessage(err, "Cannot read lang file");
        printError(err);
        return 1;
    }

    //print_lexer(l);

    // Tokenization
    TokenList *tl = tokenizer(input, l, err);
    if (tl == NULL) {
        free_lexer(l); free(input); free(langFile);
        assignErrorMessage(err, "Cannot tokenize");
        printError(err);
        return 1;
    }
    free(input); free(langFile); free_lexer(l);

    // print_tokenList(tl); // Print the token list


    /*---------- PARSER ----------*/
    InstructionBlock *pr = parse(tl, err);
    if (err->value != ERR_SUCCESS) {
        printf("[PARSER][ERROR] %s\n", err->message);
        return 1;
    }
    //printInstructionBlock(pr, 0);
    

    hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);

    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //Push base hm
    hm* hashmap = hm_create();
    hmStackPush(stack,hashmap);

    hm* functionMap = hm_create();
    
    //declare builtins : 
    __builtinToMap__(functionMap,err);

    // init rand
    srand(time(NULL));

    int runInstructionResult = runInstructionBlock(pr, stack, functionMap, err);
    //displayHashmap(stack, err);
    hmStackPop(stack);
    hm_functions_free(functionMap);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken to execute : %f seconds\n", cpu_time_used);
    if(runInstructionResult == 1){
        // Print the error msg
        printf("[RUNTIME][ERROR] %s\n", err->message);
        return 1;
    }


    hmStackDestroy(stack);
    free_tokenList(tl);
    printError(err);

    return 0;
}