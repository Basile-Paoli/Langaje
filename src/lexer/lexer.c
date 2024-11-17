#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"
#include "token.h"
#include "../errors/errors.h"

Lexer *new_lexer(error *err) {

    Lexer *l = malloc(sizeof(Lexer));
    if (l == NULL) {
        free_lexer(l);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for lexer");
        return NULL;
    }

    l->rules = NULL;
    l->nb_rules = 0;

    // Default rules
    if (
    add_lexer_rule(l, new_lexer_rule("#lang"                        , TOKEN_PREPROCESSEUR_LANG, err), err) +
    add_lexer_rule(l, new_lexer_rule("#include"                     , TOKEN_PREPROCESSEUR_INCLUDE, err), err) +
    add_lexer_rule(l, new_lexer_rule("[0-9_]+\\.[0-9_]+"            , TOKEN_FLOAT, err), err) +
    add_lexer_rule(l, new_lexer_rule("[0-9_]+"                      , TOKEN_INT, err), err) +
    add_lexer_rule(l, new_lexer_rule("\"[^\"]*\""                   , TOKEN_STRING, err), err) +
    add_lexer_rule(l, new_lexer_rule("f\"[^\"]*\""                  , TOKEN_FORMATTED_STRING, err), err) +
    add_lexer_rule(l, new_lexer_rule("@memoryDump"                  , TOKEN_MEMORY_DUMP, err), err) +
    add_lexer_rule(l, new_lexer_rule("@breakPoint"                  , TOKEN_BREAKPOINT, err), err) +
    add_lexer_rule(l, new_lexer_rule("//[^\n]*"                     , TOKEN_COMMENT, err), err) + // Single line comment
    add_lexer_rule(l, new_lexer_rule("/\\*([^*]|\\*+[^*/])*\\*+/"   , TOKEN_COMMENT, err), err) // Multi line comment
    != 0) {
        free_lexer(l);
        assignErrorMessage(err, "Cannot add default rules to lexer");
        return NULL;
    }

    return l;

}

char *get_lang(char *input, error *err) {
    // input is the whole file
    // We get the lang preprocessor
    char *lang = NULL;

    char *temp = calloc(strlen(input) + 1, sizeof(char));
    if (temp == NULL) {
        free(temp);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for temp");
        return NULL;
    }
    strcpy(temp, input);
    int langLength = 10;
    
    if (strstr(temp, "#lang") != NULL) {
        lang = calloc(langLength, sizeof(char));
        if (lang == NULL) {
            free(temp);
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Cannot allocate memory for lang");
            return NULL;
        }
        size_t k = 0;
        while (*temp != '\n' && *temp != '\0') {
            // #lang "name"
            if (*temp == '"') {
                temp++;
                while (*temp != '"') {
                    if (k == langLength) {
                        lang = (char *)realloc(lang, langLength * 2);
                        if (lang == NULL) {
                            free(lang); free(temp);
                            err->value = ERR_MEMORY;
                            assignErrorMessage(err, "Cannot reallocate memory for lang");
                            return NULL;
                        }
                        langLength *= 2;
                    }
                    lang[k++] = *temp++;
                }
                break;
            }
            temp++;
        }
    }

    return lang;
}

