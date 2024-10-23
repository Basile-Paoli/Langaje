#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "functions/functions.h"

int main() {

    for (int i = 0; i < 30; i++) printf("=");
    printf("\n");

    Lexer *l = new_lexer();
    if (l == NULL)return 1;


    add_lexer_rule(l, new_lexer_rule("def", TOKEN_KEYWORD));
    add_lexer_rule(l, new_lexer_rule("int", TOKEN_KEYWORD));
    add_lexer_rule(l, new_lexer_rule("float", TOKEN_KEYWORD));

    add_lexer_rule(l, new_lexer_rule("if", TOKEN_KEYWORD_IF));
    add_lexer_rule(l, new_lexer_rule("else", TOKEN_KEYWORD_ELSE));
    add_lexer_rule(l, new_lexer_rule("while", TOKEN_KEYWORD_WHILE));
    add_lexer_rule(l, new_lexer_rule("for", TOKEN_KEYWORD_FOR));

    add_lexer_rule(l, new_lexer_rule("#LANG_([A-Z]){1,}", TOKEN_PREPROCESSEUR_LANG));
    add_lexer_rule(l, new_lexer_rule("#include", TOKEN_PREPROCESSEUR_INCLUDE));
    
    add_lexer_rule(l, new_lexer_rule("func", TOKEN_FUNCTION_DECLARATION));

    add_lexer_rule(l, new_lexer_rule("==", TOKEN_EQUAL_EQUAL));
    add_lexer_rule(l, new_lexer_rule("!=", TOKEN_NOT_EQUAL));
    add_lexer_rule(l, new_lexer_rule("<=", TOKEN_LESS_EQUAL));
    add_lexer_rule(l, new_lexer_rule(">=", TOKEN_GREATER_EQUAL));
    add_lexer_rule(l, new_lexer_rule("<", TOKEN_LESS));
    add_lexer_rule(l, new_lexer_rule(">", TOKEN_GREATER));

    add_lexer_rule(l, new_lexer_rule("&&", TOKEN_AND));
    add_lexer_rule(l, new_lexer_rule("\\|\\|", TOKEN_OR));
    add_lexer_rule(l, new_lexer_rule("!|not", TOKEN_NOT));

    add_lexer_rule(l, new_lexer_rule("=", TOKEN_EQUAL));

    add_lexer_rule(l, new_lexer_rule("[0-9]+\\.[0-9]+", TOKEN_FLOAT));
    add_lexer_rule(l, new_lexer_rule("[0-9]+", TOKEN_NUMBER));
    add_lexer_rule(l, new_lexer_rule("\"[^\"]*\"", TOKEN_STRING));

    add_lexer_rule(l, new_lexer_rule("\\+", TOKEN_ADDITION));
    add_lexer_rule(l, new_lexer_rule("-", TOKEN_SUBTRACTION));
    add_lexer_rule(l, new_lexer_rule("\\*", TOKEN_MULTIPLICATION));
    add_lexer_rule(l, new_lexer_rule("/", TOKEN_DIVISION));

    add_lexer_rule(l, new_lexer_rule("\\(", TOKEN_LPAREN));
    add_lexer_rule(l, new_lexer_rule("\\)", TOKEN_RPAREN));

    add_lexer_rule(l, new_lexer_rule("\\{", TOKEN_LBRACE));
    add_lexer_rule(l, new_lexer_rule("\\}", TOKEN_RBRACE));

    add_lexer_rule(l, new_lexer_rule("\\[", TOKEN_LBRACKET));
    add_lexer_rule(l, new_lexer_rule("\\]", TOKEN_RBRACKET));

    add_lexer_rule(l, new_lexer_rule(",", TOKEN_COMMA));
    add_lexer_rule(l, new_lexer_rule(";", TOKEN_SEMICOLON));

    add_lexer_rule(l, new_lexer_rule("[a-zA-Z_][a-zA-Z0-9_]{0,}", TOKEN_IDENTIFIER));

    // print_lexer(l);

    char *input = read_file("test.txt");

    TokenList *tl = tokenizer(input, l);

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

    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}