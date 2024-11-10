#ifndef LEXER_H
#define LEXER_H

#include <regex.h>
#include "token.h"

typedef struct {
    char *regex; // the regex to match
    TokenType type; // the token type associated with the regex
    regex_t reg; // the compiled regex
} lexer_rule;

typedef struct {
    lexer_rule *rules; // a list of rules
    int nb_rules; // the number of rules
} Lexer;




lexer_rule *new_lexer_rule(char *regex, TokenType type);
void print_lexer_rule(lexer_rule *rule);
char *get_lang(char *input);

Lexer *new_lexer();
int readLexerFile(Lexer *l, char* filename);
void print_lexer(Lexer *l);
void free_lexer(Lexer *l);
int add_lexer_rule(Lexer *l, lexer_rule *rule);
int verifyLexerLine(char *line, char *tokenName, Lexer *l);
int is_token_reserved_by_system(TokenType type);
int is_token_in_lexer(Lexer *l, TokenType tokenName);

TokenList *replaceSugar(TokenList *tl);
TokenList *tokenizer(char *input, Lexer *l) ;
char *read_file(char *filename);
void split_into_instructions(char *buffer, char ***instructions, int *nbInstructions, lexer_rule *ruleInstructions);

#endif