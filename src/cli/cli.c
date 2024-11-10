#include "../lexer/token.h"
#include "../lexer/lexer.h"

#include "../parser/parser.h"
#include "../functions/functions.h"

#include "../interpreter/interpreter.h"


#define BASE_MEMORY_STACK_SIZE 16

int cliMode(Lexer *l) {

    char langFile[] = "lang/CLASSIC.lang";

    // We read the lang file
    if (readLexerFile(l, langFile) != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}

    // In cli mode
    printf("Welcome to the CLI mode of the language\n");
    printf("Type '@exit' to stop the program\n");
    printf(">>>");

    char *input = (char *)calloc(1, sizeof(char));
    if (input == NULL) {printf("[ERROR][MAIN]: Error while creating input"); return 1;}

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);
    hm* hashmap = hm_create();
    hmStackPush(stack, hashmap);

    int nbParenthesis = 0;
    int nbBracket = 0;
    int nbCurlyBracket = 0;

    char *curLine = (char *)calloc(1, sizeof(char));

    while ((read = getline(&line, &len, stdin)) != -1) {

        if (strcmp(line, "@exit\n") == 0) break; // Break the loop

        curLine = (char *)realloc(curLine, (strlen(curLine) + strlen(line) + 1) * sizeof(char));
        if (curLine == NULL) {printf("[ERROR][MAIN]: Error while reallocating curLine"); return 1;}
        strcat(curLine, line);

        // If the user types a line with a parenthesis, a bracket or a curly bracket, we increment the corresponding variable
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '(') nbParenthesis++;
            if (line[i] == ')') nbParenthesis--;
            if (line[i] == '[') nbBracket++;
            if (line[i] == ']') nbBracket--;
            if (line[i] == '{') nbCurlyBracket++;
            if (line[i] == '}') nbCurlyBracket--;
        }

        if (nbParenthesis != 0 || nbBracket != 0 || nbCurlyBracket != 0) {
            printf("...");
            for (int i = 0; i < nbParenthesis + nbBracket + nbCurlyBracket; i++) printf("\t");
            continue;
        }

        if (nbParenthesis == 0 && nbBracket == 0 && nbCurlyBracket == 0) {

            do {
                curLine = include_files(curLine);
            } while (strstr(curLine, "#include") != NULL);

            // Tokenize the input
            TokenList *tl = tokenizer(curLine, l);
            if (tl == NULL) return 1;

            tl = replaceSugar(tl, l);
            if (tl == NULL) return 1;

            error err;
            err.value = ERR_SUCCESS;
            InstructionBlock *pr = parse(tl, &err);
            if (err.value == ERR_END_OF_INPUT) {
                printf("...");
                continue;
            }

            if (err.value != ERR_SUCCESS) {
                printf("[PARSER][ERROR]: %s\n", getNameTypeError(err.value));
                printf("[PARSER][ERROR]: %s\n", err.message);
                return 1;
            }


            if (runInstructionBlock(pr, stack, &err)) {
                printf("[RUNTIME][ERROR]: %s\n", err.message);
                return 1;
            }

            free_tokenList(tl);

            curLine = (char *)calloc(1, sizeof(char));
            printf(">>>");
        }
    }

    error err;
    err.value = ERR_SUCCESS;
    displayHashmap(stack, &err);

    hmStackPop(stack);
    hmStackDestroy(stack);

    return 0;

}