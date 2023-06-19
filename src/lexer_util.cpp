#include "headers/lexer_util.h"
namespace Lexer
{
    char get_char(FILE* file)
    {
        char c = fgetc(file);
        if (c == EOFI)
        {
            printf("Error: Unexpected EOF\n");
            exit(1);
        }
        return c;
    }
    char peek_char(FILE* file)
    {
        char c = fgetc(file);
        ungetc(c, file);
        return c;
    }
    bool is_eof(FILE* file)
    {
        return peek_char(file) == EOF;
    }

    void add_lexer_token(Lexer *lexer, Token* lexer_token)
    {
        lexer->buffer_size++;
        lexer->tokens = (Token**) realloc(lexer->tokens, lexer->buffer_size * sizeof(Token));
        lexer->tokens[lexer->buffer_size - 1] = lexer_token;
    }
    Token* new_token(Tag tag, char* data)
    {
        Token* lexer_token = (Token*) malloc(sizeof(Token));
        lexer_token->tag = tag;
        lexer_token->data = (char*) calloc(strlen(data) + 1, sizeof(char));
        strcpy(lexer_token->data, data);
        return lexer_token;
    }

    bool is_keyword(char* str)
    {
        if (strcmp(str, "int") == 0 ) return true;
        if (strcmp(str, "char") == 0 ) return true;
        if (strcmp(str, "string") == 0 ) return true;
        if (strcmp(str, "bool") == 0 ) return true;
        if (strcmp(str, "true") == 0 ) return true;
        if (strcmp(str, "false") == 0 ) return true;
        if (strcmp(str, "void") == 0 ) return true;
        if(strcmp(str, "float") == 0) return true;
        if (strcmp(str, "func") == 0 ) return true;
        if (strcmp(str, "return") == 0 ) return true;
        if (strcmp(str, "if") == 0 ) return true;
        if (strcmp(str, "for") == 0 ) return true;
        if (strcmp(str, "while") == 0 ) return true;
        return false;
    }
    void print_lexer_token(Token *token)
    {
            Token lexer_token = *token;
            switch (lexer_token.tag)
            {
            case NUMBER:
                printf("%d", atoi(lexer_token.data));
                break;
            case KEYWORD:
                printf("%s (keyword)", lexer_token.data);
                break;
            case IDENTIFIER:
                printf("%s (identifier)", lexer_token.data);
                break;
            case EOFI:
                printf("EOF");
                break;
            default:
                printf("%s", lexer_token.data);
                break;
            }
            printf("\n");
    }
    void print(Lexer *lexer)
    {
        for (size_t i = 0; i < lexer->buffer_size; i++)
        {
            print_lexer_token(lexer->tokens[i]);
        }
    }
}