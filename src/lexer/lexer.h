#ifndef LEXER_H
#define LEXER_H

#include <regex.h>
#include "token.h"
#include "../errors/errors.h"

typedef struct {
    char *regex; // the regex to match
    TokenType type; // the token type associated with the regex
    regex_t reg; // the compiled regex
} lexer_rule;

typedef struct {
    lexer_rule *rules; // a list of rules
    int nb_rules; // the number of rules
} Lexer;




lexer_rule *new_lexer_rule(char *regex, TokenType type, error *err);
void print_lexer_rule(lexer_rule *rule);
char *get_lang(char *input, error *err);

Lexer *new_lexer(error *err);
int readLexerFile(Lexer *l, char* filename, error *err);
void print_lexer(Lexer *l);
void free_lexer(Lexer *l);
int add_lexer_rule(Lexer *l, lexer_rule *rule, error *err);
int is_token_reserved_by_system(TokenType type);
int is_token_in_lexer(Lexer *l, TokenType tokenName);

TokenList *tokenizer(char *input, Lexer *l, error *err);
char *include_files(char *input, error *err);
char *read_file(char *filename, error *err);

#endif