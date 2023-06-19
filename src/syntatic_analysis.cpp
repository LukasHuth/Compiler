#include "headers/syntatic_analysis.h"

#define STATE SYNTATIC_ANALYSIS_STATE

bool syntatic_analysis_children(AST *ast, Symbol_table *table);
bool syntatic_analysis_function(AST *ast, STATE *state, Symbol_table *table);
void syntatic_analysis_body(AST *ast, STATE *state, Symbol_table *table);
void syntatic_analysis_while(AST *ast, STATE *state, Symbol_table *table);
void syntatic_analysis_if(AST *ast, STATE *state, Symbol_table *table);

STATE *create_state();

void syntatic_analysis(AST *node)
{
  if (node == NULL)
    return;
  if (node->tag != Ast::NODE)
  {
    std::cout << "ERROR: node is not a NODE (" << Ast::get_tag_name(node->tag) << ")" << std::endl;
    exit(10); // ERROR CODE 10
  }
  // Symbol_table *table = new Symbol_table();
  std::unique_ptr<Symbol_table> table(new Symbol_table());
  bool has_main = false;
  for (size_t i = 0; i < node->children.size(); i++)
  {
    bool is_main = syntatic_analysis_children(node->children[i], table.get());
    if (is_main)
      has_main = true;
  }
  if (!has_main)
  {
    printf("ERROR: no main function\n");
    exit(10); // ERROR CODE 10
  }
  // delete table;
}
bool syntatic_analysis_children(AST *ast, Symbol_table *table)
{
  if (ast == NULL)
    return false;
  if (ast->tag == Ast::IMPORT)
    return false;
  if (ast->tag != Ast::FUNCTION)
  {
    std::cout << "ERROR: node is not a FUNCTION (" << Ast::get_tag_name(ast->tag) << ")" << std::endl;
    exit(11); // ERROR CODE 11
  }
  STATE *state = create_state();
  state->in_function = true;
  table->add_function(new Symbol::Function(ast->data.FUNCTION.name));
  bool is_main = syntatic_analysis_function(ast, state, table);
  if (!state->returned)
  {
    printf("ERROR: function %s does not return\n", ast->data.FUNCTION.name);
    exit(17); // ERROR CODE 17
  }
  table->clear_variables();
  free(state);
  return is_main;
}
STATE *create_state()
{
  STATE *state = (STATE *)calloc(1, sizeof(STATE));
  state->in_function = false;
  state->returned = false;
  state->whilecount = 0;
  state->forcount = 0;
  state->ifcount = 0;
  state->in_else = false;
  return state;
}
bool syntatic_analysis_function(AST *ast, STATE *state, Symbol_table *table)
{
  if (ast == NULL)
    return false;
  if (ast->tag != Ast::FUNCTION)
  {
    printf("ERROR: node is not a FUNCTION\n");
    exit(11); // ERROR CODE 11
  }
  if (ast->data.FUNCTION.name == NULL)
  {
    printf("ERROR: function name is NULL\n");
    exit(12); // ERROR CODE 12
  }
  if (ast->data.FUNCTION.type == NULL)
  {
    printf("ERROR: function type is NULL\n");
    exit(13); // ERROR CODE 13
  }
  if (ast->data.FUNCTION.body == NULL)
  {
    printf("ERROR: function body is NULL\n");
    exit(14); // ERROR CODE 14
  }
  char *function_name = ast->data.FUNCTION.name;
  bool is_main = strcmp(function_name, "main") == 0;
  if (is_main)
  {
    if (strcmp(ast->data.FUNCTION.type->data.TYPE.name, "int") != 0)
    {
      printf("ERROR: main function must return int\n");
      exit(15); // ERROR CODE 15
    }
  }
  AST *body = ast->data.FUNCTION.body;
  if (body->tag != Ast::NODE)
  {
    printf("ERROR: function body is not a NODE\n");
    exit(16); // ERROR CODE 16
  }
  for (size_t i = 0; i < body->children.size(); i++)
    syntatic_analysis_body(body->children[i], state, table);
  return is_main;
}
void syntatic_analysis_body(AST *ast, STATE *state, Symbol_table *table)
{
  // TODO: variable type checking
  if (state->returned)
  {
    printf("ERROR: code after return statement\n");
    exit(17); // ERROR CODE 17
  }
  if (ast == NULL)
    return;
  if (ast->tag == Ast::DECLARATION)
  {
    if (ast->data.VAR_MANIP.ast == NULL)
    {
      printf("ERROR: declaration type is NULL\n");
      exit(12); // ERROR CODE 12
    }
    if (ast->data.VAR_MANIP.name == NULL)
    {
      printf("ERROR: declaration name is NULL\n");
      exit(12); // ERROR CODE 12
    }
    if (table->has_variable(ast->data.VAR_MANIP.name))
    {
      printf("ERROR: variable %s already declared\n", ast->data.VAR_MANIP.name);
      exit(17); // ERROR CODE 17
    }
    Symbol::Variable *variable = new Symbol::Variable(ast->data.VAR_MANIP.ast, ast->data.VAR_MANIP.name);
    table->add_variable(variable);
    return;
  }
  if (ast->tag == Ast::ASSIGN)
  {
    if (ast->data.VAR_MANIP.name == NULL)
    {
      printf("ERROR: assign name is NULL\n");
      exit(12); // ERROR CODE 12
    }
    if (!table->has_variable(ast->data.VAR_MANIP.name))
    {
      printf("ERROR: variable %s not declared\n", ast->data.VAR_MANIP.name);
      exit(17); // ERROR CODE 17
    }
    if (ast->data.VAR_MANIP.ast == NULL)
    {
      printf("ERROR: assign value is NULL\n");
      exit(12); // ERROR CODE 12
    }
    if (!ast->data.VAR_MANIP.ast->has_same_type(table->get_variable(ast->data.VAR_MANIP.name)->type))
    {
      printf("ERROR: variable %s has different type\n", ast->data.VAR_MANIP.name);
      exit(17); // ERROR CODE 17
    }
    return;
  }
  if (ast->tag == Ast::RETURN)
  {
    state->returned = true;
    return;
  }
  if (ast->tag == Ast::IF)
  {
    syntatic_analysis_if(ast, state, table);
    return;
  }
  if (ast->tag == Ast::WHILE)
  {
    syntatic_analysis_while(ast, state, table);
    return;
  }
  if (ast->tag == Ast::FOR)
    return;
  if (ast->tag == Ast::BREAK)
  {
    if (state->whilecount == 0 && state->forcount == 0)
    {
      printf("ERROR: break statement outside of loop\n");
      exit(17); // ERROR CODE 17
    }
    return;
  }
  if (ast->tag == Ast::CONTINUE)
  {
    if (state->whilecount == 0 && state->forcount == 0)
    {
      printf("ERROR: continue statement outside of loop\n");
      exit(17); // ERROR CODE 17
    }
    return;
  }
  if (ast->tag == Ast::CALL)
    return;
  if (ast->tag == Ast::NOOP)
    return;
  if (ast->tag == Ast::NODE)
  {
    for (size_t i = 0; i < ast->children.size(); i++)
      syntatic_analysis_body(ast->children[i], state, table);
    return;
  }
  if (ast->tag == Ast::TYPE)
    return;
  if (ast->tag == Ast::LITERAL)
    return;
  if (ast->tag == Ast::INCREMENT)
    return;
  if (ast->tag == Ast::DECREMENT)
    return;
  if (ast->tag == Ast::MULTIPLY_ASSIGN)
    return;
  if (ast->tag == Ast::DIVIDE_ASSIGN)
    return;
  printf("ERROR: unknown node tag (%s)\n", get_tag_name(ast->tag).c_str());
  if (ast->tag != Ast::NODE)
  {
    printf("ERROR: node is not a NODE\n");
    exit(17); // ERROR CODE 17
  }
}
void syntatic_analysis_while(AST *ast, STATE *state, Symbol_table *table)
{
  if (ast == NULL)
    return;
  if (ast->tag != Ast::WHILE)
  {
    printf("ERROR: node is not a WHILE\n");
    exit(18); // ERROR CODE 18
  }
  if (ast->data.WHILE.condition == NULL)
  {
    printf("ERROR: while condition is NULL\n");
    exit(19); // ERROR CODE 19
  }
  if (ast->data.WHILE.body == NULL)
  {
    printf("ERROR: while body is NULL\n");
    exit(20); // ERROR CODE 20
  }
  AST *body = ast->data.WHILE.body;
  if (body->tag != Ast::NODE)
  {
    printf("ERROR: while body is not a NODE\n");
    exit(21); // ERROR CODE 21
  }
  state->whilecount++;
  for (size_t i = 0; i < body->children.size(); i++)
    syntatic_analysis_body(body->children[i], state, table);
  state->whilecount--;
}
void syntatic_analysis_if(AST *ast, STATE *state, Symbol_table *table)
{
  if (ast == NULL)
    return;
  if (ast->tag != Ast::IF)
  {
    printf("ERROR: node is not a IF\n");
    exit(18); // ERROR CODE 18
  }
  if (ast->data.IF.condition == NULL)
  {
    printf("ERROR: if condition is NULL\n");
    exit(19); // ERROR CODE 19
  }
  if (ast->data.IF.body == NULL)
  {
    printf("ERROR: if body is NULL\n");
    exit(20); // ERROR CODE 20
  }
  AST *body = ast->data.IF.body;
  if (body->tag != Ast::NODE)
  {
    printf("ERROR: if body is not a NODE\n");
    exit(21); // ERROR CODE 21
  }
  state->ifcount++;
  for (size_t i = 0; i < body->children.size(); i++)
  {
    syntatic_analysis_body(body->children[i], state, table);
    state->returned = false;
  }
  state->ifcount--;
  if (ast->data.IF.else_body != NULL)
  {
    AST *else_body = ast->data.IF.else_body;
    if (else_body->tag != Ast::NODE)
    {
      printf("ERROR: else body is not a NODE\n");
      exit(22); // ERROR CODE 22
    }
    for (size_t i = 0; i < else_body->children.size(); i++)
    {
      syntatic_analysis_body(else_body->children[i], state, table);
      state->returned = false;
    }
  }
}
