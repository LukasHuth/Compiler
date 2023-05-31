#include "parser.h"

AST *parse_call(LEXER *lexer, char *name);
AST *parse_body(LEXER *lexer);
AST *parse_expr(LEXER *lexer);
AST *parse_type(LEXER *lexer);
AST *parse_argument(LEXER *lexer);
AST *parse_term(LEXER *lexer);
AST *parse_factor(LEXER *lexer);
AST *parse_call(LEXER *lexer, char *name);
AST *parse_func(LEXER *lexer);

AST* parse(LEXER* lexer)
{
    AST* ast = AST_new_node();
    while (lexer->index < lexer->buffer_size)
    {
        AST* expr = parse_func(lexer);
        AST_add_child(ast, expr);
    }
    return ast;
}

LEXER_TAG peek(LEXER* lexer)
{
    return lexer->tokens[lexer->index]->tag;
}

LEXER_TOKEN *eat(LEXER* lexer, LEXER_TAG tag)
{
    if (peek(lexer) == tag)
    {
        lexer->index++;
        return lexer->tokens[lexer->index - 1];
    }
    else
    {
        printf("Parser(eat): Error: Expected %s, got %s\n", LEXER_TAG_to_string(tag), LEXER_TAG_to_string(peek(lexer)));
        exit(1);
    }
}

AST *parse_func(LEXER *lexer)
{
    eat(lexer, LEXER_KEYWORD);
    char *name = eat(lexer, LEXER_IDENTIFIER)->data;
    eat(lexer, LEXER_OPEN_PAREN);
    AST** arguments = calloc(sizeof(AST*), 0);
    size_t array_size = 0;
    while (peek(lexer) != LEXER_CLOSE_PAREN)
    {
        AST* argument = parse_argument(lexer);
        arguments = realloc(arguments, sizeof(AST*) * (array_size + 1));
        arguments[array_size] = argument;
        array_size++;
        if (peek(lexer) == LEXER_COMMA)
        {
            eat(lexer, LEXER_COMMA);
        }
    }
    eat(lexer, LEXER_CLOSE_PAREN);
    eat(lexer, LEXER_COLON);
    AST *type = parse_type(lexer);
    eat(lexer, LEXER_ARROW);
    AST *body = parse_body(lexer);
    return AST_new_function(name, type, body, arguments, array_size);
}

AST *parse_body(LEXER *lexer)
{
    eat(lexer, LEXER_OPEN_BRACE);
    AST* ast = AST_new_node();
    while (peek(lexer) != LEXER_CLOSE_BRACE)
    {
        AST* expr = parse_expr(lexer);
        AST_add_child(ast, expr);
    }
    eat(lexer, LEXER_CLOSE_BRACE);
    return ast;
}

AST *parse_expr(LEXER *lexer)
{
    if(peek(lexer) == LEXER_IDENTIFIER)
    {
        char* name = eat(lexer, LEXER_IDENTIFIER)->data;
        if(peek(lexer) == LEXER_EQUALS)
        {
            eat(lexer, LEXER_EQUALS);
            AST* value = parse_term(lexer);
            eat(lexer, LEXER_SEMICOLON);
            return AST_new_assign(name, value);
        }
        if(peek(lexer) == LEXER_OPEN_PAREN)
        {
            return parse_call(lexer, name);
        }
        if(peek(lexer) == LEXER_COLON)
        {
            eat(lexer, LEXER_COLON);
            AST* type = parse_type(lexer);
            if(peek(lexer) == LEXER_EQUALS)
            {
                eat(lexer, LEXER_EQUALS);
                AST* value = parse_term(lexer);
                eat(lexer, LEXER_SEMICOLON);
                return AST_new_declaration(name, type, value);
            }
            return AST_new_declaration(name, type, AST_new_number(0));
        }
        if(peek(lexer) == LEXER_OPEN_BRACKET)
        {
            eat(lexer, LEXER_OPEN_BRACKET);
            AST* index = parse_term(lexer);
            eat(lexer, LEXER_CLOSE_BRACKET);
            eat(lexer, LEXER_EQUALS);
            AST* value = parse_term(lexer);
            // return AST_new_index_assign(name, index, value);
            printf("Parser(parse_expr): Error: Index assignment not implemented\n");
            exit(2);
        }
        // TODO: Implement index assignment and add support for -=, +=, *=, /=, etc.
        printf("Parser(parse_expr): Error: Unexpected token %s\n", LEXER_TAG_to_string(peek(lexer)));
    }
    while(peek(lexer) != LEXER_SEMICOLON)
    {
        eat(lexer, peek(lexer));
    }
    eat(lexer, LEXER_SEMICOLON);
}

