#include "headers/codegen.h"

void generate_binary_expression(FILE *file, AST *ast, size_t* temp_count);
char* generate_expression(FILE *file, AST *ast, size_t* temp_count);
void generate_return(FILE *file, AST *ast, size_t* temp_count);
void generate_function_body(FILE *file, AST *ast);
void generate_variable_declaration(FILE *file, AST *ast);
void generate_variable_assignment(FILE *file, AST *ast, size_t* temp_count);
void generate_function_call(FILE *file, AST *ast, size_t* temp_count);
void generate_function(AST *ast, FILE *file);

void reallocate_string(char **new, char* format, char *orig)
{
    *new = realloc(*new, strlen(orig) + strlen(format) + 1);
    sprintf(*new, format, orig);
}

CODEGEN *init_codegen(FILE *file, AST *ast)
{
    CODEGEN *codegen = malloc(sizeof(CODEGEN));
    codegen->file = file;
    codegen->ast = ast;
    return codegen;
}
void codegen_free(CODEGEN *codegen)
{
    AST_free(codegen->ast);
    fclose(codegen->file);
    free(codegen);
}
void codegen_generate(CODEGEN *codegen)
{
    if(codegen == NULL)
    {
        printf("Codegen is NULL\n");
        return;
    }
    if(codegen->ast == NULL)
    {
        printf("AST is NULL\n");
        return;
    }
    if(codegen->file == NULL)
    {
        printf("File is NULL\n");
        return;
    }
    AST *ast = codegen->ast;
    FILE *file = codegen->file;
    for(size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
    {
        AST *child = ast->data.AST_NODE.children[i];
        switch (child->tag)
        {
        case AST_FUNCTION:
            generate_function(child, file);
            break;
        default:
            printf("Unknown tag: %d\n", child->tag);
            break;
        }
    }
//     char* main_function_name = "start";
//     fprintf(file, "define i32 @main(i32 %%argc) {\n");
//     fprintf(file, "%%t0 = call i32 @%s(i32 %%argc)\n", main_function_name);
//     fprintf(file, "ret i32 %%t0\n");
//     fprintf(file, "}\n");
}
bool file_and_ast_valid(FILE *file, AST *ast)
{
    return file != NULL && ast != NULL;
}
void generate_function(AST *ast, FILE *file)
{
    if(ast == NULL)
    {
        printf("AST is NULL\n");
        return;
    }
    if(file == NULL)
    {
        printf("File is NULL\n");
        return;
    }
    char* function_name = ast->data.AST_FUNCTION.name;
    fprintf(file, "define i32 @%s(", function_name);
    for(size_t i = 0; i < ast->data.AST_FUNCTION.array_size; i++)
    {
        AST *child = ast->data.AST_FUNCTION.arguments[i];
        char* name = child->data.AST_ARGUMENT.name;
        AST *type = child->data.AST_ARGUMENT.type;
        char* type_name = type->data.AST_TYPE.name;
        char* type_format = (strcmp(type_name, "int") == 0) ? "i32" : "i32";
        fprintf(file, "%s %%%s", type_format, name);
        if(i < ast->data.AST_FUNCTION.array_size - 1)
        {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ") {\n");
    generate_function_body(file, ast->data.AST_FUNCTION.body);
    fprintf(file, "}\n");
}
void generate_function_body(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    size_t* temp_count = calloc(1, sizeof(size_t));
    *temp_count = 0;
    for(size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
    {
        AST *child = ast->data.AST_NODE.children[i];
        switch (child->tag)
        {
        case AST_RETURN:
            generate_return(file, child, temp_count);
            break;
        case AST_DECLARATION:
            generate_variable_declaration(file, child);
            break;
        case AST_ASSIGN:
            generate_variable_assignment(file, child, temp_count);
            break;
        case AST_CALL:
            generate_function_call(file, child, temp_count);
            break;
        default:
            printf("Unknown tag: %d\n", child->tag);
            break;
        }
    }
    free(temp_count);
}
void generate_function_call(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    char** arguments = calloc(ast->data.AST_CALL.array_size, sizeof(char*));
    for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    {
        AST *child = ast->data.AST_CALL.arguments[i];
        char* temp_name = generate_expression(file, child, temp_count);
        arguments[i] = temp_name;
    }
    char *function_name = ast->data.AST_CALL.name;
    fprintf(file, "%%t%ld = call i32 @%s(", *temp_count, function_name);
    for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    {
        fprintf(file, "i32 %s", arguments[i]);
        if(i < ast->data.AST_CALL.array_size - 1)
        {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ")\n");
    for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    {
        free(arguments[i]);
    }
    free(arguments);
}
void generate_variable_assignment(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    char* tempname = generate_expression(file, ast->data.AST_ASSIGN.value, temp_count);
    fprintf(file, "store i32 %s, i32* %%%s\n", tempname, ast->data.AST_ASSIGN.name);
    free(tempname);
}

void generate_variable_declaration(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    fprintf(file, "%%%s = alloca i32\n", ast->data.AST_DECLARATION.name);
}
void generate_return(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    AST *expr = ast->data.AST_RETURN.expr;
    char* tempname = generate_expression(file, expr, temp_count);
    fprintf(file, "ret i32 %s\n", tempname);
    free(tempname);
}
char* generate_expression(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return "";
    char* temp_name = calloc(10, sizeof(char));
    switch (ast->tag)
    {
    case AST_NUMBER:
        temp_name = realloc(temp_name, strlen(ast->data.AST_NUMBER.number) + 1);
        sprintf(temp_name, "%s", ast->data.AST_NUMBER.number);
        return temp_name;
    case AST_VARIABLE:
        if(ast->data.AST_VARIABLE.is_arg)
        {
            temp_name = realloc(temp_name, strlen(ast->data.AST_VARIABLE.name) + 2);
            sprintf(temp_name, "%%%s", ast->data.AST_VARIABLE.name);
            // reallocate_string(&temp_name, "%%%s", ast->data.AST_VARIABLE.name);
            return temp_name;
        }
        fprintf(file, "%%t%ld = load i32, i32* %%%s\n", *temp_count, ast->data.AST_VARIABLE.name);
        break;
    case AST_BINARY_OP:
        generate_binary_expression(file, ast, temp_count);
        break;
    case AST_CALL:
        generate_function_call(file, ast, temp_count);
        break;
    default:
        printf("Unknown tag: %d\n", ast->tag);
        break;
    }
    sprintf(temp_name, "%%t%zu", *temp_count);
    *temp_count = *temp_count + 1;
    return temp_name;
}
void generate_binary_expression(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    if(ast->data.AST_TUPLE.op == NULL)
    {
        printf("Operator is NULL\n");
        return;
    }
    if(ast->data.AST_TUPLE.left == NULL)
    {
        printf("Left is NULL\n");
        return;
    }
    if(ast->data.AST_TUPLE.right == NULL)
    {
        printf("Right is NULL\n");
        return;
    }
    char *op = ast->data.AST_TUPLE.op;
    if(strcmp(op, "+") == 0)
    {
        op = "add";
    }
    else if(strcmp(op, "-") == 0)
    {
        op = "sub";
    }
    else if(strcmp(op, "*") == 0)
    {
        op = "mul";
    }
    else if(strcmp(op, "/") == 0)
    {
        op = "sdiv";
    }
    else
    {
        printf("Unknown operator: %s\n", op);
        return;
    }
    printf("op: %s\n", op);
    // free(op);
    char* left_name = generate_expression(file, ast->data.AST_TUPLE.left, temp_count);
    char* right_name = generate_expression(file, ast->data.AST_TUPLE.right, temp_count);
    char* temp_name = calloc(1, sizeof(char*));
    sprintf(temp_name, "%%t%ld", *temp_count);
    fprintf(file, "%s = %s i32 %s, %s\n", temp_name, op, left_name, right_name);
    free(left_name);
    free(right_name);
    free(temp_name);
}