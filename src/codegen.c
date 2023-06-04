#include "headers/codegen.h"

void generate_binary_expression(FILE *file, AST *ast, size_t* temp_count);
char* generate_expression(FILE *file, AST *ast, size_t* temp_count);
void generate_return(FILE *file, AST *ast, size_t* temp_count);
void generate_function_body(FILE *file, AST *ast);
void generate_variable_declaration(FILE *file, AST *ast, size_t* temp_count);
void generate_variable_assignment(FILE *file, AST *ast, size_t* temp_count);
void generate_function_call(FILE *file, AST *ast, size_t* temp_count);
void generate_function(AST *ast, FILE *file);

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
    char* main_function_name = "start";
    fprintf(file, "define i32 @main() {\n");
    fprintf(file, "%%t0 = call i32 @%s()\n", main_function_name);
    fprintf(file, "ret i32 %%t0\n");
    fprintf(file, "}\n");
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
    fprintf(file, "define i32 @%s() {\n", function_name);
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
            generate_variable_declaration(file, child, temp_count);
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
    fprintf(file, "call i32 @%s(", function_name);
    for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    {
        fprintf(file, "i32 %s", arguments[i]);
        if(i < ast->data.AST_CALL.array_size - 1)
        {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ")\n");
}
void generate_variable_assignment(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    char* tempname = generate_expression(file, ast->data.AST_ASSIGN.value, temp_count);
    fprintf(file, "store i32 %s, i32* %%%s\n", tempname, ast->data.AST_ASSIGN.name);
}

void generate_variable_declaration(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    fprintf(file, "%%%s = alloca i32\n", ast->data.AST_DECLARATION.name);
    // fprintf(file, "store i32 ");
    char* tempname = generate_expression(file, ast->data.AST_DECLARATION.value, temp_count);
    fprintf(file, "store i32 %s, i32* %%%s\n", tempname, ast->data.AST_DECLARATION.name);
}
void generate_return(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return;
    AST *expr = ast->data.AST_RETURN.expr;
    char* tempname = generate_expression(file, expr, temp_count);
    fprintf(file, "ret i32 %s\n", tempname);
}
char* generate_expression(FILE *file, AST *ast, size_t* temp_count)
{
    if(!file_and_ast_valid(file, ast)) return "";
    char* temp_name = calloc(10, sizeof(char));
    switch (ast->tag)
    {
    case AST_NUMBER:
        return ast->data.AST_NUMBER.number;
    case AST_VARIABLE:
        fprintf(file, "%%t%ld = load i32, i32* %%%s\n", *temp_count, ast->data.AST_VARIABLE.name);
        break;
    case AST_ADD: // merge in binary tag
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
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
    char *op = NULL;
    switch (ast->tag)
    {
    case AST_ADD:
        op = "add";
        break;
    case AST_SUB:
        op = "sub";
        break;
    case AST_MUL:
        op = "mul";
        break;
    case AST_DIV:
        op = "sdiv";
        break;
    default:
        printf("Unknown tag: %d\n", ast->tag);
        break;
    }
    printf("op: %s\n", op);
    // free(op);
    char* left_name = generate_expression(file, ast->data.AST_TUPLE.left, temp_count);
    char* right_name = generate_expression(file, ast->data.AST_TUPLE.right, temp_count);
    *temp_count = *temp_count + 1;
    char* temp_name = calloc(1, sizeof(char*));
    sprintf(temp_name, "%%t%ld", *temp_count);
    fprintf(file, "%s = %s i32 %s, %s\n", temp_name, op, left_name, right_name);
    free(temp_name);
}