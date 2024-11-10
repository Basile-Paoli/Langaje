#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "lexer.h"
#include "token.h"

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
    add_lexer_rule(l, new_lexer_rule("#LANG_([A-Z])+"               , TOKEN_PREPROCESSEUR_LANG)) +
    add_lexer_rule(l, new_lexer_rule("#include"                     , TOKEN_PREPROCESSEUR_INCLUDE)) +
    add_lexer_rule(l, new_lexer_rule("[0-9]+\\.[0-9]+"              , TOKEN_FLOAT)) +
    add_lexer_rule(l, new_lexer_rule("[0-9]+"                       , TOKEN_INT)) +
    add_lexer_rule(l, new_lexer_rule("\"[^\"]*\""                   , TOKEN_STRING)) + 
    add_lexer_rule(l, new_lexer_rule("f\"[^\"]*\""                  , TOKEN_FORMATTED_STRING)) +
    add_lexer_rule(l, new_lexer_rule("@memoryDump"                  , TOKEN_MEMORY_DUMP)) +
    add_lexer_rule(l, new_lexer_rule("@cli"                         , TOKEN_CLI_MODE)) + 
    add_lexer_rule(l, new_lexer_rule("@breakPoint"                  , TOKEN_BREAKPOINT)) +
    add_lexer_rule(l, new_lexer_rule("//[^\n]*"                     , TOKEN_COMMENT)) + 
    add_lexer_rule(l, new_lexer_rule("/\\*([^*]|\\*+[^*/])*\\*+/"   , TOKEN_COMMENT))
    != 0) {
        printf("[ERROR][LEXER]: Cannot add default rules\n");
        return NULL;
    }

    return l;

}

