#ifndef H_LEXER
#define H_LEXER
#include <string.h>
#include <stdio.h>
#include <ctype.h>
typedef enum LEXER_TAG LEXER_TAG;
enum LEXER_TAG {
    LEXER_NUMBER,
    LEXER_PLUS,
    LEXER_MINUS,
    LEXER_STAR,
    LEXER_SLASH,
    LEXER_OPEN_PAREN,
    LEXER_CLOSE_PAREN,
    LEXER_KEYWORD,
    LEXER_IDENTIFIER,
    LEXER_SEMICOLON,
    LEXER_EQUALS,
    LEXER_COMMA,
    LEXER_COLON,
    LEXER_LESS,
    LEXER_GREATER,
    LEXER_ARROW,
    LEXER_OPEN_BRACE,
    LEXER_CLOSE_BRACE,
    LEXER_EOF,
};
typedef struct LEXER_TOKEN LEXER_TOKEN;
struct LEXER_TOKEN {
    LEXER_TAG tag;
    union {
        struct LEXER_NUMBER { int number; } LEXER_NUMBER;
        struct LEXER_KEYWORD { char* keyword; } LEXER_KEYWORD;
        struct LEXER_IDENTIFIER { char* identifier; } LEXER_IDENTIFIER;
        struct LEXER_DATA { char* data; } LEXER_DATA;
    } data;
};
typedef struct LEXER LEXER;
struct LEXER {
    FILE *file;
    LEXER_TOKEN* buffer;
    size_t buffer_size;
};
LEXER* init_lexer(FILE* file);
void lex(LEXER *lexer);
void lexer_print(LEXER *lexer);
void lexer_free(LEXER *lexer);
#endif