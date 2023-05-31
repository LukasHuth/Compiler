#include "main.h"

int main()
{
    printf("Hello World!(better)\n");
    AST* ast;
    ast = AST_new_tuple(AST_ADD ,AST_new_number(1), AST_new_number(2));
    ast = AST_new_tuple(AST_MUL ,ast, AST_new_number(3));
    ast = AST_new_tuple(AST_ADD ,ast, AST_new_number(4));
    ast = AST_new_single(AST_EXPR, ast);
    ast = AST_new_single(AST_RETURN, ast);
    AST_print(ast);
    printf("\n");
    AST_free(ast);
    const char* filename = "test.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return 1;
    }
    LEXER *lexer = init_lexer(file);
    lex(lexer);
    lexer_print(lexer);
    lexer_free(lexer);
    return 0;
}