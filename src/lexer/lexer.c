#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"
#include "token.h"

Lexer *new_lexer() {

    Lexer *l = (Lexer *)malloc(sizeof(Lexer));
    if (l == NULL) return NULL;

    l->rules = NULL;
    l->nb_rules = 0;

    return l;

}

TokenList *tokenizer(char *input, Lexer *l) {

    TokenList *list = new_TokenList();

    // For each character in the input
    for (int i = 0; i < strlen(input);i++) {

        // For each rule
        for (int j = 0; j < l->nb_rules; j++) {
            
            regex_t regex;
            regcomp(&regex, l->rules[j].regex, REG_EXTENDED);
            size_t maxGroup = 10;
            regmatch_t match[maxGroup];

            if (regexec(&regex, input + i, maxGroup, match, 0) == 0) {

                if (match->rm_so != 0) continue;
                
                char *buffer = (char *)calloc(match->rm_eo, sizeof(char));
                if (buffer == NULL) return NULL;
                strncpy(buffer, input + i, match->rm_eo);

                Token *t = new_Token(l->rules[j].type, buffer);
                add_Token(list, t);
                
                free(buffer);

                i += match->rm_eo-1;
                break;

            }

        }

    }

    return list;
}

char *read_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);

    return buffer;

}

lexer_rule *new_lexer_rule(Lexer *l, char *regex, TokenType type){
    lexer_rule *rule = (lexer_rule *)malloc(sizeof(lexer_rule));
    if (rule == NULL) return NULL;

    rule->regex = (char *)malloc(strlen(regex) + 1);
    if (rule->regex == NULL) return NULL;
    strcpy(rule->regex, regex);

    rule->type = type;
    return rule;
}
void add_lexer_rule(Lexer *l, lexer_rule *rule){
    l->rules = (lexer_rule *)realloc(l->rules, (l->nb_rules + 1) * sizeof(lexer_rule));
    if (l->rules == NULL) return;

    l->rules[l->nb_rules] = *rule;
    l->nb_rules++;
}

void free_lexer(Lexer *l) {
    for (int i = 0; i < l->nb_rules; i++) {
        free(l->rules[i].regex);
    }
    free(l->rules);
    free(l);
}

void print_lexer(Lexer *l){
    for (int i = 0; i < l->nb_rules; i++) {
        print_lexer_rule(&l->rules[i]);
    }
}
void print_lexer_rule(lexer_rule *rule){
    printf("Rule: %s %s\n", token_type_to_str(rule->type), rule->regex);
}