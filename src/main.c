#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"

int main() {

    Lexer *l = new_lexer();
    if (l == NULL)return 1;

    add_lexer_rule(l, new_lexer_rule(l, "def", TOKEN_KEYWORD));
    add_lexer_rule(l, new_lexer_rule(l, "int", TOKEN_KEYWORD));
    add_lexer_rule(l, new_lexer_rule(l, "[a-zA-Z_][a-zA-Z0-9_]{0,}", TOKEN_IDENTIFIER));

    add_lexer_rule(l, new_lexer_rule(l, "=", TOKEN_EQUAL));

    add_lexer_rule(l, new_lexer_rule(l, "[0-9]+", TOKEN_NUMBER));
    add_lexer_rule(l, new_lexer_rule(l, "\"[^\"]*\"", TOKEN_STRING));

    add_lexer_rule(l, new_lexer_rule(l, "\\+", TOKEN_ADDITION));
    add_lexer_rule(l, new_lexer_rule(l, "-", TOKEN_SUBTRACTION));
    add_lexer_rule(l, new_lexer_rule(l, "\\*", TOKEN_MULTIPLICATION));
    add_lexer_rule(l, new_lexer_rule(l, "/", TOKEN_DIVISION));

    add_lexer_rule(l, new_lexer_rule(l, "\\(", TOKEN_LPAREN));
    add_lexer_rule(l, new_lexer_rule(l, "\\)", TOKEN_RPAREN));

    add_lexer_rule(l, new_lexer_rule(l, "\\{", TOKEN_LBRACE));
    add_lexer_rule(l, new_lexer_rule(l, "\\}", TOKEN_RBRACE));

    add_lexer_rule(l, new_lexer_rule(l, "\\[", TOKEN_LBRACKET));
    add_lexer_rule(l, new_lexer_rule(l, "\\]", TOKEN_RBRACKET));

    add_lexer_rule(l, new_lexer_rule(l, ",", TOKEN_COMMA));
    add_lexer_rule(l, new_lexer_rule(l, ";", TOKEN_SEMICOLON));

    // print_lexer(l);

    char *input = read_file("test.txt");

    TokenList *tl = tokenizer(input, l);

    print_tokenList(tl);

    error err;
    err.value = ERR_SUCCESS;
    ParseResult *pr = parse(tl, &err);
    if( err.value != ERR_SUCCESS){
        printf("Error: %s\n", err.message);
        return 1;
    }
    for (int i = 0; i < pr->instructionsCount; ++i) {
        printAST(pr->instructions[i], 0);
    }


    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}