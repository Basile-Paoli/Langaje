#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

TokenList *new_TokenList() {
    TokenList *tl = (TokenList *)malloc(sizeof(TokenList));
    if (tl == NULL) {
        printf("[ERROR][TOKEN]: Cannot allocate memory for TokenList\n");
        return NULL;
    }

    tl->tokens = NULL;
    tl->nb_tokens = 0;
    return tl;
}

void print_tokenList(TokenList *tl) {
    for (int i = 0; i < tl->nb_tokens; i++) {
        print_token(&tl->tokens[i]);
    }
    printf("\n");
}

void print_token(Token *t) {
    printf("[%s:%s] ", token_type_to_str(t->type), t->value);
    if (t->type == TOKEN_SEMICOLON) printf("\n");
}

Token *new_Token(TokenType type, char *value, int line, int column) {
    Token *t = (Token *)malloc(sizeof(Token));
    if (t == NULL) {
        printf("[ERROR][TOKEN]: Cannot allocate memory for Token\n");
        return NULL;
    }

    t->type = type;
    t->value = (char *) malloc(strlen(value) + 1);
    if (t->value == NULL) {
        printf("[ERROR][TOKEN]: Cannot allocate memory for value\n");
        free(t);
        return NULL;
    }

    // Where the token was found
    t->line = line;
    t->column = column;

    strcpy(t->value, value);
    return t;
}

int add_Token(TokenList *tl, Token *t) {
    tl->tokens = (Token *)realloc(tl->tokens, (tl->nb_tokens + 1) * sizeof(Token));
    if (tl->tokens == NULL) {
        printf("[ERROR][TOKEN]: Cannot allocate memory for token\n");
        return 1;
    }
    
    tl->tokens[tl->nb_tokens++] = *t;

    return 0;
}

void free_Token(Token *t) {
    free(t->value);
}

void free_tokenList(TokenList *tl) {
    for (int i = 0; i < tl->nb_tokens; i++) {
        free_Token(&tl->tokens[i]);
    }
    free(tl->tokens);
    free(tl);
}

TokenType str_to_token_type(char *input) {
    if (strcmp(input, "TOKEN_IDENTIFIER") == 0) return TOKEN_IDENTIFIER;

    else if (strcmp(input, "TOKEN_DEF") == 0) return TOKEN_DEF;
    else if (strcmp(input, "TOKEN_INT") == 0) return TOKEN_INT;
    else if (strcmp(input, "TOKEN_FLOAT") == 0) return TOKEN_FLOAT;
    else if (strcmp(input, "TOKEN_STRING") == 0) return TOKEN_STRING;

    else if (strcmp(input, "TOKEN_IF") == 0) return TOKEN_IF;
    else if (strcmp(input, "TOKEN_ELSE") == 0) return TOKEN_ELSE;
    else if (strcmp(input, "TOKEN_WHILE") == 0) return TOKEN_WHILE;
    else if (strcmp(input, "TOKEN_FOR") == 0) return TOKEN_FOR;
    else if (strcmp(input, "TOKEN_FOREACH") == 0) return TOKEN_FOREACH;
    else if (strcmp(input, "TOKEN_IN") == 0) return TOKEN_IN;

    else if (strcmp(input, "TOKEN_PREPROCESSEUR_LANG") == 0) return TOKEN_PREPROCESSEUR_LANG;
    else if (strcmp(input, "TOKEN_PREPROCESSEUR_INCLUDE") == 0) return TOKEN_PREPROCESSEUR_INCLUDE;

    else if (strcmp(input, "TOKEN_FUNCTION_DECLARATION") == 0) return TOKEN_FUNCTION_DECLARATION;
    else if (strcmp(input, "TOKEN_FUNCTION_RETURN") == 0) return TOKEN_FUNCTION_RETURN;

    else if (strcmp(input, "TOKEN_TYPE_INT") == 0) return TOKEN_TYPE_INT;
    else if (strcmp(input, "TOKEN_TYPE_FLOAT") == 0) return TOKEN_TYPE_FLOAT;
    else if (strcmp(input, "TOKEN_TYPE_STRING") == 0) return TOKEN_TYPE_STRING;

    else if (strcmp(input, "TOKEN_EQUAL_EQUAL") == 0) return TOKEN_EQUAL_EQUAL;
    else if (strcmp(input, "TOKEN_NOT_EQUAL") == 0) return TOKEN_NOT_EQUAL;
    else if (strcmp(input, "TOKEN_GREATER") == 0) return TOKEN_GREATER;
    else if (strcmp(input, "TOKEN_GREATER_EQUAL") == 0) return TOKEN_GREATER_EQUAL;
    else if (strcmp(input, "TOKEN_LESS") == 0) return TOKEN_LESS;
    else if (strcmp(input, "TOKEN_LESS_EQUAL") == 0) return TOKEN_LESS_EQUAL;

    else if (strcmp(input, "TOKEN_AND") == 0) return TOKEN_AND;
    else if (strcmp(input, "TOKEN_OR") == 0) return TOKEN_OR;
    else if (strcmp(input, "TOKEN_NOT") == 0) return TOKEN_NOT;

    else if (strcmp(input, "TOKEN_EQUAL") == 0) return TOKEN_EQUAL;

    else if (strcmp(input, "TOKEN_ADDITION") == 0) return TOKEN_ADDITION;
    else if (strcmp(input, "TOKEN_SUBSTRACTION") == 0) return TOKEN_SUBSTRACTION;
    else if (strcmp(input, "TOKEN_MULTIPLICATION") == 0) return TOKEN_MULTIPLICATION;
    else if (strcmp(input, "TOKEN_DIVISION") == 0) return TOKEN_DIVISION;

    else if (strcmp(input, "TOKEN_LPAREN") == 0) return TOKEN_LPAREN;
    else if (strcmp(input, "TOKEN_RPAREN") == 0) return TOKEN_RPAREN;

    else if (strcmp(input, "TOKEN_LBRACE") == 0) return TOKEN_LBRACE;
    else if (strcmp(input, "TOKEN_RBRACE") == 0) return TOKEN_RBRACE;

    else if (strcmp(input, "TOKEN_LBRACKET") == 0) return TOKEN_LBRACKET;
    else if (strcmp(input, "TOKEN_RBRACKET") == 0) return TOKEN_RBRACKET;

    else if (strcmp(input, "TOKEN_COMMA") == 0) return TOKEN_COMMA;
    else if (strcmp(input, "TOKEN_SEMICOLON") == 0) return TOKEN_SEMICOLON;

    else if (strcmp(input, "TOKEN_UNKNOWN") == 0) return TOKEN_UNKNOWN;
    else printf("Unknown token type: %s\n", input);
    return TOKEN_UNKNOWN;
}

