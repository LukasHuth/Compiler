#include "lexer.h"
#include "lexer_util.h"
LEXER* init_lexer(FILE* file)
{
    LEXER *lexer = malloc(sizeof(LEXER));
    lexer->index = 0;
    lexer->file = file;
    lexer->tokens = calloc(0, sizeof(LEXER_TOKEN));
    lexer->buffer_size = 0;
    return lexer;
}
void lexer_print_lexer_token(LEXER_TOKEN *token)
{
        LEXER_TOKEN lexer_token = *token;
        switch (lexer_token.tag)
        {
        case LEXER_NUMBER:
            printf("%d", atoi(lexer_token.data));
            break;
        case LEXER_KEYWORD:
            printf("%s (keyword)", lexer_token.data);
            break;
        case LEXER_IDENTIFIER:
            printf("%s (identifier)", lexer_token.data);
            break;
        case LEXER_EOF:
            printf("EOF");
            break;
        default:
            printf("%s", lexer_token.data);
            break;
        }
        printf("\n");
}
void lexer_print(LEXER *lexer)
{
    for (size_t i = 0; i < lexer->buffer_size; i++)
    {
        lexer_print_lexer_token(lexer->tokens[i]);
    }
}
void lexer_free(LEXER *lexer)
{
    free(lexer->tokens);
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