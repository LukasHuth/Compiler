#include "headers/symbol_table.h"

namespace Symbol {
    Variable::Variable(AST *type, std::string name)
    {
        this->type = type;
        this->name = name;
    }
    Function::Function(std::string name)
    {
        this->name = name;
    }
    void Function::add(AST *attribute)
    {
        this->attributes.push_back(attribute);
    }
}
void Symbol_table::add_variable(Symbol::Variable* variable)
{
    this->variables.push_back(variable);
}
void Symbol_table::add_function(Symbol::Function* function)
{
    this->functions.push_back(function);
}
void Symbol_table::clear_variables()
{
    for(Symbol::Variable* var : this->variables)
    {
        delete var;
    }
    this->variables.clear();
}
Symbol::Variable* Symbol_table::get_variable(std::string name)
{
    for(Symbol::Variable* var : this->variables)
    {
        if(var->name == name) return var;
    }
    return NULL;
}
size_t Symbol_table::var_amount()
{
    return this->variables.size();
}
bool Symbol_table::has_variable(std::string name)
{
    for(Symbol::Variable* var : this->variables)
    {
        if(var->name == name) return true;
    }
    return false;
}
Symbol_table::~Symbol_table()
{
    for(auto var : this->variables)
    {
        delete var;
    }
    for(auto func : this->functions)
    {
        delete func;
    }
}