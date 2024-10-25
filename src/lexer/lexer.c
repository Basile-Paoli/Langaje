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

void readLexerFile(Lexer *l, char* filename) {
    char *buffer = read_file(filename);

    // for each line in the file
    for (char *line = strtok(buffer, "\n"); line != NULL; line = strtok(NULL, "\n")) {
        // print from the start of the line to the first "=" without strtok
        char *tokenName = (char *)calloc(strlen(line), sizeof(char));
        if (tokenName == NULL) return;
        size_t k = 0;

        while (*line != '=') *(tokenName+k++) = *line++;

        add_lexer_rule(l, new_lexer_rule(strstr(line, "=") + 1, str_to_token_type(tokenName)));
        free(tokenName);
    }

    free(buffer);

}

TokenList *tokenizer(char *input, Lexer *l) {

    TokenList *list = new_TokenList();
    if (list == NULL) return NULL;

    char matchFound;

    // For each character in the input
    for (int i = 0; i < strlen(input); i++) {
        matchFound = 0;

        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\t') continue;

        // For each rule
        for (int j = 0; j < l->nb_rules; j++) {
            
            size_t maxGroup = 10;
            regmatch_t match[maxGroup];

            if (regexec(&l->rules[j].reg, input + i, maxGroup, match, 0) == 0) {

                if (match->rm_so != 0) continue;
                if (match->rm_eo == 0) {
                    print_lexer_rule(&l->rules[j]);
                }

                matchFound = 1;
                
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

        if (!matchFound) {
            printf("Unknown token: %c\n", input[i]);
            return NULL;
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

lexer_rule *new_lexer_rule(char *regex, TokenType type){
    lexer_rule *rule = (lexer_rule *)malloc(sizeof(lexer_rule));
    if (rule == NULL) return NULL;

    rule->regex = (char *)malloc(strlen(regex) + 1);
    if (rule->regex == NULL) return NULL;
    strcpy(rule->regex, regex);

    rule->type = type;
    regcomp(&rule->reg, rule->regex, REG_EXTENDED);

    return rule;
}
void add_lexer_rule(Lexer *l, lexer_rule *rule){

    if (rule == NULL) {
        printf("Error: rule is NULL\n");
        return;
    }

    l->rules = (lexer_rule *)realloc(l->rules, (l->nb_rules + 1) * sizeof(lexer_rule));
    if (l->rules == NULL) {
        printf("Error: realloc\n");
        return;
    }

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
    printf("Rule => %s %s\n", token_type_to_str(rule->type), rule->regex);
}