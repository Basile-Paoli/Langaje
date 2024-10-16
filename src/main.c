#include <stdlib.h>
#include <stdio.h>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "functions/functions.h"

int main() {

    Lexer *l = new_lexer();
    if (l == NULL)return 1;

    add_lexer_rule(l, new_lexer_rule(l, "def", TOKEN_KEYWORD));
    add_lexer_rule(l, new_lexer_rule(l, "int", TOKEN_KEYWORD));
    add_lexer_rule(l, new_lexer_rule(l, "if", TOKEN_KEYWORD));
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
    InstructionBlock *pr = parse(tl, &err);
    if (err.value != ERR_SUCCESS) {
        printf("Error: %s\n", err.message);
        return 1;
    }
    printInstructionBlock(pr, 0);


    free_tokenList(tl);
    free_lexer(l);
    free(input);

     */

    var a;
    var b;
    var c;
    error err;

    /*

    a.type = _float;
    a.value._float= 6.0;

    a.type = _int;
    a.value._int= 3;

    a.type = _char;
    a.value._char= 'A';

    a.type = _string;
    a.value._string = malloc(strlen("test") + 1);
    strcpy(a.value._string, "test");

    a.type = _array;
    a.value._array = malloc(sizeof(int ));


     */

    a.type = _float;
    a.value._float= 3.5;

    b.type = _int;
    b.value._int= 2;

    /*

    b.type = _float;
    b.value._float= 3.0;

    b.type = _int;
    b.value._int= 6;

    b.type = _char;
    b.value._char= 'A';

    b.type = _string;
    b.value._string = malloc(strlen("test") + 1);
    b.value._string = "test";

    b.type = _array;
    b.value._array = malloc(sizeof(int ));

    */

    c = power(&a, &b, &err);

    if(err.value != ERR_SUCCESS)
        printf("%s", err.message);
    else
        display(&c);

    return 0;
}