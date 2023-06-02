#include "headers/symbol_table.h"

void variable_array_add_variable(VARIABLE_ARRAY *array, VARIABLE *variable)
{
    if(array == NULL)
    {
        printf("ERROR: array is NULL\n");
        exit(23); // ERROR CODE 23
    }
    if(variable == NULL)
    {
        printf("ERROR: variable is NULL\n");
        exit(24); // ERROR CODE 24
    }
    if(array->variables == NULL)
    {
        array->variables = calloc(1, sizeof(VARIABLE*));
        array->size = 1;
        array->variables[0] = variable;
        return;
    }
    array->size++;
    array->variables = realloc(array->variables, sizeof(VARIABLE*) * array->size);
    array->variables[array->size - 1] = variable;
}

VARIABLE *variable_array_get_variable(VARIABLE_ARRAY *array, char *name)
{
    if(array == NULL)
    {
        printf("ERROR: array is NULL\n");
        exit(25); // ERROR CODE 25
    }
    if(name == NULL)
    {
        printf("ERROR: name is NULL\n");
        exit(26); // ERROR CODE 26
    }
    if(array->variables == NULL)
        return NULL;
    for(size_t i = 0; i < array->size; i++)
        if(strcmp(array->variables[i]->name, name) == 0)
            return array->variables[i];
    return NULL;
}

bool variable_array_has_variable(VARIABLE_ARRAY *array, char *name)
{
    if(array == NULL)
    {
        printf("ERROR: array is NULL\n");
        exit(27); // ERROR CODE 27
    }
    if(name == NULL)
    {
        printf("ERROR: name is NULL\n");
        exit(28); // ERROR CODE 28
    }
    if(array->variables == NULL)
        return false;
    for(size_t i = 0; i < array->size; i++)
        if(strcmp(array->variables[i]->name, name) == 0)
            return true;
    return false;
}

VARIABLE_ARRAY *create_variable_array()
{
    VARIABLE_ARRAY *array = calloc(1, sizeof(VARIABLE_ARRAY));
    array->size = 0;
    array->variables = NULL;
    return array;
}

VARIABLE *create_variable(AST *type, char *name)
{
    if(type == NULL)
    {
        printf("ERROR: type is NULL\n");
        exit(29); // ERROR CODE 29
    }
    if(name == NULL)
    {
        printf("ERROR: name is NULL\n");
        exit(30); // ERROR CODE 30
    }
    VARIABLE *variable = calloc(1, sizeof(VARIABLE));
    variable->type = type;
    variable->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(variable->name, name);
    return variable;
}

void free_variable(VARIABLE *variable)
{
    if(variable == NULL)
        return;
    if(variable->name != NULL)
        free(variable->name);
    free(variable);
}

void free_variable_array(VARIABLE_ARRAY *array)
{
    if(array == NULL)
        return;
    if(array->variables != NULL)
    {
        for(size_t i = 0; i < array->size; i++)
            free_variable(array->variables[i]);
        free(array->variables);
    }
    free(array);
}

bool has_same_type(AST *value, AST *type)
{
    if(value == NULL)
    {
        printf("ERROR: value is NULL\n");
        exit(31); // ERROR CODE 31
    }
    if(type == NULL)
    {
        printf("ERROR: type is NULL\n");
        exit(32); // ERROR CODE 32
    }
    // TODO: check if the value has the same type as the type
    return true;
}
FUNC_ARRAY *create_function_array()
{
    FUNC_ARRAY *array = calloc(1, sizeof(FUNC_ARRAY));
    array->size = 0;
    array->functions = NULL;
    return array;
}
FUNC *create_function(char *name)
{
    if(name == NULL)
    {
        printf("ERROR: name is NULL\n");
        exit(33); // ERROR CODE 33
    }
    FUNC *function = calloc(1, sizeof(FUNC));
    function->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(function->name, name);
    function->size = 0;
    function->attributes = NULL;
    return function;
}
void FUNCTION_add_attribute(FUNC *function, AST *attribute)
{
    if(function == NULL)
    {
        printf("ERROR: function is NULL\n");
        exit(34); // ERROR CODE 34
    }
    if(attribute == NULL)
    {
        printf("ERROR: attribute is NULL\n");
        exit(35); // ERROR CODE 35
    }
    if(function->attributes == NULL)
    {
        function->attributes = calloc(1, sizeof(AST*));
        function->size = 1;
        function->attributes[0] = attribute;
        return;
    }
    function->size++;
    function->attributes = realloc(function->attributes, sizeof(AST*) * function->size);
    function->attributes[function->size - 1] = attribute;
}
void FUNCTION_ARRAY_add_function(FUNC_ARRAY *array, FUNC *function)
{
    if(array == NULL)
    {
        printf("ERROR: array is NULL\n");
        exit(36); // ERROR CODE 36
    }
    if(function == NULL)
    {
        printf("ERROR: function is NULL\n");
        exit(37); // ERROR CODE 37
    }
    if(array->functions == NULL)
    {
        array->functions = calloc(1, sizeof(FUNC*));
        array->size = 1;
        array->functions[0] = function;
        return;
    }
    array->size++;
    array->functions = realloc(array->functions, sizeof(FUNC*) * array->size);
    array->functions[array->size - 1] = function;
}
void free_function(FUNC *function)
{
    if(function == NULL)
        return;
    if(function->name != NULL)
        free(function->name);
    if(function->attributes != NULL)
    {
        free(function->attributes);
    }
    free(function);
}
void free_function_array(FUNC_ARRAY *array)
{
    if(array == NULL)
        return;
    if(array->functions != NULL)
    {
        for(size_t i = 0; i < array->size; i++)
            free_function(array->functions[i]);
        free(array->functions);
    }
    free(array);
}
SYMBOL_TABLE *create_symbol_table()
{
    SYMBOL_TABLE *table = calloc(1, sizeof(SYMBOL_TABLE));
    table->functions = create_function_array();
    table->variables = NULL;
    return table;
}
void free_symbol_table(SYMBOL_TABLE *table)
{
    if(table == NULL)
        return;
    if(table->functions != NULL)
        free_function_array(table->functions);
    free(table);
}