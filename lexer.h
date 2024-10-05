#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    char *regex;
    TokenType type;
} lexer_rule;

typedef struct {
    lexer_rule *rules;
    int nb_rules;
} Lexer;




lexer_rule *new_lexer_rule(Lexer *l, char *regex, TokenType type);
void print_lexer_rule(lexer_rule *rule);

Lexer *new_lexer();
void print_lexer(Lexer *l);
void free_lexer(Lexer *l);
void add_lexer_rule(Lexer *l, lexer_rule *rule);

TokenList *tokenizer(char *input, Lexer *l) ;
char *read_file(char *filename);
void split_into_instructions(char *buffer, char ***instructions, int *nbInstructions, lexer_rule *ruleInstructions);

#endif