char *token_type_to_str(TokenType type) {
    char *TokenType[] = {
        [TOKEN_IDENTIFIER           ] = "IDENTIFIER",

        [TOKEN_DEF                  ] = "DEF",
        [TOKEN_INT                  ] = "INT",
        [TOKEN_FLOAT                ] = "FLOAT",
        [TOKEN_STRING               ] = "STRING",

        [TOKEN_IF                   ] = "IF",
        [TOKEN_ELSE                 ] = "ELSE",
        [TOKEN_WHILE                ] = "WHILE",
        [TOKEN_FOR                  ] = "FOR",
        [TOKEN_FOREACH              ] = "FOREACH",

        [TOKEN_PREPROCESSEUR_LANG   ] = "PREPOCESSEUR_LANG",
        [TOKEN_PREPROCESSEUR_INCLUDE] = "PREPOCESSEUR_INCLUDE",

        [TOKEN_FUNCTION_DECLARATION ] = "FUNCTION_DECLARATION",
        [TOKEN_FUNCTION_RETURN      ] = "FUNCTION_RETURN",

        [TOKEN_TYPE_INT             ] = "TYPE_INT",
        [TOKEN_TYPE_FLOAT           ] = "TYPE_FLOAT",
        [TOKEN_TYPE_STRING          ] = "TYPE_STRING",

        [TOKEN_EQUAL_EQUAL          ] = "EQUAL_EQUAL",
        [TOKEN_NOT_EQUAL            ] = "NOT_EQUAL",
        [TOKEN_GREATER              ] = "GREATER",
        [TOKEN_GREATER_EQUAL        ] = "GREATER_EQUAL",
        [TOKEN_LESS                 ] = "LESS",
        [TOKEN_LESS_EQUAL           ] = "LESS_EQUAL",

        [TOKEN_AND                  ] = "AND",
        [TOKEN_OR                   ] = "OR",
        [TOKEN_NOT                  ] = "NOT",

        [TOKEN_EQUAL                ] = "EQUAL",

        [TOKEN_ADDITION             ] = "ADDITION",
        [TOKEN_SUBSTRACTION         ] = "SUBSTRACTION",
        [TOKEN_MULTIPLICATION       ] = "MULTIPLICATION",
        [TOKEN_DIVISION             ] = "DIVISION",

        [TOKEN_LPAREN               ] = "LPAREN",
        [TOKEN_RPAREN               ] = "RPAREN",

        [TOKEN_LBRACE               ] = "LBRACE",
        [TOKEN_RBRACE               ] = "RBRACE",

        [TOKEN_LBRACKET             ] = "LBRACKET",
        [TOKEN_RBRACKET             ] = "RBRACKET",

        [TOKEN_COMMA                ] = "COMMA",
        [TOKEN_SEMICOLON            ] = "SEMICOLON",

        [TOKEN_MEMORY_DUMP          ] = "MEMORY_DUMP",
        [TOKEN_BREAKPOINT           ] = "BREAKPOINT",
        [TOKEN_EXIT                 ] = "EXIT",

        [TOKEN_UNKNOWN              ] = "UNKNOWN",
        [TOKEN_COMMENT              ] = "COMMENT"

    };

    return TokenType[type];
}