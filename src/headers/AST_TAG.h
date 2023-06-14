#ifndef H_AST_TAG
#define H_AST_TAG
#include <stdio.h>
#include <string>
typedef enum {
  AST_NUMBER,
  AST_BINARY_OP,
  AST_EXPR,
  AST_RETURN,
  AST_ARGUMENT,
  AST_DECLARATION,
  AST_ASSIGN,
  AST_IF,
  AST_WHILE,
  AST_FOR,
  AST_BREAK,
  AST_CONTINUE,
  AST_TYPE,
  AST_FUNCTION,
  AST_IMPORT,
  AST_CALL,
  AST_NODE,
  AST_VARIABLE,
  AST_NOOP,
} AST_TAG;
std::string get_tag_name(AST_TAG tag);
int tag_get_priority(std::string tag);
#endif