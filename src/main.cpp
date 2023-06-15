#include "headers/main.h"

int main()
{
    // printf("Hello World!(better)\n");
    const char* filename = "test.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return 1;
    }
    Lexer::Lexer *lexer = Lexer::init(file);
    Lexer::lex(lexer);
    // printf("Hallo\n");
    // lexer_print(lexer);
    AST *ast = Parser::parse(lexer);
    // AST_print(ast);
    syntatic_analysis(ast);
    Codegen::init(ast);
    // codegen_generate(codegen);
    // free everything
    Lexer::Free(lexer);
    fclose(file);
    return 0;
}