#include "headers/AST.h"

// AST* AST::Number(char* literal) {AST* ast = new AST(Ast::LITERAL);ast->data.LITERAL.literal = literal; return ast;}
const bool DEBUG = false;
AST::AST(Ast::Tag tag, char *data)
{
  this->tag = tag;
  if (tag == Ast::LITERAL)
  {
    this->data.LITERAL.literal = data;
  }
}
AST::AST(Ast::Tag tag, char *data, std::vector<AST *> args)
{
  this->tag = tag;
  if (tag == Ast::CALL)
  {
    this->data.CALL.name = data;
    this->arguments.insert(this->arguments.end(), args.begin(), args.end());
  }
}
AST::AST(Ast::Tag tag, char *data, bool con, int count)
{
  this->tag = tag;
  if (tag == Ast::VARIABLE)
  {
    this->data.VARIABLE.name = data;
    this->data.VARIABLE.is_arg = con;
    this->data.VARIABLE.arg_index = count;
  }
}
AST::AST(Ast::Tag tag, AST *expr)
{
  this->tag = tag;
  this->data.EXPR.expr = expr;
}
AST::AST(Ast::Tag tag, char *name, AST *ast)
{
  this->tag = tag;
  this->data.VAR_MANIP.name = name;
  this->data.VAR_MANIP.ast = ast;
}
AST::AST(Ast::Tag tag, AST *left, AST *right, char *op)
{
  this->tag = tag;
  this->data.TUPLE.left = left;
  this->data.TUPLE.right = right;
  this->data.TUPLE.op = op;
}
AST::AST(char *name, AST *type, AST *body, std::vector<AST *> args)
{
  this->tag = Ast::FUNCTION;
  this->data.FUNCTION.type = type;
  this->data.FUNCTION.body = body;
  this->data.FUNCTION.name = name;
  this->arguments.insert(this->arguments.end(), args.begin(), args.end());
}
AST::AST(char *name, bool con, AST *expr_array_size)
{
  this->tag = Ast::TYPE;
  this->data.TYPE.array_size = expr_array_size;
  this->data.TYPE.is_array = con;
  this->data.TYPE.name = name;
}
AST::AST(AST *condition, AST *body, AST *else_body)
{
  this->tag = Ast::IF;
  this->data.IF.body = body;
  this->data.IF.condition = condition;
  this->data.IF.else_body = else_body;
}
AST::AST(AST *condition, AST *body)
{
  this->tag = Ast::WHILE;
  this->data.IF.body = body;
  this->data.IF.condition = condition;
}
AST::AST(AST *init, AST *condition, AST *increment, AST *body)
{
  this->tag = Ast::FOR;
  this->data.FOR.init = init;
  this->data.FOR.condition = condition;
  this->data.FOR.increment = increment;
  this->data.FOR.body = body;
}
AST::~AST()
{
  switch (this->tag)
  {
  case Ast::RETURN:
    if (DEBUG)
      printf("free return\n");
    delete this->data.RETURN.expr;
    break;
  case Ast::NODE:
    if (DEBUG)
      printf("free node\n");
    break;
  case Ast::FUNCTION:
    if (DEBUG)
      printf("free function\n");
    delete this->data.FUNCTION.type;
    delete this->data.FUNCTION.body;
    break;
  case Ast::CALL:
    if (DEBUG)
      printf("free call\n");
    break;
  case Ast::TYPE:
    if (DEBUG)
      printf("free type\n");
    delete this->data.TYPE.array_size;
    break;
  case Ast::ARGUMENT:
  case Ast::DECLARATION:
  case Ast::ASSIGN:
    if (DEBUG)
      printf("free var_manip\n");
    delete this->data.VAR_MANIP.ast;
    break;
  case Ast::IF:
    if (DEBUG)
      printf("free if\n");
    delete this->data.IF.condition;
    delete this->data.IF.body;
    delete this->data.IF.else_body;
    break;
  case Ast::WHILE:
    if (DEBUG)
      printf("free while\n");
    delete this->data.WHILE.condition;
    delete this->data.WHILE.body;
    break;
  case Ast::FOR:
    if (DEBUG)
      printf("free for\n");
    delete this->data.FOR.init;
    delete this->data.FOR.condition;
    delete this->data.FOR.increment;
    delete this->data.FOR.body;
    break;
  case Ast::BINARY_OP:
    if (DEBUG)
      printf("free tuple\n");
    delete this->data.TUPLE.left;
    delete this->data.TUPLE.right;
    break;
  case Ast::EXPR:
    if (DEBUG)
      printf("free expr\n");
    delete this->data.EXPR.expr;
    break;
  default:
    break;
  }
  while (this->arguments.size() > 0)
  {
    AST *ast = this->arguments.back();
    this->arguments.pop_back();
    delete ast;
  }
  while (this->children.size() > 0)
  {
    AST *ast = this->children.back();
    this->children.pop_back();
    delete ast;
  }
}
void AST::print()
{
  switch (this->tag)
  {
  case Ast::LITERAL:
    printf("%s", this->data.LITERAL.literal);
    break;
  case Ast::BINARY_OP:
    printf("(");
    this->data.TUPLE.left->print();
    printf("%s", this->data.TUPLE.op);
    this->data.TUPLE.right->print();
    printf(")");
    break;
  case Ast::EXPR:
    printf("(");
    this->data.EXPR.expr->print();
    printf(")");
    break;
  case Ast::RETURN:
    printf("return ");
    this->data.RETURN.expr->print();
    break;
  case Ast::DECLARATION:
  case Ast::ARGUMENT:
  case Ast::ASSIGN:
    this->data.VAR_MANIP.ast->print();
    printf(" %s", this->data.VAR_MANIP.name);
    break;
  case Ast::FUNCTION:
    this->data.FUNCTION.type->print();
    printf(" %s(", this->data.FUNCTION.name);
    for (size_t i = 0; i < this->arguments.size(); i++)
    {
      this->arguments[i]->print();
      if (i < this->arguments.size() - 1)
      {
        printf(", ");
      }
    }
    printf(") {\n");
    this->data.FUNCTION.body->print();
    printf("}\n");
    break;
  case Ast::NODE:
    for (size_t i = 0; i < this->children.size(); i++)
    {
      this->children[i]->print();
      printf("\n");
    }
    break;
  case Ast::TYPE:
    printf("%s", this->data.TYPE.name);
    if (this->data.TYPE.is_array)
    {
      printf("[");
      this->data.TYPE.array_size->print();
      printf("]");
    }
    break;
  case Ast::IF:
    printf("if (");
    this->data.IF.condition->print();
    printf(") {\n");
    this->data.IF.body->print();
    printf("}\n");
    if (this->data.IF.else_body != NULL)
    {
      printf("else {\n");
      this->data.IF.else_body->print();
      printf("}\n");
    }
    break;
  case Ast::WHILE:
    printf("while (");
    this->data.WHILE.condition->print();
    printf(") {\n");
    this->data.WHILE.body->print();
    printf("}\n");
    break;
  case Ast::FOR:
    printf("for (");
    this->data.FOR.init->print();
    printf("; ");
    this->data.FOR.condition->print();
    printf("; ");
    this->data.FOR.increment->print();
    printf(") {\n");
    this->data.FOR.body->print();
    printf("}\n");
    break;
  case Ast::BREAK:
    printf("break");
    break;
  case Ast::CONTINUE:
    printf("continue");
    break;
  case Ast::CALL:
    printf("%s(", this->data.CALL.name);
    for (size_t i = 0; i < this->arguments.size(); i++)
    {
      this->arguments[i]->print();
      if (i != this->arguments.size() - 1)
      {
        printf(", ");
      }
    }
    printf(")");
    break;
  case Ast::VARIABLE:
    printf("%s", this->data.VARIABLE.name);
    break;
  case Ast::INCREMENT:
    printf("%s += ", this->data.VAR_MANIP.name);
    this->data.VAR_MANIP.ast->print();
    break;
  default:
    printf("AST(print): Error: Unknown tag (%s)\n", get_tag_name(this->tag).c_str());
    break;
  }
}
bool AST::has_same_type(AST *ast)
{
  // TODO: check if the value has the same type as the type
  return true;
}