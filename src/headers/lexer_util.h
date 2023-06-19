#ifndef H_LEXER_UTIL
#define H_LEXER_UTIL
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
namespace Lexer
{
  bool is_keyword(char *str);
  bool is_eof(FILE *file);
  char peek_char(FILE *file);
  char get_char(FILE *file);
  void add_lexer_token(Lexer *lexer, Token *lexer_token);
  Token *new_token(Tag tag, char *data);
  void print(Lexer *lexer);
}
#endif