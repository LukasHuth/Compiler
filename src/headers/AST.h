#ifndef H_AST
#define H_AST
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <vector>

#include "AST_TAG.h"

typedef struct AST AST;
struct AST {
  Ast::Tag tag;
  std::vector<AST*> children;
  std::vector<AST*> arguments;
  union data {
    struct NUMBER { char* number; } NUMBER;
    struct TUPLE { AST *left; AST *right; char* op; } TUPLE;
    struct EXPR { AST *expr; } EXPR;
    struct RETURN { AST *expr; } RETURN;
    struct ARGUMENT { char* name; AST *type; } ARGUMENT;
    struct FUNCTION { char* name; AST *type; AST *body; } FUNCTION;
    struct TYPE { char* name; bool is_array; AST* array_size; } TYPE;
    struct DECLARATION { char* name; AST *type; } DECLARATION;
    struct ASSIGN { char* name; AST *value; } ASSIGN;
    struct IF { AST *condition; AST *body; } IF;
    struct WHILE { AST *condition; AST *body; } WHILE;
    struct FOR { AST *init; AST *condition; AST *increment; AST *body; } FOR;
    struct CALL { char* name; } CALL;
    struct VARIABLE { char* name; bool is_arg; int arg_index; } VARIABLE;
  } data;
};

namespace Ast
{
  AST *new_single(Tag tag, AST *expr);
  AST *new_number(char* number);
  AST *new_tuple(Tag tag, AST *left, AST *right, char* op);
  // from here on, the functions are not written at the moment
  AST *new_node();
  AST *new_expr(AST *expr);
  AST *new_return(AST *expr);
  AST *new_argument(char* name, AST *type);
  AST *new_function(char* name, AST *type, AST *body, std::vector<AST*> arguments);
  AST *new_type(char* name, bool is_array, AST* array_size);
  AST *new_declaration(char* name, AST *type);
  AST *new_assign(char* name, AST *value);
  AST *new_if(AST *condition, AST *body);
  AST *new_while(AST *condition, AST *body);
  AST *new_for(AST *init, AST *condition, AST *increment, AST *body);
  AST *new_break();
  AST *new_noop();
  AST *new_continue();
  AST *new_call(char* name, std::vector<AST*> arguments);
  AST *new_variable(char* name, bool is_arg, int arg_index);
  void add_argument(AST* ast, AST* argument);
  void add_child(AST* ast, AST* child);
  // until here
  void Free(AST *ast);
  void print(AST *ast);
}
#endif