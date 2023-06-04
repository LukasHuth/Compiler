#include "headers/syntatic_analysis.h"

#define STATE SYNTATIC_ANALYSIS_STATE

void syntatic_analysis_children(AST *ast, SYMBOL_TABLE *table);
void syntatic_analysis_function(AST *ast, STATE *state, VARIABLE_ARRAY *variables);
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
    if(node->tag != AST_NODE)
    {
        printf("ERROR: node is not a AST_NODE\n");
        exit(10); // ERROR CODE 10
    }
    SYMBOL_TABLE *table = create_symbol_table();
    for(size_t i = 0; i < node->data.AST_NODE.array_size; i++)
        syntatic_analysis_children(node->data.AST_NODE.children[i], table);
    free_symbol_table(table);
}
void syntatic_analysis_children(AST *ast, SYMBOL_TABLE *table)
{
    if(ast == NULL)
        return;
    if(ast->tag == AST_IMPORT)
        return;
    if(ast->tag != AST_FUNCTION)
    {
        printf("ERROR: node is not a AST_FUNCTION\n");
        exit(11); // ERROR CODE 11
    }
    STATE *state = create_state();
    state->in_function = true;
    VARIABLE_ARRAY *variables = create_variable_array();
    table->functions->functions = realloc(table->functions->functions, (table->functions->size + 1) * sizeof(FUNC*));
    table->functions->functions[table->functions->size] = create_function(ast->data.AST_FUNCTION.name);
    table->functions->size++;
    table->variables = variables;
    syntatic_analysis_function(ast, state, variables);
    free_variable_array(variables);
    free(state);
}
STATE *create_state()
{
    STATE *state = calloc(1, sizeof(STATE));
    state->in_function = false;
    state->returned = false;
    state->whilecount = 0;
    state->forcount = 0;
    state->ifcount = 0;
    state->in_else = false;
    return state;
}
void syntatic_analysis_function(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    if(ast == NULL)
        return;
    if(ast->tag != AST_FUNCTION)
    {
        printf("ERROR: node is not a AST_FUNCTION\n");
        exit(11); // ERROR CODE 11
    }
    if(ast->data.AST_FUNCTION.name == NULL)
    {
        printf("ERROR: function name is NULL\n");
        exit(12); // ERROR CODE 12
    }
    if(ast->data.AST_FUNCTION.type == NULL)
    {
        printf("ERROR: function type is NULL\n");
        exit(13); // ERROR CODE 13
    }
    if(ast->data.AST_FUNCTION.body == NULL)
    {
        printf("ERROR: function body is NULL\n");
        exit(14); // ERROR CODE 14
    }
    if(ast->data.AST_FUNCTION.arguments == NULL)
    {
        printf("ERROR: function arguments is NULL\n");
        exit(15); // ERROR CODE 15
    }
    AST *body = ast->data.AST_FUNCTION.body;
    if(body->tag != AST_NODE)
    {
        printf("ERROR: function body is not a AST_NODE\n");
        exit(16); // ERROR CODE 16
    }
    for(size_t i = 0; i < body->data.AST_NODE.array_size; i++)
        syntatic_analysis_body(body->data.AST_NODE.children[i], state, variables);
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
    if(ast->tag == AST_DECLARATION)
    {
        if(ast->data.AST_DECLARATION.type == NULL)
        {
            printf("ERROR: declaration type is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(ast->data.AST_DECLARATION.name == NULL)
        {
            printf("ERROR: declaration name is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(variable_array_has_variable(variables, ast->data.AST_DECLARATION.name))
        {
            printf("ERROR: variable %s already declared\n", ast->data.AST_DECLARATION.name);
            exit(17); // ERROR CODE 17
        }
        VARIABLE *variable = create_variable(ast->data.AST_DECLARATION.type, ast->data.AST_DECLARATION.name);
        // add_variable(variables, create_variable(ast->data.AST_DECLARATION.type, ast->data.AST_DECLARATION.name));
        variable_array_add_variable(variables, variable);
        return;
    }
    if(ast->tag == AST_ASSIGN)
    {
        if(ast->data.AST_ASSIGN.name == NULL)
        {
            printf("ERROR: assign name is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(!variable_array_has_variable(variables, ast->data.AST_ASSIGN.name))
        {
            printf("ERROR: variable %s not declared\n", ast->data.AST_ASSIGN.name);
            exit(17); // ERROR CODE 17
        }
        if(ast->data.AST_ASSIGN.value == NULL)
        {
            printf("ERROR: assign value is NULL\n");
            exit(12); // ERROR CODE 12
        }
        if(!has_same_type(ast->data.AST_ASSIGN.value, variable_array_get_variable(variables, ast->data.AST_ASSIGN.name)->type))
        {
            printf("ERROR: variable %s has different type\n", ast->data.AST_ASSIGN.name);
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == AST_RETURN)
    {
        state->returned = true;
        return;
    }
    if(ast->tag == AST_IF)
    {
        syntatic_analysis_if(ast, state, variables);
        return;
    }
    if(ast->tag == AST_WHILE)
    {
        syntatic_analysis_while(ast, state, variables);
        return;
    }
    if(ast->tag == AST_FOR)
        return;
    if(ast->tag == AST_BREAK)
    {
        if(state->whilecount == 0 && state->forcount == 0)
        {
            printf("ERROR: break statement outside of loop\n");
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == AST_CONTINUE)
    {
        if(state->whilecount == 0 && state->forcount == 0)
        {
            printf("ERROR: continue statement outside of loop\n");
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == AST_CALL)
        return;
    if(ast->tag != AST_NODE)
    {
        printf("ERROR: node is not a AST_NODE\n");
        exit(17); // ERROR CODE 17
    }
}
void syntatic_analysis_while(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    if(ast == NULL)
        return;
    if(ast->tag != AST_WHILE)
    {
        printf("ERROR: node is not a AST_WHILE\n");
        exit(18); // ERROR CODE 18
    }
    if(ast->data.AST_WHILE.condition == NULL)
    {
        printf("ERROR: while condition is NULL\n");
        exit(19); // ERROR CODE 19
    }
    if(ast->data.AST_WHILE.body == NULL)
    {
        printf("ERROR: while body is NULL\n");
        exit(20); // ERROR CODE 20
    }
    AST *body = ast->data.AST_WHILE.body;
    if(body->tag != AST_NODE)
    {
        printf("ERROR: while body is not a AST_NODE\n");
        exit(21); // ERROR CODE 21
    }
    state->whilecount++;
    for(size_t i = 0; i < body->data.AST_NODE.array_size; i++)
        syntatic_analysis_body(body->data.AST_NODE.children[i], state, variables);
    state->whilecount--;
}
void syntatic_analysis_if(AST *ast, STATE *state, VARIABLE_ARRAY *variables)
{
    if(ast == NULL)
        return;
    if(ast->tag != AST_IF)
    {
        printf("ERROR: node is not a AST_IF\n");
        exit(18); // ERROR CODE 18
    }
    if(ast->data.AST_IF.condition == NULL)
    {
        printf("ERROR: if condition is NULL\n");
        exit(19); // ERROR CODE 19
    }
    if(ast->data.AST_IF.body == NULL)
    {
        printf("ERROR: if body is NULL\n");
        exit(20); // ERROR CODE 20
    }
    AST *body = ast->data.AST_IF.body;
    if(body->tag != AST_NODE)
    {
        printf("ERROR: if body is not a AST_NODE\n");
        exit(21); // ERROR CODE 21
    }
    state->ifcount++;
    for(size_t i = 0; i < body->data.AST_NODE.array_size; i++)
    {
        syntatic_analysis_body(body->data.AST_NODE.children[i], state, variables);
        state->returned = false;
    }
    state->ifcount--;
    // add else body option
    /*
    if(ast->data.AST_IF.else_body != NULL)
    {
        AST *else_body = ast->data.AST_IF.else_body;
        if(else_body->tag != AST_NODE)
        {
            printf("ERROR: else body is not a AST_NODE\n");
            exit(22); // ERROR CODE 22
        }
        for(size_t i = 0; i < else_body->data.AST_NODE.array_size; i++)
            syntatic_analysis_if_body(else_body->data.AST_NODE.children[i], state);
    }
    */
}
