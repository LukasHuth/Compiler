#include "headers/main.h"

int main()
{
  const char *filename = "test.txt";
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Could not open file %s\n", filename);
    return 1;
  }
  Lexer::Lexer *lexer = Lexer::init(file);
  Lexer::lex(lexer);
  AST *ast = Parser::parse(lexer);
  syntatic_analysis(ast);
  // ast->print();
  Codegen::init(ast);
  // std::cout << "Codegen done" << std::endl;
  Lexer::Free(lexer);
  fclose(file);
  return 0;
}