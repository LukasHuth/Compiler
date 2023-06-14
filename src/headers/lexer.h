#ifndef H_LEXER
#define H_LEXER
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#define LEXER_TAG enum LEXER_TAG
// typedef enum LEXER_TAG LEXER_TAG;
LEXER_TAG {
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
    LEXER_OPEN_BRACKET,
    LEXER_CLOSE_BRACKET,
    LEXER_EOF,
};
typedef struct LEXER_TOKEN LEXER_TOKEN;
struct LEXER_TOKEN {
    LEXER_TAG tag;
    char* data;
};
typedef struct LEXER LEXER;
struct LEXER {
    size_t index;
    FILE *file;
    LEXER_TOKEN** tokens;
    size_t buffer_size;
};
LEXER* init_lexer(FILE* file);
void lex(LEXER *lexer);
void lexer_print(LEXER *lexer);
void lexer_free(LEXER *lexer);
std::string LEXER_TAG_to_string(LEXER_TAG tag);
#endif