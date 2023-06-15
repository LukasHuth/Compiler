#ifndef H_AST_TAG
#define H_AST_TAG
#include <stdio.h>
#include <string>
namespace Ast
{
  typedef enum {
    NUMBER,
    BINARY_OP,
    EXPR,
    RETURN,
    ARGUMENT,
    DECLARATION,
    ASSIGN,
    IF,
    WHILE,
    FOR,
    BREAK,
    CONTINUE,
    TYPE,
    FUNCTION,
    IMPORT,
    CALL,
    NODE,
    VARIABLE,
    NOOP,
  } Tag;
  std::string get_tag_name(Tag tag);
  int tag_get_priority(std::string tag);
}
#endif