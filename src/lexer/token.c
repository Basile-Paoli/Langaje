#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

TokenList *new_TokenList() {
    TokenList *tl = (TokenList *)malloc(sizeof(TokenList));
    if (tl == NULL) return NULL;

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

Token *new_Token(TokenType type, char *value) {
    Token *t = (Token *)malloc(sizeof(Token));
    if (t == NULL) return NULL;

    t->type = type;
    t->value = (char *)malloc(strlen(value) + 1);
    if (t->value == NULL) {
        free(t);
        return NULL;
    }

    strcpy(t->value, value);
    return t;
}

void add_Token(TokenList *tl, Token *t) {
    tl->tokens = (Token *)realloc(tl->tokens, (tl->nb_tokens + 1) * sizeof(Token));
    if (tl->tokens == NULL) return;
    
    tl->tokens[tl->nb_tokens] = *t;
    tl->nb_tokens++;
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

char *token_type_to_str(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";

        case TOKEN_KEYWORD:
            return "KEYWORD";
        case TOKEN_KEYWORD_IF:
            return "IF";
        case TOKEN_KEYWORD_ELSE:
            return "ELSE";
        case TOKEN_KEYWORD_WHILE:
            return "WHILE";
        case TOKEN_KEYWORD_FOR:
            return "FOR";

        case TOKEN_PREPROCESSEUR_LANG:
            return "PREPOCESSEUR_LANG";
        case TOKEN_PREPROCESSEUR_INCLUDE:
            return "PREPROCESSEUR_INCLUDE";

        case TOKEN_FUNCTION_DECLARATION:
            return "FUNCTION_DECLARATION";

        case TOKEN_NUMBER:
            return "NUMBER";
        case TOKEN_FLOAT:
            return "FLOAT";
        case TOKEN_STRING:
            return "STRING";

        case TOKEN_EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case TOKEN_NOT_EQUAL:
            return "NOT_EQUAL";
        case TOKEN_GREATER:
            return "GREATER";
        case TOKEN_GREATER_EQUAL:
            return "GREATER_EQUAL";
        case TOKEN_LESS:
            return "LESS";
        case TOKEN_LESS_EQUAL:
            return "LESS_EQUAL";

        case TOKEN_AND:
            return "AND";
        case TOKEN_OR:
            return "OR";
        case TOKEN_NOT:
            return "NOT";

        case TOKEN_EQUAL:
            return "EQUAL";

        case TOKEN_ADDITION:
            return "ADDITION";
        case TOKEN_SUBTRACTION:
            return "SUBSTRACTION";
        case TOKEN_MULTIPLICATION:
            return "MULTIPLICATION";
        case TOKEN_DIVISION:
            return "DIVISION";

        case TOKEN_LPAREN:
            return "LPAREN";
        case TOKEN_RPAREN:
            return "RPAREN";

        case TOKEN_LBRACE:
            return "LBRACE";
        case TOKEN_RBRACE:
            return "RBRACE";

        case TOKEN_LBRACKET:
            return "LBRACKET";
        case TOKEN_RBRACKET:
            return "RBRACKET";

        case TOKEN_COMMA:
            return "COMMA";
        case TOKEN_SEMICOLON:
            return "SEMICOLON";
        default:
            return "UNKNOWN";
    }
}