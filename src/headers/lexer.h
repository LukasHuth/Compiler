#ifndef H_LEXER
#define H_LEXER
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
namespace Lexer
{
    // #define TAG enum TAG
    enum Tag {
        NUMBER,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        OPEN_PAREN,
        CLOSE_PAREN,
        KEYWORD,
        IDENTIFIER,
        SEMICOLON,
        EQUALS,
        COMMA,
        COLON,
        LESS,
        GREATER,
        ARROW,
        OPEN_BRACE,
        CLOSE_BRACE,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        EOFI,
    };
    typedef enum Tag Tag;
    typedef struct Token Token;
    struct Token {
        Tag tag;
        char* data;
    };
    typedef struct Lexer Lexer;
    struct Lexer {
        size_t index;
        FILE *file;
        Token** tokens;
        size_t buffer_size;
    };
    Lexer *init(FILE* file);
    void lex(Lexer *lexer);
    void print(Lexer *lexer);
    void Free(Lexer *lexer);
    std::string Tag_to_string(Tag tag);
}
#endif