#include "main.h"

int main()
{
    printf("Hello World!(better)\n");
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
    AST *ast = parse(lexer);
    // AST_print(ast);
    AST_print(ast);
    syntatic_analysis(ast);
    AST_free(ast);
    lexer_free(lexer);
    fclose(file);
    return 0;
}