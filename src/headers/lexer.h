#ifndef H_LEXER
#define H_LEXER
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
namespace Lexer
{
  enum Tag
  {
    LITERAL,
    INCREMENT,
    DECREMENT,
    MULTIPLY_ASSIGN,
    DIVIDE_ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    OPEN_PAREN,
    CLOSE_PAREN,
    KEYWORD,
    IDENTIFIER,
    SEMICOLON,
    EQUALS,
    COMMA,
    COLON,
    LESS,
    GREATER,
    ARROW,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    BANG,
    BANG_EQUALS,
    EQUALS_EQUALS,
    LESS_EQUALS,
    GREATER_EQUALS,
    L_AND,
    L_OR,
    AND,
    PIPE,
    EOFI,
  };
  typedef enum Tag Tag;
  typedef struct Token Token;
  struct Token
  {
    Tag tag;
    char *data;
  };
  typedef struct Lexer Lexer;
  struct Lexer
  {
    size_t index;
    FILE *file;
    Token **tokens;
    size_t buffer_size;
  };
  Lexer *init(FILE *file);
  void lex(Lexer *lexer);
  void print(Lexer *lexer);
  void Free(Lexer *lexer);
  std::string Tag_to_string(Tag tag);
}
#endif