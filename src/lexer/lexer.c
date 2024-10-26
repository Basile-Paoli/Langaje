#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"
#include "token.h"
#include "../errors/errors.h"

Lexer *new_lexer() {

    Lexer *l = (Lexer *)malloc(sizeof(Lexer));
    if (l == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for lexer\n");
        return NULL;
    }

    l->rules = NULL;
    l->nb_rules = 0;

    // Default rules
    if (
    add_lexer_rule(l, new_lexer_rule("#LANG_([A-Z])+", TOKEN_PREPROCESSEUR_LANG)) +
    add_lexer_rule(l, new_lexer_rule("#include", TOKEN_PREPROCESSEUR_INCLUDE)) +
    add_lexer_rule(l, new_lexer_rule("[0-9]+\\.[0-9]+", TOKEN_FLOAT)) +
    add_lexer_rule(l, new_lexer_rule("[0-9]+", TOKEN_INT)) +
    add_lexer_rule(l, new_lexer_rule("\"[^\"]*\"", TOKEN_STRING))
    != 0) {
        printf("[ERROR][LEXER]: Cannot add default rules\n");
        return NULL;
    }

    return l;

}

int readLexerFile(Lexer *l, char* filename) {
    char *buffer = read_file(filename);
    if (buffer == NULL){
        printf("[ERROR][LEXER]: Cannot read file <%s>\n", filename);
        return 1;
    }

    if (strlen(buffer) == 0) {
        printf("[ERROR][LEXER]: Empty file\n");
        return 1;
    }

    // for each line in the file
    for (char *line = strtok(buffer, "\n"); line != NULL; line = strtok(NULL, "\n")) {

        // We get the token name
        char *tokenName = (char *)calloc(strlen(line), sizeof(char));
        if (tokenName == NULL) {printf("[ERROR][LEXER]: Cannot allocate memory for tokenName\n"); return 1;}
        size_t k = 0;
        char *tempLine = (char *)malloc(strlen(line) + 1);
        if (tempLine == NULL) {printf("[ERROR][LEXER]: Cannot allocate memory for tempLine\n"); return 1;}
        strcpy(tempLine, line);
        while (*tempLine != '=') *(tokenName+k++) = *tempLine++;

        if (verifyLexerLine(line, tokenName, l) != 0) {
            printf("[ERROR][LEXER]: Invalid line <%s>\n", line);
            return 1;
        }

        if (add_lexer_rule(l, new_lexer_rule(strstr(line, "=") + 1, str_to_token_type(tokenName))) != 0) {
            printf("[ERROR][LEXER]: Cannot add rule for line <%s>\n", line);
            return 1;
        }

        free(tokenName);
    }

    free(buffer);

    // Default rule for identifiers. This rule must be the last one
    if (add_lexer_rule(l, new_lexer_rule("[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_IDENTIFIER)) != 0) {
        printf("[ERROR][LEXER]: Cannot add default rule for identifiers\n");
        return 1;
    }

    return 0;

}

int verifyLexerLine(char *line, char *tokenName, Lexer *l) {

    if (strlen(line) == 0) {
        printf("[ERROR][LEXER]: Empty line\n");
        return 1;
    }

    if (strchr(line, '=') == NULL) {
        printf("[ERROR][LEXER]: Missing '=' at line <%s>\n", line);
        return 1;
    }

    if (strchr(line, '=') == line) {
        printf("[ERROR][LEXER]: Missing token name at line <%s>\n", line);
        return 1;
    }

    if (strchr(line, '=') == line + strlen(line) - 1) {
        printf("[ERROR][LEXER]: Missing regex at line <%s>\n", line);
        return 1;
    }

    if (str_to_token_type(tokenName) == TOKEN_UNKNOWN) {
        printf("[ERROR][LEXER]: Token unknown at line <%s>\n", line);
        return 1;
    }

    if (is_token_in_lexer(l, str_to_token_type(tokenName)) == 1) {
        printf("[ERROR][LEXER]: Token already defined at line <%s>\n", line);
        return 1;
    }

    regex_t reg;
    if (regcomp(&reg, strstr(line, "=") + 1, REG_EXTENDED) != 0) {
        printf("[ERROR][LEXER]: Invalid regex at line <%s>\n", line);
        return 1;
    }

    if (is_token_reserved_by_system(str_to_token_type(tokenName))) {
        printf("[ERROR][LEXER]: Token reserved by the system at line <%s>\n", line);
        return 1;
    }

    return 0;
}

int is_token_reserved_by_system(TokenType type) {
    switch (type) {
        case TOKEN_UNKNOWN:
        case TOKEN_INT:
        case TOKEN_FLOAT:
        case TOKEN_STRING:
        case TOKEN_IDENTIFIER:
        case TOKEN_PREPROCESSEUR_INCLUDE:
        case TOKEN_PREPROCESSEUR_LANG:
            return 1;
        default:
            return 0;
    }
}

int is_token_in_lexer(Lexer *l, TokenType token) {
    for (int i = 0; i < l->nb_rules; i++) {
        if (l->rules[i].type == token) return 1;
    }
    return 0;
}

TokenList *tokenizer(char *input, Lexer *l) {

    TokenList *list = new_TokenList();
    if (list == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for TokenList\n");
        return NULL;
    }

    char matchFound;

    // For each character in the input
    for (int i = 0; i < strlen(input); i++) {
        matchFound = 0;

        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\t') continue; // Skip spaces

        // For each rule
        for (int j = 0; j < l->nb_rules; j++)  {
            
            size_t maxGroup = 10;
            regmatch_t match[maxGroup];

            if (regexec(&l->rules[j].reg, input + i, maxGroup, match, 0) == 0) {

                if (match->rm_so != 0) continue; // The match MUST start at the beginning of the string

                matchFound = 1;
                
                char *buffer = (char *)calloc(match->rm_eo, sizeof(char));
                if (buffer == NULL) {
                    printf("[ERROR][LEXER]: Cannot allocate memory for buffer\n");
                    return NULL;
                }
                strncpy(buffer, input + i, match->rm_eo);

                Token *t = new_Token(l->rules[j].type, buffer);
                if (t == NULL) {
                    printf("[ERROR][LEXER]: Cannot allocate memory for token\n");
                    return NULL;
                }
                if (add_Token(list, t) != 0) {
                    printf("[ERROR][LEXER]: Cannot add token to list\n");
                    return NULL;
                }
                
                free(buffer);

                i += match->rm_eo-1;
                break;

            }

        }

        if (!matchFound) {
            printf("[ERROR][LEXER]: Unknown token: <%c> at position %d\n", input[i], i);
            printf("[ERROR][LEXER]: The lang used may not be correctly configurated\n");
            // print the line
            int j = i;
            while (input[j] != '\n' && j > 0) j--;
            j++;
            while (input[j] != '\n' && input[j] != '\0') printf("%c", input[j++]);
            printf("\n");
            for (int k = 0; k < i; k++) printf(" ");
            printf("^\n");

            free_tokenList(list);
            return NULL;
        }

    }

    return list;
}

char *read_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[ERROR][LEXER]: Cannot open file <%s>\n", filename);
        return NULL;
    }

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
    if (rule == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for rule\n");
        return NULL;
    }

    rule->regex = (char *)malloc(strlen(regex) + 1);
    if (rule->regex == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for regex\n");
        return NULL;
    }
    strcpy(rule->regex, regex);

    rule->type = type;
    regcomp(&rule->reg, rule->regex, REG_EXTENDED);

    return rule;
}
int add_lexer_rule(Lexer *l, lexer_rule *rule){

    if (rule == NULL) {
        printf("[ERROR][LEXER]: rule is NULL\n");
        return 1;
    }

    l->rules = (lexer_rule *)realloc(l->rules, (l->nb_rules + 1) * sizeof(lexer_rule));
    if (l->rules == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for rule\n");
        return 1;
    }

    l->rules[l->nb_rules] = *rule;
    l->nb_rules++;

    return 0;
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