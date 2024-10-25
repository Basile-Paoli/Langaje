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

    Lexer *l = new_lexer();
    if (l == NULL)return 1;

    //readLexerFile(l, "CLASSIC.lang");
    readLexerFile(l, "CUSTOM.lang");

    add_lexer_rule(l, new_lexer_rule("#LANG_([A-Z])+", TOKEN_PREPROCESSEUR_LANG));
    add_lexer_rule(l, new_lexer_rule("#include", TOKEN_PREPROCESSEUR_INCLUDE));
    add_lexer_rule(l, new_lexer_rule("[0-9]+\\.[0-9]+", TOKEN_FLOAT));
    add_lexer_rule(l, new_lexer_rule("[0-9]+", TOKEN_INT));
    add_lexer_rule(l, new_lexer_rule("\"[^\"]*\"", TOKEN_STRING));

    add_lexer_rule(l, new_lexer_rule("[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_IDENTIFIER));

    //print_lexer(l);

    char *input = read_file("test.txt");

    TokenList *tl = tokenizer(input, l);
    if (tl == NULL) return 1;

    print_tokenList(tl);

    /*error err;
    err.value = ERR_SUCCESS;
    InstructionBlock *pr = parse(tl, &err);
    if (err.value != ERR_SUCCESS) {
        printf("Error: %s\n", err.message);
        return 1;
    }
    printInstructionBlock(pr, 0);
    */

    // hmStack* stack = hmStackCreate(BASE_MEMORY_STACK_SIZE);

//    runInstructionBlock(pr,stack);

    // hmStackDestroy(stack);
    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}