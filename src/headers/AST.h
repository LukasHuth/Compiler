#ifndef H_AST
#define H_AST
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <vector>

#include "AST_TAG.h"

typedef class AST AST;
class AST
{
public:
  Ast::Tag tag;
  std::vector<AST *> children;
  std::vector<AST *> arguments;
  union data
  {
    struct LITERAL
    {
      char *literal;
    } LITERAL;
    struct TUPLE
    {
      AST *left;
      AST *right;
      char *op;
    } TUPLE;
    struct EXPR
    {
      AST *expr;
    } EXPR;
    struct RETURN
    {
      AST *expr;
    } RETURN;
    struct FUNCTION
    {
      char *name;
      AST *type;
      AST *body;
    } FUNCTION;
    struct TYPE
    {
      char *name;
      bool is_array;
      AST *array_size;
    } TYPE;
    struct VAR_MANIP
    {
      char *name;
      AST *ast;
    } VAR_MANIP;
    struct IF
    {
      AST *condition;
      AST *body;
      AST *else_body;
    } IF;
    struct WHILE
    {
      AST *condition;
      AST *body;
    } WHILE;
    struct FOR
    {
      AST *init;
      AST *condition;
      AST *increment;
      AST *body;
    } FOR;
    struct CALL
    {
      char *name;
    } CALL;
    struct VARIABLE
    {
      char *name;
      bool is_arg;
      int arg_index;
    } VARIABLE;
  } data;
  void add_argument(AST *argument) { this->arguments.push_back(argument); }
  void add_child(AST *child) { this->children.push_back(child); }
  void print();
  bool has_same_type(AST *ast);
  AST() {}
  AST(Ast::Tag tag) { this->tag = tag; }
  AST(Ast::Tag tag, char *data);
  AST(Ast::Tag tag, char *data, std::vector<AST *> args);
  AST(Ast::Tag tag, char *data, bool con, int count);
  AST(Ast::Tag tag, AST *expr);
  AST(Ast::Tag tag, char *name, AST *ast);
  AST(Ast::Tag tag, AST *left, AST *right, char *op);
  AST(char *name, AST *type, AST *body, std::vector<AST *> args);
  AST(char *name, bool con, AST *expr_array_size);
  AST(AST *condition, AST *body);
  AST(AST *condition, AST *body, AST *else_body);
  AST(AST *init, AST *condition, AST *increment, AST *body);
  ~AST();
};
#endif