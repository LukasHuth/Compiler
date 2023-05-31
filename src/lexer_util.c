#include "lexer_util.h"

char get_char(FILE* file)
{
    char c = fgetc(file);
    if (c == EOF)
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

void lexer_add_lexer_token(LEXER *lexer, LEXER_TOKEN* lexer_token)
{
    lexer->buffer_size++;
    lexer->buffer = realloc(lexer->buffer, lexer->buffer_size * sizeof(LEXER_TOKEN));
    lexer->buffer[lexer->buffer_size - 1] = *lexer_token;
}
LEXER_TOKEN* lexer_new_token(LEXER_TAG tag, char* data)
{
    LEXER_TOKEN* lexer_token = malloc(sizeof(LEXER_TOKEN));
    lexer_token->tag = tag;
    switch(tag) {
        case LEXER_NUMBER:
            lexer_token->data.LEXER_NUMBER.number = atoi(data);
            break;
        case LEXER_KEYWORD:
            lexer_token->data.LEXER_KEYWORD.keyword = data;
            break;
        case LEXER_IDENTIFIER:
            lexer_token->data.LEXER_IDENTIFIER.identifier = data;
            break;
        default:
            lexer_token->data.LEXER_DATA.data = data;
            break;
    }
    return lexer_token;
}

bool is_keyword(char* str)
{
    if (strcmp(str, "int") == 0 ) return true;
    if (strcmp(str, "func") == 0 ) return true;
    if (strcmp(str, "return") == 0 ) return true;
    if (strcmp(str, "if") == 0 ) return true;
    if (strcmp(str, "for") == 0 ) return true;
    if (strcmp(str, "while") == 0 ) return true;
    return false;
}