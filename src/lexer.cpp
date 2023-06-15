#include "headers/lexer.h"
#include "headers/lexer_util.h"
namespace Lexer
{
    Lexer *init(FILE* file)
    {
        Lexer *lexer = new Lexer();
        lexer->index = 0;
        lexer->file = file;
        lexer->tokens = (Token**) calloc(0, sizeof(Token));
        lexer->buffer_size = 0;
        return lexer;
    }
    void Free(Lexer *lexer)
    {
        for (size_t i = 0; i < lexer->buffer_size; i++)
        {
            free(lexer->tokens[i]->data);
            free(lexer->tokens[i]);
        }
        free(lexer->tokens);
        delete lexer;
    }
    char* parse_next(Lexer *lexer, Tag *tag)
    {
        char c = get_char(lexer->file);
        char* str = (char*) calloc(2, sizeof(char));
        str[0] = c;
        size_t str_size = 2;
        *tag = EOFI;
            
        switch (c)
        {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                break;
            case EOF:
                break;
            case '+':
                *tag = PLUS;
                break;
            case '<':
                *tag = LESS;
                break;
            case '>':
                *tag = GREATER;
                break;
            case '-':
            {
                if(peek_char(lexer->file) == '>') {
                    get_char(lexer->file);
                    str_size++;
                    str = (char*) realloc(str, str_size * sizeof(char));
                    str[str_size - 1] = '\0';
                    str[str_size - 2] = '>';
                    *tag = ARROW;
                    break;
                }
                *tag = MINUS;
                break;
            }
            case '*':
                *tag = STAR;
                break;
            case '/':
                if(peek_char(lexer->file) == '/') {
                    while(get_char(lexer->file) != '\n');
                    return parse_next(lexer, tag);
                    break;
                }
                *tag = SLASH;
                break;
            case '(':
                *tag = OPEN_PAREN;
                break;
            case ')':
                *tag = CLOSE_PAREN;
                break;
            case ';':
                *tag = SEMICOLON;
                break;
            case '=':
                *tag = EQUALS;
                break;
            case ',':
                *tag = COMMA;
                break;
            case ':':
                *tag = COLON;
                break;
            case '{':
                *tag = OPEN_BRACE;
                break;
            case '}':
                *tag = CLOSE_BRACE;
                break;
            case '[':
                *tag = OPEN_BRACKET;
                break;
            case ']':
                *tag = CLOSE_BRACKET;
                break;
            default:
            {
                if (isdigit(c))
                {
                    while (isdigit(peek_char(lexer->file)) || peek_char(lexer->file) == '.')
                    {
                        c = get_char(lexer->file);
                        str_size++;
                        str = (char*) realloc(str, str_size * sizeof(char));
                        str[str_size - 2] = c;
                        str[str_size - 1] = '\0';
                    }
                    *tag = NUMBER;
                }
                else if (isalpha(c))
                {
                    while (isalpha(peek_char(lexer->file)) || isdigit(peek_char(lexer->file)) || peek_char(lexer->file) == '_')
                    {
                        c = get_char(lexer->file);
                        str_size++;
                        str = (char*) realloc(str, str_size * sizeof(char));
                        str[str_size - 2] = c;
                        str[str_size - 1] = '\0';
                    }
                    if (is_keyword(str))
                    {
                        *tag = KEYWORD;
                    }
                    else
                    {
                        *tag = IDENTIFIER;
                    }
                }
                else
                {
                    printf("Error: Unexpected character: %c %d\n", c, c);
                    exit(1);
                }
            }
            break;
        }
        return str;
    }
    void lex(Lexer *lexer)
    {
        while (!is_eof(lexer->file))
        {
            Tag tag = COMMA;
            char* str = parse_next(lexer, &tag);
            if (tag != EOFI)
            {
                add_lexer_token(lexer, new_token(tag, str));
            }
            free(str);
        }
    }
    std::string Tag_to_string(Tag tag)
    {
        switch (tag)
        {
        case NUMBER:
            return "NUMBER";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case STAR:
            return "STAR";
        case SLASH:    
            return "SLASH";
        case OPEN_PAREN:
            return "OPEN_PAREN";
        case CLOSE_PAREN:
            return "CLOSE_PAREN";
        case KEYWORD:
            return "KEYWORD";
        case IDENTIFIER:
            return "IDENTIFIER";
        case SEMICOLON:
            return "SEMICOLON";
        case EQUALS:
            return "EQUALS";
        case COMMA:
            return "COMMA";
        case COLON:
            return "COLON";
        case LESS:
            return "LESS";
        case GREATER:
            return "GREATER";
        case ARROW:
            return "ARROW";
        case OPEN_BRACE:
            return "OPEN_BRACE";
        case CLOSE_BRACE:
            return "CLOSE_BRACE";
        case OPEN_BRACKET:
            return "OPEN_BRACKET";
        case CLOSE_BRACKET:
            return "CLOSE_BRACKET";
        case EOFI:
            return "EOF";
        default:
            printf("Error: Unknown tag\n");
            exit(1);
        }
    }
}