int readLexerFile(Lexer *l, char* filename, error *err) {
    char *buffer = read_file(filename, err);
    if (buffer == NULL){
        free(buffer);
        err->value = ERR_FILE;
        assignErrorMessage(err, "Cannot read lexer file :");
        assignErrorMessage(err, filename);
        return 1;
    }

    if (strlen(buffer) == 0) {
        free(buffer);
        err->value = ERR_FILE;
        assignErrorMessage(err, "Empty lexer file :");
        assignErrorMessage(err, filename);
        return 1;
    }

    // for each line in the file
    for (char *line = strtok(buffer, "\n"); line != NULL; line = strtok(NULL, "\n")) {

        if (strlen(line) == 0) {
            free(buffer);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Empty line in lexer file");
            return 1;
        }

        if (strchr(line, '=') == NULL) {
            free(buffer);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Missing '=' at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        if (strchr(line, '=') == line) {
            free(buffer);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Missing token name at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        if (strchr(line, '=') == line + strlen(line) - 1) {
            free(buffer);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Missing regex at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        // We get the token name
        char *tokenName = (char *)calloc(strlen(line), sizeof(char));
        if (tokenName == NULL) {
            free(buffer); free(tokenName);
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Cannot allocate memory for tokenName");
            return 1;
        }
        size_t k = 0;
        char *tempLine = (char *)malloc(strlen(line) + 1);
        if (tempLine == NULL) {
            free(buffer); free(tokenName); free(tempLine);
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Cannot allocate memory for tempLine");
            return 1;
        }
        strcpy(tempLine, line);
        while (*tempLine != '=') *(tokenName+k++) = *tempLine++;
        free(tempLine - k);

        if (str_to_token_type(tokenName) == TOKEN_UNKNOWN) {
            free(buffer); free(tokenName);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Token unknown at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        if (is_token_in_lexer(l, str_to_token_type(tokenName)) == 1) {
            free(buffer); free(tokenName);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Token already defined at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        regex_t reg;
        if (regcomp(&reg, strstr(line, "=") + 1, REG_EXTENDED) != 0) {
            regfree(&reg); free(buffer); free(tokenName);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Invalid regex at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        if (is_token_reserved_by_system(str_to_token_type(tokenName))) {
            regfree(&reg); free(buffer); free(tokenName);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Token reserved by the system at line : ");
            assignErrorMessage(err, line);
            return 1;
        }

        regfree(&reg);

        if (add_lexer_rule(l, new_lexer_rule(strstr(line, "=") + 1, str_to_token_type(tokenName), err), err) != 0) {
            free(buffer); free(tokenName);
            err->value = ERR_SYNTAX;
            assignErrorMessage(err, "Cannot add lexer rule at line: ");
            assignErrorMessage(err, line);
            return 1;
        }

        free(tokenName);
    }

    free(buffer);

    // Default rule for identifiers. This rule must be the last one
    if (add_lexer_rule(l, new_lexer_rule("[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_IDENTIFIER, err), err) != 0) {
        err->value = ERR_SYNTAX;
        assignErrorMessage(err, "Cannot add default rule for identifiers");
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

TokenList *tokenizer(char *input, Lexer *l, error *err) {

    TokenList *list = new_TokenList(err);
    if (list == NULL) {
        free_tokenList(list);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for TokenList");
        return NULL;
    }

    char matchFound;
    char nbLine = 1;
    char nbColon = 1;

    int lengthMatchMax;
    int matchStartIndex;
    int matchEndIndex;
    int ruleIndex;

    // For each character in the input
    for (int i = 0; i < strlen(input); i++) {
        matchFound = 0;

        if (input[i] == ' ' || input[i] == '\t') {nbColon++;continue;} // Skip spaces
        if (input[i] == '\n') {
            nbLine++;
            nbColon = 1;
            continue; // Skip new lines
        }

        lengthMatchMax = 0;
        matchStartIndex = 0;
        matchEndIndex = 0;
        
        ruleIndex = 0;

        // For each rule in the lexer
        for (int j = 0; j < l->nb_rules; j++)  {
            
            size_t maxGroup = 10;
            regmatch_t match[maxGroup];

            if (regexec(&l->rules[j].reg, input + i, maxGroup, match, 0) == 0) { // If there is a match

                if (match->rm_so != 0) continue; // The match MUST start at the beginning of the string

                // We keep the longest match
                if (match->rm_eo > lengthMatchMax) {
                    lengthMatchMax = match->rm_eo;
                    matchEndIndex = match->rm_eo;
                    matchStartIndex = i;
                    ruleIndex = j;
                    matchFound = 1;

                }
            }
        }

        if (matchFound) {

            // If the match is a comment, we skip it
            if (l->rules[ruleIndex].type == TOKEN_COMMENT) {
                i += matchEndIndex;
                continue;
            }

            // we skip the preprocessor whole line
            if (l->rules[ruleIndex].type == TOKEN_PREPROCESSEUR_INCLUDE || l->rules[ruleIndex].type == TOKEN_PREPROCESSEUR_LANG) {
                while (input[i] != '\n' && i < strlen(input)) i++;
                nbLine++;
                nbColon = 1;
                continue;
            }
                
            // We copy the matched string
            char *buffer = (char *)calloc(matchEndIndex + 1, sizeof(char));
            if (buffer == NULL) {
                free(buffer); free_tokenList(list);
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Cannot allocate memory for buffer");
                return NULL;
            }
            strncpy(buffer, input + matchStartIndex, matchEndIndex);

            // Create the token
            Token *t = new_Token(l->rules[ruleIndex].type, buffer, nbLine, nbColon, err);
            if (t == NULL) {
                free(buffer); free_tokenList(list);
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Cannot allocate memory for token");
                return NULL;
            }

            // Add the token to the list
            if (add_Token(list, t, err) != 0) {
                free(buffer); free_tokenList(list);
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Cannot add token to list");
                return NULL;
            }

            free(buffer);

            i+=matchEndIndex-1; // We move i to the end of the match
            nbColon += matchEndIndex;
            
        } else { // If no match has been found, we print an error
            free_tokenList(list);
            err->value = ERR_SYNTAX;

            char *buffer = (char *)calloc(100, sizeof(char));
            if (buffer == NULL) {
                free(buffer); free_tokenList(list);
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Cannot allocate memory for buffer");
                return NULL;
            }
            sprintf(buffer, "<%c> at position Ln-%d Col-%d\n", input[i], nbLine, nbColon);
            strcat(buffer, "The lang used may not be correctly configurated\n");

            // print the line where the error occured
            int j = i;
            while (input[j] != '\n' && j > 0) j--;
            j++;
            while (input[j] != '\n' && input[j] != '\0') {
                buffer = (char *)realloc(buffer, strlen(buffer) * 2);
                if (buffer == NULL) {
                    free(buffer); free_tokenList(list);
                    err->value = ERR_MEMORY;
                    assignErrorMessage(err, "Cannot allocate memory for buffer");
                    return NULL;
                }
                sprintf(buffer, "%s%c", buffer, input[j]);
                j++;
            }

            strcat(buffer, "\n");
            // print ^ where the error occured
            for (int k = 0; k < nbColon - 1; k++) strcat(buffer, " ");
            strcat(buffer, "^\n");

            assignErrorMessage(err, buffer);

            free(buffer);

            return NULL;
        }

    }

    return list;
}

char *include_files(char *input, error *err) {

    // if the input contains #include, we include the file
    char *final = (char *)calloc(strlen(input) + 1, sizeof(char));
    if (final == NULL) {
        free(final);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for final");
        return NULL;
    }

    char *temp = (char *)calloc(strlen(input) + 1, sizeof(char));
    if (temp == NULL) {
        free(final); free(temp);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for temp");
        return NULL;
    }
    strcpy(temp, input);

    // If "#include" is found
    if (strstr(temp, "#include") != NULL) {
        size_t k = 0;
        for (char *line = strtok(temp, "\n"); line != NULL; line = strtok(NULL, "\n")) {
            if (strstr(line, "#include") != NULL) {
                char *filename = (char *)calloc(strlen(line) + 1, sizeof(char));
                if (filename == NULL) {
                    free(final); free(temp); free(filename);
                    err->value = ERR_MEMORY;
                    assignErrorMessage(err, "Cannot allocate memory for filename");
                    return NULL;
                }
                size_t j = 0;
                while (*line != '"') line++;
                line++;
                while (*line != '"') filename[j++] = *line++;
                char *buffer = read_file(filename, err);
                if (buffer == NULL) {
                    free(final); free(temp); free(filename); free(buffer);
                    err->value = ERR_FILE;
                    assignErrorMessage(err, "Cannot read file :");
                    assignErrorMessage(err, filename);
                    return NULL;
                }

                final = (char *)realloc(final, strlen(final) + strlen(buffer) + 1);
                if (final == NULL) {
                    free(final); free(temp); free(filename); free(buffer);
                    err->value = ERR_MEMORY;
                    assignErrorMessage(err, "Cannot allocate memory for final");
                    return NULL;
                }

                strcat(final, buffer);
                strcat(final, "\n");
                free(buffer);
                free(filename);
            } else {
                final = (char *)realloc(final, strlen(final) + strlen(line) + 2);
                if (final == NULL) {
                    free(final); free(temp);
                    err->value = ERR_MEMORY;
                    assignErrorMessage(err, "Cannot allocate memory for final");
                    return NULL;
                }
                strcat(final, line);
                strcat(final, "\n");
            }
        }
    } else {
        strcpy(final, input);
    }

    free(temp);

    return final;
}

char *read_file(char *filename, error *err) {
    // We read the file and output a single char *
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        err->value = ERR_FILE;
        assignErrorMessage(err, "Cannot open file :");
        assignErrorMessage(err, filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(length + 1);
    if (buffer == NULL) {
        free(buffer); fclose(file);
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for buffer");
        return NULL;
    }
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);

    return buffer;

}

lexer_rule *new_lexer_rule(char *regex, TokenType type, error *err) {
    lexer_rule *rule = malloc(sizeof(lexer_rule));
    if (rule == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for rule");
        return NULL;
    }

    rule->regex = malloc(strlen(regex) + 1);
    if (rule->regex == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot allocate memory for rule->regex");
        return NULL;
    }
    strcpy(rule->regex, regex); // copy the regex string to the rule

    rule->type = type;
    regcomp(&rule->reg, rule->regex, REG_EXTENDED); // compile the regex in advance

    return rule;
}
int add_lexer_rule(Lexer *l, lexer_rule *rule, error *err) {
    // Add a rule to the lexer (a rule is a regex and a token type)

    if (rule == NULL) {
        err->value = ERR_NULL_POINTER;
        assignErrorMessage(err, "Cannot add NULL rule to lexer");
        return 1;
    }

    l->rules = (lexer_rule *)realloc(l->rules, (l->nb_rules + 1) * sizeof(lexer_rule));
    if (l->rules == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Cannot reallocate memory for lexer->rules");
        return 1;
    }

    l->rules[l->nb_rules] = *rule;
    l->nb_rules++;

    free(rule);

    return 0;
}

void free_lexer(Lexer *l) {
    for (int i = 0; i < l->nb_rules; i++) {
        free(l->rules[i].regex);
        regfree(&l->rules[i].reg);
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