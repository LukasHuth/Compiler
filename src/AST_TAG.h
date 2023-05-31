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
} AST_TAG;
char* get_tag_name(AST_TAG tag);
char get_op(AST_TAG tag);
#endif