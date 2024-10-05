#include <stdlib.h>

#include "lexer.h"
#include "token.h"

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
    add_lexer_rule(l, new_lexer_rule(l, "-", TOKEN_SUBSTRACTION));
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

    free_tokenList(tl);
    free_lexer(l);
    free(input);

    return 0;
}