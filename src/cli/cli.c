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

#define MAX_INPUT 1024
#define BASE_MEMORY_STACK_SIZE 16

typedef enum {
    PARENTHESES,
    BRACKETS,
    CURLY_BRACKETS
} BracketType;

void enableRawMode(struct termios *orig_termios) {
    struct termios raw = *orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

void printPrompt(const char *input, int cursor_pos) {
    printf("\r\033[K>>> %s", input); // Clear the line before printing
    printf("\033[%dG", cursor_pos + 5); // Move cursor to the correct position
    fflush(stdout);
}

int runCode(char *input, Lexer *l, hm* functionMap, hmStack* stack) {
    // Tokenize the input
    TokenList *tl = tokenizer(input, l);
    if (tl == NULL) return 1;

    error err;
    err.value = ERR_SUCCESS;
    InstructionBlock *pr = parse(tl, &err);
    if (err.value != ERR_SUCCESS) {
        printf("[PARSER][ERROR]: %s\n", getNameTypeError(err.value));
        printf("[PARSER][ERROR]: %s\n", err.message);
        return 1;
    }


    if (runInstructionBlock(pr, stack, functionMap, &err)) {
        printf("[RUNTIME][ERROR]: %s\n", err.message);
        return 1;
    }

    //free_tokenList(tl);

    return 0;
}

int cliMode(Lexer *l) {

    char langFile[] = "lang/CLASSIC.lang";

    // We read the lang file
    if (readLexerFile(l, langFile) != 0) {printf("[ERROR][LEXER]: Error while lexing"); return 1;}

    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    enableRawMode(&orig_termios);

    char input[MAX_INPUT] = {0};
    char final_input[MAX_INPUT] = {0};
    int cursor_pos = 0;
    int len = 0;
    
    printf(">>> ");

    hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);
    hm* hashmap = hm_create();
    hm* functionMap = hm_create();
    hmStackPush(stack, hashmap);

    int nb_brackets[3] = {0};

    while (1) {
        char c = getchar();
        if (c == 27) { // Escape sequence
            getchar(); // Skip the '['
            switch (getchar()) {
                case 'C': // Right arrow
                    if (cursor_pos < len) cursor_pos++;
                    break;
                case 'D': // Left arrow
                    if (cursor_pos > 0) cursor_pos--;
                    break;
                case 'A': // Up arrow
                case 'B': // Down arrow
                    break;
            }
        } else if (c == 10) { // Enter key
            if (len == 0) {
                printf("\n>>> ");
                continue;
            }

            // foreach char, we count the number of brackets
            for (int i = 0; i < len; i++) {
                if (input[i] == '(') nb_brackets[PARENTHESES]++;
                if (input[i] == ')') nb_brackets[PARENTHESES]--;
                if (input[i] == '[') nb_brackets[BRACKETS]++;
                if (input[i] == ']') nb_brackets[BRACKETS]--;
                if (input[i] == '{') nb_brackets[CURLY_BRACKETS]++;
                if (input[i] == '}') nb_brackets[CURLY_BRACKETS]--;
            }

            // Check if there are any unclosed brackets
            int unclosed_brackets = 0;
            for (int i = 0; i < 3; i++) {
                unclosed_brackets += nb_brackets[i];
            }

            if (unclosed_brackets > 0) {
                strcat(final_input, input);
                strcat(final_input, "\n");
                input[0] = 0;
                len = 0;
                cursor_pos = 0;
                for (int i = 0; i < MAX_INPUT; i++) {
                    input[i] = 0;
                }
                printf("\n>>> ");
                continue;
            } else {
                strcat(final_input, input);
                strcpy(input, final_input);
                memset(final_input, 0, sizeof(final_input));
                len = strlen(input);
            }


            input[len] = '\0';
            if (strcmp(input, "@exit") == 0) break;
            if (runCode(input, l, functionMap, stack)) { // If an error occured
                printf(">>> ");
                continue;
            }
            len = 0;
            cursor_pos = 0;
            for (int i = 0; i < MAX_INPUT; i++) {
                input[i] = 0;
            }
            printf(">>> ");
        } else if (c == 127) { // Backspace key
            if (cursor_pos > 0) {
                for (int i = cursor_pos - 1; i < len - 1; i++) {
                    input[i] = input[i + 1];
                }
                input[--len] = '\0';
                cursor_pos--;
                printPrompt(input, cursor_pos);
                continue;
            }
        } else if (c == 126) { // Delete key
            if (cursor_pos < len) {
                for (int i = cursor_pos; i < len - 1; i++) {
                    input[i] = input[i + 1];
                }
                input[--len] = '\0';
                printPrompt(input, cursor_pos);
                continue;
            }
        } else {
            for (int i = len; i > cursor_pos; i--) {
                input[i] = input[i - 1];
            }
            input[cursor_pos++] = c;
            len++;
        }
        printPrompt(input, cursor_pos);
    }

    disableRawMode(&orig_termios);

    error err;
    err.value = ERR_SUCCESS;
    displayHashmap(stack, &err);

    hmStackPop(stack);
    hm_functions_free(functionMap);
    hmStackDestroy(stack);
    hm_functions_free(functionMap);


    return 0;

}