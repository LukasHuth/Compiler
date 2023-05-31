#include "parser.h"

AST* parse_expr(LEXER* lexer);

AST* parse(LEXER* lexer)
{
    AST* ast = AST_new_node();
    while (lexer->index < lexer->buffer_size)
    {
        AST* expr = parse_expr(lexer);
        AST_add_child(ast, expr);
    }
    return ast;
}

AST *parse_expr(LEXER *lexer)
{
    LEXER_TOKEN *token = lexer->tokens[lexer->index];
    if (token->tag == LEXER_NUMBER)
    {
        lexer->index++;
        return AST_new_number(atoi(token->data));
    }
    else if (token->tag == LEXER_OPEN_PAREN)
    {
        lexer->index++;
        AST *expr = parse_expr(lexer);
        if (lexer->tokens[lexer->index]->tag != LEXER_CLOSE_PAREN)
        {
            printf("Parser(parse_expr): Error: Expected ')'\n");
            exit(1);
        }
        lexer->index++;
        return AST_new_single(AST_EXPR, expr);
    }
    else
    {
        printf("Parser(parse_expr): Error: Expected number or '('\n");
        exit(1);
    }
}