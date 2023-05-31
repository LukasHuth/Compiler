#ifndef H_AST
#define H_AST
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "AST_TAG.h"

typedef struct AST AST;
struct AST {
  AST_TAG tag;
  union {
    struct AST_NUMBER { int number; } AST_NUMBER;
    struct AST_TUPLE { AST *left; AST *right; } AST_TUPLE;
    struct AST_EXPR { AST *expr; } AST_EXPR;
    struct AST_RETURN { AST *expr; } AST_RETURN;
  } data;
};
AST *AST_new();
AST *AST_new_single(AST_TAG tag, AST *expr);
AST *AST_new_number(int number);
AST *AST_new_tuple(AST_TAG tag, AST *left, AST *right);
void AST_free(AST *ast);
void AST_print(AST *ast);
#endif