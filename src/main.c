#include "headers/main.h"

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
    AST_print(ast);
    syntatic_analysis(ast);
    FILE* output_file = fopen("output.ll", "w");
    CODEGEN *codegen = init_codegen(output_file, ast);
    codegen_generate(codegen);
    // free everything
    codegen_free(codegen);
    AST_free(ast);
    lexer_free(lexer);
    fclose(file);
    return 0;
}