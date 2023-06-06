#ifndef H_AST
#define H_AST
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "AST_TAG.h"

typedef struct AST AST;
struct AST {
  AST_TAG tag;
  union data {
    struct AST_NUMBER { char* number; } AST_NUMBER;
    struct AST_TUPLE { AST *left; AST *right; char* op; } AST_TUPLE;
    struct AST_EXPR { AST *expr; } AST_EXPR;
    struct AST_RETURN { AST *expr; } AST_RETURN;
    struct AST_ARGUMENT { char* name; AST *type; } AST_ARGUMENT;
    struct AST_FUNCTION { char* name; AST *type; AST *body; AST* *arguments; size_t array_size; } AST_FUNCTION;
    struct AST_NODE { AST* *children; size_t array_size; } AST_NODE;
    struct AST_TYPE { char* name; bool is_array; AST* array_size; } AST_TYPE;
    struct AST_DECLARATION { char* name; AST *type; } AST_DECLARATION;
    struct AST_ASSIGN { char* name; AST *value; } AST_ASSIGN;
    struct AST_IF { AST *condition; AST *body; } AST_IF;
    struct AST_WHILE { AST *condition; AST *body; } AST_WHILE;
    struct AST_FOR { AST *init; AST *condition; AST *increment; AST *body; } AST_FOR;
    struct AST_CALL { char* name; AST* *arguments; size_t array_size; } AST_CALL;
    struct AST_VARIABLE { char* name; bool is_arg; int arg_index; } AST_VARIABLE;
  } data;
};
AST *AST_new();
AST *AST_new_single(AST_TAG tag, AST *expr);
AST *AST_new_number(char* number);
AST *AST_new_tuple(AST_TAG tag, AST *left, AST *right, char* op);
// from here on, the functions are not written at the moment
AST *AST_new_node();
AST *AST_new_expr(AST *expr);
AST *AST_new_return(AST *expr);
AST *AST_new_argument(char* name, AST *type);
AST *AST_new_function(char* name, AST *type, AST *body, AST* *arguments, size_t array_size);
AST *AST_new_type(char* name, bool is_array, AST* array_size);
AST *AST_new_declaration(char* name, AST *type);
AST *AST_new_assign(char* name, AST *value);
AST *AST_new_if(AST *condition, AST *body);
AST *AST_new_while(AST *condition, AST *body);
AST *AST_new_for(AST *init, AST *condition, AST *increment, AST *body);
AST *AST_new_break();
AST *AST_new_noop();
AST *AST_new_continue();
AST *AST_new_call(char* name, AST* *arguments, size_t array_size);
AST *AST_new_variable(char* name, bool is_arg);
void AST_add_argument(AST* ast, AST* argument);
void AST_add_child(AST* ast, AST* child);
// until here
void AST_free(AST *ast);
void AST_print(AST *ast);
#endif