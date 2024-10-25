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

TokenType str_to_token_type(char *input) {
    if (strcmp(input, "TOKEN_DEF") == 0) return TOKEN_DEF;
    else if (strcmp(input, "TOKEN_TYPE_INT") == 0) return TOKEN_INT;
    else if (strcmp(input, "TOKEN_TYPE_FLOAT") == 0) return TOKEN_FLOAT;
    else if (strcmp(input, "TOKEN_TYPE_STRING") == 0) return TOKEN_STRING;

    else if (strcmp(input, "TOKEN_IF") == 0) return TOKEN_IF;
    else if (strcmp(input, "TOKEN_ELSE") == 0) return TOKEN_ELSE;
    else if (strcmp(input, "TOKEN_WHILE") == 0) return TOKEN_WHILE;
    else if (strcmp(input, "TOKEN_FOR") == 0) return TOKEN_FOR;
    
    else if (strcmp(input, "TOKEN_FUNCTION_DECLARATION") == 0) return TOKEN_FUNCTION_DECLARATION;
    else if (strcmp(input, "TOKEN_FUNCTION_RETURN") == 0) return TOKEN_FUNCTION_RETURN;
    
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
    else printf("Unknown token type: %s\n", input);
    return TOKEN_UNKNOWN;
}

char *token_type_to_str(TokenType type) {
    switch (type) {

        case TOKEN_IDENTIFIER: return "IDENTIFIER";

        case TOKEN_DEF: return "DEF";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";

        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";

        case TOKEN_PREPROCESSEUR_LANG: return "PREPOCESSEUR_LANG";
        case TOKEN_PREPROCESSEUR_INCLUDE: return "PREPROCESSEUR_INCLUDE";

        case TOKEN_FUNCTION_DECLARATION: return "FUNCTION_DECLARATION";
        case TOKEN_FUNCTION_RETURN: return "FUNCTION_RETURN";

        case TOKEN_TYPE_INT: return "TYPE_INT";
        case TOKEN_TYPE_FLOAT: return "TYPE_FLOAT";
        case TOKEN_TYPE_STRING: return "TYPE_STRING";

        case TOKEN_EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";

        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";

        case TOKEN_EQUAL: return "EQUAL";

        case TOKEN_ADDITION: return "ADDITION";
        case TOKEN_SUBSTRACTION: return "SUBSTRACTION";
        case TOKEN_MULTIPLICATION: return "MULTIPLICATION";
        case TOKEN_DIVISION: return "DIVISION";

        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";

        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";

        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";

        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        
        case TOKEN_UNKNOWN: return "UNKNOWN"; 
    }
}