char *get_lang(char *input) {
    // input is the whole file
    // We get the lang preprocessor
    char *lang = NULL;

    char *temp = (char *)calloc(strlen(input), sizeof(char));
    if (temp == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for temp\n");
        return NULL;
    }
    strcpy(temp, input);
    
    if (strstr(temp, "#LANG_") != NULL) {
        lang = (char *)calloc(10, sizeof(char));
        if (lang == NULL) {
            printf("[ERROR][LEXER]: Cannot allocate memory for lang\n");
            return NULL;
        }
        size_t k = 0;
        while (*temp != '\n' && *temp != '\0') {
            if (*temp == '#') {
                while (*temp != '_') temp++;
                temp++;
                while (*temp != '\n' && *temp != '\0') lang[k++] = *temp++;
                break;
            }
            temp++;
        }
    }

    return lang;
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

TokenList *replaceSugar(TokenList *tl, Lexer *l) {
    // We replace the sugar syntax by the real syntax
    // For example: a++ => a = a + 1
    //              a-- => a = a - 1
    //              a += 2 => a = a + 2
    //              a -= 2 => a = a - 2
    //              f"Hello {name}" => "Hello " + name
    //              ...

    for (int i = 0; i < tl->nb_tokens; i++) {
        if (next_n_token_equal_to(tl, i, 3, (TokenType[]){TOKEN_IDENTIFIER, TOKEN_ADDITION, TOKEN_ADDITION})) {

            Token *equal = new_Token(TOKEN_EQUAL, "=", tl->tokens[i].line, tl->tokens[i].column);
            Token *addition = new_Token(TOKEN_ADDITION, "+", tl->tokens[i].line, tl->tokens[i].column);
            Token *one = new_Token(TOKEN_INT, "1", tl->tokens[i].line, tl->tokens[i].column);

            TokenList *tempTokenList = new_TokenListFromTokens((Token[]){
                tl->tokens[i],
                *equal,
                tl->tokens[i],
                *addition,
                *one
                
            }, 5);

            tl = insertTokenListIntoTokenList(
                tl,
                tempTokenList,
                i
            );
            tl = removeNTokenFromTokenList(tl, i + 5, 3);

        } else if (next_n_token_equal_to(tl, i, 3, (TokenType[]){TOKEN_IDENTIFIER, TOKEN_SUBSTRACTION, TOKEN_SUBSTRACTION})) {

            Token *equal = new_Token(TOKEN_EQUAL, "=", tl->tokens[i].line, tl->tokens[i].column);
            Token *sub = new_Token(TOKEN_SUBSTRACTION, "-", tl->tokens[i].line, tl->tokens[i].column);
            Token *one = new_Token(TOKEN_INT, "1", tl->tokens[i].line, tl->tokens[i].column);

            TokenList *tempTokenList = new_TokenListFromTokens((Token[]){
                tl->tokens[i],
                *equal,
                tl->tokens[i],
                *sub,
                *one
                
            }, 5);

            tl = insertTokenListIntoTokenList(
                tl,
                tempTokenList,
                i
            );
            tl = removeNTokenFromTokenList(tl, i + 5, 3);
        } else if (next_n_token_equal_to(tl, i, 3, (TokenType[]){TOKEN_IDENTIFIER, TOKEN_ADDITION, TOKEN_EQUAL})) {

            Token *equal = new_Token(TOKEN_EQUAL, "=", tl->tokens[i].line, tl->tokens[i].column);
            Token *addition = new_Token(TOKEN_ADDITION, "+", tl->tokens[i].line, tl->tokens[i].column);

            TokenList *tempTokenList = new_TokenListFromTokens((Token[]){
                tl->tokens[i],
                *equal,
                tl->tokens[i],
                *addition
            }, 4);

            tl = insertTokenListIntoTokenList(
                tl,
                tempTokenList,
                i
            );

            tl = removeNTokenFromTokenList(tl, i + 4, 3);

        } else if (next_n_token_equal_to(tl, i, 3, (TokenType[]){TOKEN_IDENTIFIER, TOKEN_SUBSTRACTION, TOKEN_EQUAL})) {

            Token *equal = new_Token(TOKEN_EQUAL, "=", tl->tokens[i].line, tl->tokens[i].column);
            Token *sub = new_Token(TOKEN_SUBSTRACTION, "-", tl->tokens[i].line, tl->tokens[i].column);

            TokenList *tempTokenList = new_TokenListFromTokens((Token[]){
                tl->tokens[i],
                *equal,
                tl->tokens[i],
                *sub
            }, 4);

            tl = insertTokenListIntoTokenList(
                tl,
                tempTokenList,
                i
            );

            tl = removeNTokenFromTokenList(tl, i + 4, 3);

        } else if (next_n_token_equal_to(tl, i, 3, (TokenType[]){TOKEN_IDENTIFIER, TOKEN_MULTIPLICATION, TOKEN_EQUAL})) {
                
                Token *equal = new_Token(TOKEN_EQUAL, "=", tl->tokens[i].line, tl->tokens[i].column);
                Token *mul = new_Token(TOKEN_MULTIPLICATION, "*", tl->tokens[i].line, tl->tokens[i].column);
    
                TokenList *tempTokenList = new_TokenListFromTokens((Token[]){
                    tl->tokens[i],
                    *equal,
                    tl->tokens[i],
                    *mul
                }, 4);
    
                tl = insertTokenListIntoTokenList(
                    tl,
                    tempTokenList,
                    i
                );
    
                tl = removeNTokenFromTokenList(tl, i + 4, 3);

        } else if (next_n_token_equal_to(tl, i, 3, (TokenType[]){TOKEN_IDENTIFIER, TOKEN_DIVISION, TOKEN_EQUAL})) {
    
            Token *equal = new_Token(TOKEN_EQUAL, "=", tl->tokens[i].line, tl->tokens[i].column);
            Token *div = new_Token(TOKEN_DIVISION, "/", tl->tokens[i].line, tl->tokens[i].column);

            TokenList *tempTokenList = new_TokenListFromTokens((Token[]){
                tl->tokens[i],
                *equal,
                tl->tokens[i],
                *div
            }, 4);

            tl = insertTokenListIntoTokenList(
                tl,
                tempTokenList,
                i
            );

            tl = removeNTokenFromTokenList(tl, i + 4, 3);
        } else if (tl->tokens[i].type == TOKEN_FORMATTED_STRING) {
            // replace '{' by '"+' and '}' by '+"'
            char *buffer = (char *)calloc(strlen(tl->tokens[i].value) + 1, sizeof(char));
            if (buffer == NULL) {
                printf("[ERROR][LEXER]: Cannot allocate memory for buffer\n");
                return NULL;
            }
            strcpy(buffer, tl->tokens[i].value + 1);

            for (int j = 0; j < strlen(buffer); j++) {
                if (buffer[j] == '{') {
                    buffer[j] = '"';
                    buffer = (char *)realloc(buffer, strlen(buffer) + 2);
                    if (buffer == NULL) {
                        printf("[ERROR][LEXER]: Cannot allocate memory for buffer\n");
                        return NULL;
                    }
                    memmove(buffer + j + 2, buffer + j + 1, strlen(buffer) - j);
                    buffer[j + 1] = '+';
                } else if (buffer[j] == '}') {
                    buffer[j] = '+';
                    buffer = (char *)realloc(buffer, strlen(buffer) + 2);
                    if (buffer == NULL) {
                        printf("[ERROR][LEXER]: Cannot allocate memory for buffer\n");
                        return NULL;
                    }
                    memmove(buffer + j + 2, buffer + j + 1, strlen(buffer) - j);
                    buffer[j + 1] = '"';
                }
            }
            TokenList *tempTokenList = tokenizer(buffer, l);
            if (tempTokenList == NULL) return NULL;

            tl = insertTokenListIntoTokenList(
                tl,
                tempTokenList,
                i
            );

            tl = removeNTokenFromTokenList(tl, i + tempTokenList->nb_tokens, 1);
        }
    }

    return tl;

}

TokenList *tokenizer(char *input, Lexer *l) {

    TokenList *list = new_TokenList();
    if (list == NULL) {
        printf("[ERROR][LEXER]: Cannot allocate memory for TokenList\n");
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

        // For each rule
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
                
            // We copy the matched string
            char *buffer = (char *)calloc(matchEndIndex, sizeof(char));
            if (buffer == NULL) {
                printf("[ERROR][LEXER]: Cannot allocate memory for buffer\n");
                return NULL;
            }
            strncpy(buffer, input + matchStartIndex, matchEndIndex);

            // Create the token
            Token *t = new_Token(l->rules[ruleIndex].type, buffer, nbLine, matchStartIndex);
            if (t == NULL) {
                printf("[ERROR][LEXER]: Cannot allocate memory for token\n");
                return NULL;
            }

            // Add the token to the list
            if (add_Token(list, t) != 0) {
                printf("[ERROR][LEXER]: Cannot add token to list\n");
                return NULL;
            }
            
            free(buffer);

            i+=matchEndIndex-1; // We move i to the end of the match
            nbColon += matchEndIndex;
            
        } else { // If no match has been found, we print an error
            printf("[ERROR][LEXER]: Unknown token: <%c> at position Ln-%d Col-%d\n", input[i], nbLine, nbColon);
            printf("[ERROR][LEXER]: The lang used may not be correctly configurated\n");

            // print the line
            int j = i;
            while (input[j] != '\n' && j > 0) j--;
            j++;
            while (input[j] != '\n' && input[j] != '\0') printf("%c", input[j++]);
            printf("\n");
            for (int k = 1; k < nbColon; k++) printf(" ");
            printf("^\n"); // the error is here

            free_tokenList(list);
            return NULL;
        }

    }

    return list;
}

char *read_file(char *filename) {
    // We read the file and output a single char *
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
    strcpy(rule->regex, regex); // copy the regex string to the rule

    rule->type = type;
    regcomp(&rule->reg, rule->regex, REG_EXTENDED); // compile the regex in advance

    return rule;
}
int add_lexer_rule(Lexer *l, lexer_rule *rule){
    // Add a rule to the lexer (a rule is a regex and a token type)

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