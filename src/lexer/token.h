#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_IDENTIFIER,

    TOKEN_KEYWORD,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_FOR,

    TOKEN_PREPROCESSEUR_LANG,
    TOKEN_PREPROCESSEUR_INCLUDE,

    TOKEN_FUNCTION_DECLARATION,

    TOKEN_NUMBER,
    TOKEN_FLOAT,
    TOKEN_STRING,

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
    TOKEN_SUBTRACTION,
    TOKEN_MULTIPLICATION,
    TOKEN_DIVISION,

    TOKEN_LPAREN,
    TOKEN_RPAREN,

    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_LBRACKET,
    TOKEN_RBRACKET,

    TOKEN_COMMA,
    TOKEN_SEMICOLON,

} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct {
    Token *tokens;
    int nb_tokens;
} TokenList;


Token *new_Token(TokenType type, char *value);

void print_token(Token *t);

void free_token(Token *t);

TokenList *new_TokenList();

void print_tokenList(TokenList *tl);

void free_tokenList(TokenList *tl);

void add_Token(TokenList *tl, Token *t);

char *token_type_to_str(TokenType type);


#endif