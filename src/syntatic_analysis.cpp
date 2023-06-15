#include "headers/syntatic_analysis.h"

#define STATE SYNTATIC_ANALYSIS_STATE

bool syntatic_analysis_children(AST *ast, SYMBOL_TABLE *table);
bool syntatic_analysis_function(AST *ast, STATE *state, VARIABLE_ARRAY *variables);
void syntatic_analysis_body(AST *ast, STATE *state, VARIABLE_ARRAY *variables);
void syntatic_analysis_while(AST *ast, STATE *state, VARIABLE_ARRAY *variables);
void syntatic_analysis_if(AST *ast, STATE *state, VARIABLE_ARRAY *variables);

void variable_array_add_variable(VARIABLE_ARRAY *array, VARIABLE *variable);
VARIABLE *variable_array_get_variable(VARIABLE_ARRAY *array, char *name);
bool variable_array_has_variable(VARIABLE_ARRAY *array, char *name);
VARIABLE_ARRAY *create_variable_array();
VARIABLE *create_variable(AST *type, char *name);
STATE *create_state();
void free_variable(VARIABLE *variable);
void free_variable_array(VARIABLE_ARRAY *array);
bool has_same_type(AST *value, AST *type);

void syntatic_analysis(AST *node)
{
    if(node == NULL)
        return;
    if(node->tag != Ast::NODE)
    {
        printf("ERROR: node is not a NODE\n");
        exit(10); // ERROR CODE 10
    }
    SYMBOL_TABLE *table = create_symbol_table();
    bool has_main = false;
    for(size_t i = 0; i < node->children.size(); i++)
    {
        bool is_main = syntatic_analysis_children(node->children[i], table);
        if(is_main)
            has_main = true;
    }
    if(!has_main)
    {
        printf("ERROR: no main function\n");
        exit(10); // ERROR CODE 10
    }
    free_symbol_table(table);
}
bool syntatic_analysis_children(AST *ast, SYMBOL_TABLE *table)
{
    if(ast == NULL)
        return false;
    if(ast->tag == Ast::IMPORT)
        return false;
    if(ast->tag != Ast::FUNCTION)
    {
        printf("ERROR: node is not a FUNCTION\n");
        exit(11); // ERROR CODE 11
    }
    STATE *state = create_state();
    state->in_function = true;
    VARIABLE_ARRAY *variables = create_variable_array();
    table->functions->functions = (FUNC**) realloc(table->functions->functions, (table->functions->size + 1) * sizeof(FUNC*));
    table->functions->functions[table->functions->size] = create_function(ast->data.FUNCTION.name);
    table->functions->size++;
    table->variables = variables;
    bool is_main = syntatic_analysis_function(ast, state, variables);
    if(!state->returned)
    {
        printf("ERROR: function %s does not return\n", ast->data.FUNCTION.name);
        exit(17); // ERROR CODE 17
    }
    free_variable_array(variables);
    free(state);
    return is_main;
}
STATE *create_state()
{
    STATE *state = (STATE*) calloc(1, sizeof(STATE));
    state->in_function = false;
    state->returned = false;
    state->whilecount = 0;
    state->forcount = 0;
    state->ifcount = 0;
    state->in_else = false;
    return state;
}
bool syntatic_analysis_function(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    if(ast == NULL)
        return false;
    if(ast->tag != Ast::FUNCTION)
    {
        printf("ERROR: node is not a FUNCTION\n");
        exit(11); // ERROR CODE 11
    }
    if(ast->data.FUNCTION.name == NULL)
    {
        printf("ERROR: function name is NULL\n");
        exit(12); // ERROR CODE 12
    }
    if(ast->data.FUNCTION.type == NULL)
    {
        printf("ERROR: function type is NULL\n");
        exit(13); // ERROR CODE 13
    }
    if(ast->data.FUNCTION.body == NULL)
    {
        printf("ERROR: function body is NULL\n");
        exit(14); // ERROR CODE 14
    }
    char* function_name = ast->data.FUNCTION.name;
    bool is_main = strcmp(function_name, "main") == 0;
    if(is_main)
    {
        if(strcmp(ast->data.FUNCTION.type->data.TYPE.name, "int") != 0)
        {
            printf("ERROR: main function must return int\n");
            exit(15); // ERROR CODE 15
        }
    }
    AST *body = ast->data.FUNCTION.body;
    if(body->tag != Ast::NODE)
    {
        printf("ERROR: function body is not a NODE\n");
        exit(16); // ERROR CODE 16
    }
    for(size_t i = 0; i < body->children.size(); i++)
        syntatic_analysis_body(body->children[i], state, variables);
    return is_main;
}
void syntatic_analysis_body(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    // TODO: variable type checking
    if(state->returned)
    {
        printf("ERROR: code after return statement\n");
        exit(17); // ERROR CODE 17
    }
    if(ast == NULL)
        return;
    if(ast->tag == Ast::DECLARATION)
    {
        if(ast->data.DECLARATION.type == NULL)
        {
            printf("ERROR: declaration type is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(ast->data.DECLARATION.name == NULL)
        {
            printf("ERROR: declaration name is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(variable_array_has_variable(variables, ast->data.DECLARATION.name))
        {
            printf("ERROR: variable %s already declared\n", ast->data.DECLARATION.name);
            exit(17); // ERROR CODE 17
        }
        VARIABLE *variable = create_variable(ast->data.DECLARATION.type, ast->data.DECLARATION.name);
        // add_variable(variables, create_variable(ast->data.DECLARATION.type, ast->data.DECLARATION.name));
        variable_array_add_variable(variables, variable);
        return;
    }
    if(ast->tag == Ast::ASSIGN)
    {
        if(ast->data.ASSIGN.name == NULL)
        {
            printf("ERROR: assign name is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(!variable_array_has_variable(variables, ast->data.ASSIGN.name))
        {
            printf("ERROR: variable %s not declared\n", ast->data.ASSIGN.name);
            exit(17); // ERROR CODE 17
        }
        if(ast->data.ASSIGN.value == NULL)
        {
            printf("ERROR: assign value is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(!has_same_type(ast->data.ASSIGN.value, variable_array_get_variable(variables, ast->data.ASSIGN.name)->type))
        {
            printf("ERROR: variable %s has different type\n", ast->data.ASSIGN.name);
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == Ast::RETURN)
    {
        state->returned = true;
        return;
    }
    if(ast->tag == Ast::IF)
    {
        syntatic_analysis_if(ast, state, variables);
        return;
    }
    if(ast->tag == Ast::WHILE)
    {
        syntatic_analysis_while(ast, state, variables);
        return;
    }
    if(ast->tag == Ast::FOR)
        return;
    if(ast->tag == Ast::BREAK)
    {
        if(state->whilecount == 0 && state->forcount == 0)
        {
            printf("ERROR: break statement outside of loop\n");
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == Ast::CONTINUE)
    {
        if(state->whilecount == 0 && state->forcount == 0)
        {
            printf("ERROR: continue statement outside of loop\n");
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == Ast::CALL)
        return;
    if(ast->tag == Ast::NOOP)
        return;
    printf("ERROR: unknown node tag (%s)\n", get_tag_name(ast->tag).c_str());
    if(ast->tag != Ast::NODE)
    {
        printf("ERROR: node is not a NODE\n");
        exit(17); // ERROR CODE 17
    }
}
void syntatic_analysis_while(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    if(ast == NULL)
        return;
    if(ast->tag != Ast::WHILE)
    {
        printf("ERROR: node is not a WHILE\n");
        exit(18); // ERROR CODE 18
    }
    if(ast->data.WHILE.condition == NULL)
    {
        printf("ERROR: while condition is NULL\n");
        exit(19); // ERROR CODE 19
    }
    if(ast->data.WHILE.body == NULL)
    {
        printf("ERROR: while body is NULL\n");
        exit(20); // ERROR CODE 20
    }
    AST *body = ast->data.WHILE.body;
    if(body->tag != Ast::NODE)
    {
        printf("ERROR: while body is not a NODE\n");
        exit(21); // ERROR CODE 21
    }
    state->whilecount++;
    for(size_t i = 0; i < body->children.size(); i++)
        syntatic_analysis_body(body->children[i], state, variables);
    state->whilecount--;
}
void syntatic_analysis_if(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    if(ast == NULL)
        return;
    if(ast->tag != Ast::IF)
    {
        printf("ERROR: node is not a IF\n");
        exit(18); // ERROR CODE 18
    }
    if(ast->data.IF.condition == NULL)
    {
        printf("ERROR: if condition is NULL\n");
        exit(19); // ERROR CODE 19
    }
    if(ast->data.IF.body == NULL)
    {
        printf("ERROR: if body is NULL\n");
        exit(20); // ERROR CODE 20
    }
    AST *body = ast->data.IF.body;
    if(body->tag != Ast::NODE)
    {
        printf("ERROR: if body is not a NODE\n");
        exit(21); // ERROR CODE 21
    }
    state->ifcount++;
    for(size_t i = 0; i < body->children.size(); i++)
    {
        syntatic_analysis_body(body->children[i], state, variables);
        state->returned = false;
    }
    state->ifcount--;
    // add else body option
    /*
    if(ast->data.IF.else_body != NULL)
    {
        AST *else_body = ast->data.IF.else_body;
        if(else_body->tag != NODE)
        {
            printf("ERROR: else body is not a NODE\n");
            exit(22); // ERROR CODE 22
        }
        for(size_t i = 0; i < else_body->data.NODE.array_size; i++)
            syntatic_analysis_if_body(else_body->data.NODE.children[i], state);
    }
    */
}
