#ifndef H_SYMBOL_TABLE
#define H_SYMBOL_TABLE

#include "AST.h"

typedef struct {
    AST *type;
    char* name;
} VARIABLE;

typedef struct {
    VARIABLE* *variables;
    size_t size;
} VARIABLE_ARRAY;

typedef struct {
    AST* *attributes;
    size_t size;
    char* name;
} FUNC;

typedef struct {
    FUNC* *functions;
    size_t size;
} FUNC_ARRAY;

typedef struct {
    VARIABLE_ARRAY *variables;
    FUNC_ARRAY *functions;
} SYMBOL_TABLE;

void variable_array_add_variable(VARIABLE_ARRAY *array, VARIABLE *variable);
VARIABLE *variable_array_get_variable(VARIABLE_ARRAY *array, char *name);
bool variable_array_has_variable(VARIABLE_ARRAY *array, char *name);
VARIABLE_ARRAY *create_variable_array();
VARIABLE *create_variable(AST *type, char *name);
void free_variable(VARIABLE *variable);
void free_variable_array(VARIABLE_ARRAY *array);
bool has_same_type(AST *value, AST *type);
FUNC_ARRAY *create_function_array();
FUNC *create_function(char *name);
void FUNCTION_add_attribute(FUNC *function, AST *attribute);
void FUNCTION_ARRAY_add_function(FUNC_ARRAY *array, FUNC *function);
void free_function(FUNC *function);
void free_function_array(FUNC_ARRAY *array);
SYMBOL_TABLE *create_symbol_table();
void free_symbol_table(SYMBOL_TABLE *table);

#endif