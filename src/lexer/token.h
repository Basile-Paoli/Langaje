#ifndef TOKEN_H
#define TOKEN_H

#include "../errors/errors.h"

typedef enum {
    TOKEN_IDENTIFIER,

    TOKEN_DEF,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_FORMATTED_STRING,

    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_FOREACH,
    TOKEN_IN,
    TOKEN_BREAK,
    TOKEN_CONTINUE,

    TOKEN_PREPROCESSEUR_LANG,
    TOKEN_PREPROCESSEUR_INCLUDE,

    TOKEN_FUNCTION_DECLARATION,
    TOKEN_FUNCTION_RETURN,

    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_STRING,

    TOKEN_EQUAL_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,

    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,

    TOKEN_EQUAL,

    TOKEN_ADDITION,
    TOKEN_SUBSTRACTION,
    TOKEN_MULTIPLICATION,
    TOKEN_DIVISION,
    TOKEN_MODULO,
    TOKEN_POWER,

    TOKEN_LPAREN,
    TOKEN_RPAREN,

    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_LBRACKET,
    TOKEN_RBRACKET,

    TOKEN_COMMA,
    TOKEN_SEMICOLON,

    TOKEN_MEMORY_DUMP,
    TOKEN_BREAKPOINT,
    TOKEN_EXIT,

    TOKEN_UNKNOWN,
    TOKEN_COMMENT

} TokenType;

typedef struct {
    TokenType type;
    char *value;

    int line;
    int column;
} Token;

typedef struct {
    Token *tokens;
    int nb_tokens;
} TokenList;

Token *new_Token(TokenType type, char *value, int line, int column, error *err);

void print_token(Token *t);

void free_Token(Token *t);

TokenList *new_TokenList(error *err);

void print_tokenList(TokenList *tl);

void free_tokenList(TokenList *tl);

int add_Token(TokenList *tl, Token *t, error *err);

TokenType str_to_token_type(char *input);

char *token_type_to_str(TokenType type);


#endif