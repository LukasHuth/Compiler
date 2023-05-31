#ifndef H_AST_TAG
#define H_AST_TAG
#include <stdio.h>
typedef enum {
  AST_NUMBER,
  AST_ADD,
  AST_SUB,
  AST_MUL,
  AST_DIV,
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
  AST_CALL,
  AST_NODE,
} AST_TAG;
char* get_tag_name(AST_TAG tag);
int tag_get_priority(AST_TAG tag);
char get_op(AST_TAG tag);
#endif