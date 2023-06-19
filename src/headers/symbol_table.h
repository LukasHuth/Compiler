#ifndef H_SYMBOL_TABLE
#define H_SYMBOL_TABLE

#include "AST.h"
// TODO: finish conversion

namespace Symbol
{
  class Variable
  {
  public:
    AST *type;
    std::string name;
    Variable(AST *type, std::string name);
  };

  class Function
  {
  public:
    std::vector<AST *> attributes;
    std::string name;
    void add(AST *attribute);
    Function(std::string name);
  };

}

class Symbol_table
{
public:
  std::vector<Symbol::Function *> functions;
  std::vector<Symbol::Variable *> variables;
  void add_variable(Symbol::Variable *variable);
  void add_function(Symbol::Function *function);
  void clear_variables();
  Symbol::Variable *get_variable(std::string name);
  size_t var_amount();
  bool has_variable(std::string name);
  ~Symbol_table();
};
// TODO: Add this to AST
bool has_same_type(AST *value, AST *type);

#endif