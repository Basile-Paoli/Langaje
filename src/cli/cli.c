#include "cli.h"

// Constants for bracket types
#define PARENTHESES 0
#define BRACKETS 1
#define CURLY_BRACKETS 2

// Enable raw terminal mode
void enableRawMode(struct termios *orig_termios) {
    struct termios raw = *orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Restore the terminal to its original state
void disableRawMode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

void printPrompt(const char *input, int cursor_pos) {
    printf("\r\033[K");  // Clear the current line

    // Display '...' prompt if brackets are unbalanced
    if (countUnbalancedBrackets(input) > 0) {
        printf("... ");
    } else {
        printf(">>> ");  // Default prompt
    }

    printf("%s", input);  // Print the input
    // Move cursor to the correct position (add 5 to the prompt length)
    printf("\033[%dG", cursor_pos + 5);
    fflush(stdout);
}

// Tokenize and execute the input code
int executeCode(char *input, Lexer *l, hm* functionMap, hmStack* stack, error *err) {
    // Tokenize the input
    TokenList *tokens = tokenizer(input, l, err);
    if (!tokens) {
        assignErrorMessage(err, "Tokenization failed");
        return 1;
    }

    // Parse the tokens into an instruction block
    InstructionBlock *instructions = parse(tokens, err);
    if (err->value != ERR_SUCCESS) {
        free_tokenList(tokens);
        assignErrorMessage(err, "Parsing failed");
        return 1;
    }

    // Execute the instructions
    if (runInstructionBlock(instructions, stack, functionMap, l, err)) {
        free_tokenList(tokens);
        assignErrorMessage(err, "Execution failed");
        return 1;
    }

    free_tokenList(tokens);
    return 0;
}

// Count unbalanced brackets in the input string
int countUnbalancedBrackets(const char *input) {
    int counts[3] = {0}; // PARENTHESES, BRACKETS, CURLY_BRACKETS
    for (int i = 0; input[i] != '\0'; i++) {
        switch (input[i]) {
            case '(': counts[PARENTHESES]++; break;
            case ')': counts[PARENTHESES]--; break;
            case '[': counts[BRACKETS]++; break;
            case ']': counts[BRACKETS]--; break;
            case '{': counts[CURLY_BRACKETS]++; break;
            case '}': counts[CURLY_BRACKETS]--; break;
        }
    }
    return counts[PARENTHESES] + counts[BRACKETS] + counts[CURLY_BRACKETS];
}

// Check if the input is "@exit" and exit the program if true
int checkExitCondition(const char *input) {
    return strcmp(input, "@exit") == 0;
}

// Main interactive CLI loop
int cliMode(Lexer *l, hmStack *stack, hm* functionMap, error *err) {
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    enableRawMode(&orig_termios);

    char input[MAX_INPUT] = {0};
    char final_input[MAX_INPUT] = {0};
    int cursor_pos = 0;
    int len = 0;

    // Track whether to display '...' or '>>>'
    int isParenOpen = 0;

    printf(">>> ");  // Initial prompt

    int running = 1;
    while (running) {
        char c = getchar();
        switch (c) {
            case 27:  // Escape sequence (arrow keys)
                handleArrowKeys(&cursor_pos, len);
                break;
            case 10:  // Enter key
                if (handleEnterKey(input, final_input, &len, cursor_pos)) {
                    continue;  // Skip to the next iteration if still gathering input
                }

                // Check for "@exit" and break the loop if found
                if (checkExitCondition(input)) {
                    running = 0;
                    break;  // Exit the loop and program
                }

                // Execute the code if the input is not "@exit"
                if (executeCode(input, l, functionMap, stack, err)) {
                    return 1;  // Error occurred during code execution
                }

                // Reset the input buffer
                resetInputBuffer(input, &len, &cursor_pos);

                // Reset prompt condition after processing
                isParenOpen = 0; // Reset after input is processed
                break;
            case 127:  // Backspace key
                handleBackspace(input, &len, &cursor_pos);
                break;
            case 126:  // Delete key
                handleDelete(input, &len, &cursor_pos);
                break;
            default:
                // Handle regular character input
                handleCharacterInput(input, &len, &cursor_pos, c);
        }

        if (!running) break;  // Exit the loop if running is false

        // Print the appropriate prompt based on whether "(" was entered or brackets are unbalanced
        printPrompt(input, cursor_pos);

    }

    disableRawMode(&orig_termios);
    return 0;
}

void handleArrowKeys(int *cursor_pos, int len) {
    char direction[3]; // Store the full escape sequence
    direction[0] = getchar();  // First byte (Escape, 27)
    direction[1] = getchar();  // Second byte ([)
    direction[2] = getchar();  // Third byte (Arrow code: 'C' for right, 'D' for left)
    
    if (direction[0] == 27 && direction[1] == 91) {
        if (direction[2] == 'C' && *cursor_pos < len) {
            (*cursor_pos)++;  // Right arrow
        } else if (direction[2] == 'D' && *cursor_pos > 0) {
            (*cursor_pos)--;  // Left arrow
        }
    }
}


int handleEnterKey(char *input, char *final_input, int *len, int cursor_pos) {
    // Count unbalanced brackets
    if (countUnbalancedBrackets(input) > 0) {
        //strcat(final_input, input);
        strcat(final_input, "\n");
        printf("\n... ");
        return 1;  // Continue gathering input if brackets are unbalanced
    }

    if (strlen(input) == 0 || strspn(input, " \t\n") == strlen(input)) {
        printf("\n>>> ");
        return 1;  // Skip processing if input is empty or only whitespace
    }

    // Otherwise, process the complete input
    strcat(final_input, input);
    strcpy(input, final_input);
    memset(final_input, 0, sizeof(final_input));
    *len = strlen(input);
    return 0;
}

// Reset the input buffer after each input processing
void resetInputBuffer(char *input, int *len, int *cursor_pos) {
    *len = 0;
    *cursor_pos = 0;
    memset(input, 0, MAX_INPUT);
    printf("\n>>> ");  // Prompt for new input
}

// Handle backspace key input
void handleBackspace(char *input, int *len, int *cursor_pos) {
    if (*cursor_pos > 0) {
        for (int i = *cursor_pos - 1; i < *len - 1; i++) {
            input[i] = input[i + 1];
        }
        input[--(*len)] = '\0';
        (*cursor_pos)--;
    }
}

// Handle delete key input
void handleDelete(char *input, int *len, int *cursor_pos) {
    if (*cursor_pos < *len) {
        for (int i = *cursor_pos; i < *len - 1; i++) {
            input[i] = input[i + 1];
        }
        input[--(*len)] = '\0';
    }
}

// Handle character input (add character at cursor position)
void handleCharacterInput(char *input, int *len, int *cursor_pos, char c) {
    for (int i = *len; i > *cursor_pos; i--) {
        input[i] = input[i - 1];
    }
    input[*cursor_pos] = c;
    (*len)++;
    (*cursor_pos)++;
}
