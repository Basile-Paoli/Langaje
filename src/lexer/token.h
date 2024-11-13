#ifndef TOKEN_H
#define TOKEN_H

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

int next_n_token_equal_to(TokenList *tl, int startIndex, int nTokens, TokenType *types);

TokenList *mergeTokenLists(TokenList *tl1, TokenList *tl2);

TokenList *removeNTokenFromTokenList(TokenList *tl, int position, int n);

TokenList *insertTokenListIntoTokenList(TokenList *tl, TokenList *tl2, int position);

TokenList *new_TokenListFromTokens(Token *tokens, int nb_tokens);

Token *new_Token(TokenType type, char *value, int line, int column);

void print_token(Token *t);

void free_token(Token *t);

TokenList *new_TokenList();

void print_tokenList(TokenList *tl);

void free_tokenList(TokenList *tl);

int add_Token(TokenList *tl, Token *t);

TokenType str_to_token_type(char *input);

char *token_type_to_str(TokenType type);


#endif