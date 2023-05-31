#include "lexer.h"
bool is_keyword(char* str);
bool is_eof(FILE* file);
char peek_char(FILE* file);
char get_char(FILE* file);
LEXER* init_lexer(FILE* file)
{
    LEXER *lexer = malloc(sizeof(LEXER));
    lexer->file = file;
    lexer->buffer = calloc(0, sizeof(LEXER_TOKEN));
    lexer->buffer_size = 0;
    return lexer;
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
void lexer_print_lexer_token(LEXER_TOKEN *token)
{
        LEXER_TOKEN lexer_token = *token;
        switch (lexer_token.tag)
        {
        case LEXER_NUMBER:
            printf("%d", lexer_token.data.LEXER_NUMBER.number);
            break;
        case LEXER_KEYWORD:
            printf("%s (keyword)", lexer_token.data.LEXER_KEYWORD.keyword);
            break;
        case LEXER_IDENTIFIER:
            printf("%s (identifier)", lexer_token.data.LEXER_IDENTIFIER.identifier);
            break;
        case LEXER_EOF:
            printf("EOF");
            break;
        default:
            printf("%s", lexer_token.data.LEXER_DATA.data);
            break;
        }
        printf("\n");
}
void lexer_print(LEXER *lexer)
{
    for (size_t i = 0; i < lexer->buffer_size; i++)
    {
        lexer_print_lexer_token(&lexer->buffer[i]);
    }
}
void lexer_free(LEXER *lexer)
{
    free(lexer->buffer);
    free(lexer);
}
void lex(LEXER *lexer)
{
    while (!is_eof(lexer->file))
    {
        char c = get_char(lexer->file);
        char* str = malloc(sizeof(char) * 2);
        str[0] = c;
        str[1] = '\0';
        size_t str_size = 2;
        LEXER_TAG tag = LEXER_EOF;
        switch (c)
        {
        case ' ':
        case '\n':
        case '\t':
            break;
        case '+':
            tag = LEXER_PLUS;
            break;
        case '<':
            tag = LEXER_LESS;
            break;
        case '>':
            tag = LEXER_GREATER;
            break;
        case '-':
        {
            if(peek_char(lexer->file) == '>') {
                get_char(lexer->file);
                str_size++;
                str = realloc(str, str_size * sizeof(char));
                str[str_size - 1] = '\0';
                str[str_size - 2] = '>';
                tag = LEXER_ARROW;
                break;
            }
            tag = LEXER_MINUS;
            break;
        }
        case '*':
            tag = LEXER_STAR;
            break;
        case '/':
            tag = LEXER_SLASH;
            break;
        case '(':
            tag = LEXER_OPEN_PAREN;
            break;
        case ')':
            tag = LEXER_CLOSE_PAREN;
            break;
        case ';':
            tag = LEXER_SEMICOLON;
            break;
        case '=':
            tag = LEXER_EQUALS;
            break;
        case ',':
            tag = LEXER_COMMA;
            break;
        case ':':
            tag = LEXER_COLON;
            break;
        case '{':
            tag = LEXER_OPEN_BRACE;
            break;
        case '}':
            tag = LEXER_CLOSE_BRACE;
            break;
        default:
        {
            if (isdigit(c))
            {
                while (isdigit(peek_char(lexer->file)))
                {
                    c = get_char(lexer->file);
                    str_size++;
                    str = realloc(str, str_size * sizeof(char));
                    str[str_size - 2] = c;
                    str[str_size - 1] = '\0';
                }
                tag = LEXER_NUMBER;
            }
            else if (isalpha(c))
            {
                while (isalpha(peek_char(lexer->file)))
                {
                    c = get_char(lexer->file);
                    str_size++;
                    str = realloc(str, str_size * sizeof(char));
                    str[str_size - 2] = c;
                    str[str_size - 1] = '\0';
                }
                if (is_keyword(str))
                {
                    tag = LEXER_KEYWORD;
                }
                else
                {
                    tag = LEXER_IDENTIFIER;
                }
            }
            else
            {
                printf("Error: Unexpected character: %c\n", c);
                exit(1);
            }
        }
        break;
        }
        if (tag != LEXER_EOF)
        {
            lexer_add_lexer_token(lexer, lexer_new_token(tag, str));
        }
    }
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