AST *parse_argument(LEXER *lexer)
{
    char *name = eat(lexer, LEXER_IDENTIFIER)->data;
    eat(lexer, LEXER_COLON);
    AST *type = parse_type(lexer);
    return AST_new_argument(name, type);
}

AST *parse_type(LEXER *lexer)
{
    char *name = eat(lexer, LEXER_KEYWORD)->data;
    if (peek(lexer) == LEXER_OPEN_BRACKET)
    {
        eat(lexer, LEXER_OPEN_BRACKET);
        AST *array_size = parse_term(lexer);
        eat(lexer, LEXER_CLOSE_BRACKET);
        return AST_new_type(name, true, array_size);
    }
    else
    {
        return AST_new_type(name, false, NULL);
    }
}

AST *parse_term(LEXER *lexer)
{
    AST *left = parse_factor(lexer);
    while (peek(lexer) == LEXER_STAR || peek(lexer) == LEXER_SLASH)
    {
        LEXER_TAG tag = peek(lexer);
        eat(lexer, tag);
        AST *right = parse_factor(lexer);
        left = AST_new_tuple(tag == LEXER_STAR ? AST_MUL : AST_DIV, left, right);
    }
    while (peek(lexer) == LEXER_PLUS || peek(lexer) == LEXER_MINUS)
    {
        LEXER_TAG tag = peek(lexer);
        eat(lexer, tag);
        AST *right = parse_factor(lexer);
        left = AST_new_tuple(tag == LEXER_PLUS ? AST_ADD : AST_SUB, left, right);
    }
    return left;
}

AST *parse_factor(LEXER *lexer)
{
    if (peek(lexer) == LEXER_NUMBER)
    {
        int number = atoi(eat(lexer, LEXER_NUMBER)->data);
        return AST_new_number(number);
    }
    if (peek(lexer) == LEXER_OPEN_PAREN)
    {
        eat(lexer, LEXER_OPEN_PAREN);
        AST *expr = parse_term(lexer);
        eat(lexer, LEXER_CLOSE_PAREN);
        eat(lexer, LEXER_SEMICOLON);
        return expr;
    }
    if (peek(lexer) == LEXER_IDENTIFIER)
    {
        char *name = eat(lexer, LEXER_IDENTIFIER)->data;
        if (peek(lexer) == LEXER_OPEN_PAREN)
        {
            return parse_call(lexer, name);
        }
        eat(lexer, LEXER_SEMICOLON);
        return AST_new_declaration(name, NULL, NULL);
    }
    printf("Parser(parse_factor): Error: Unexpected token %s\n", LEXER_TAG_to_string(peek(lexer)));
    exit(1);
}

AST *parse_call(LEXER *lexer, char *name)
{
    eat(lexer, LEXER_OPEN_PAREN);
    AST** arguments = calloc(sizeof(AST*), 0);
    size_t array_size = 0;
    while (peek(lexer) != LEXER_CLOSE_PAREN)
    {
        AST* argument = parse_term(lexer);
        arguments = realloc(arguments, sizeof(AST*) * (array_size + 1));
        arguments[array_size] = argument;
        array_size++;
        if (peek(lexer) == LEXER_COMMA)
        {
            eat(lexer, LEXER_COMMA);
        }
    }
    eat(lexer, LEXER_CLOSE_PAREN);
    eat(lexer, LEXER_SEMICOLON);
    return AST_new_call(name, arguments, array_size);
}