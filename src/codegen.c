#include "headers/codegen.h"

void generate_binary_expression(FILE *file, AST *ast);
void generate_expression(FILE *file, AST *ast);
void generate_return(FILE *file, AST *ast);
void generate_function_body(FILE *file, AST *ast);
void generate_variable_declaration(FILE *file, AST *ast);
void generate_variable_assignment(FILE *file, AST *ast);
void generate_function_call(FILE *file, AST *ast);
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
    fprintf(file, "ret i32 call i32 @%s\n", main_function_name);
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
    for(size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
    {
        AST *child = ast->data.AST_NODE.children[i];
        switch (child->tag)
        {
        case AST_RETURN:
            generate_return(file, child);
            break;
        case AST_DECLARATION:
            generate_variable_declaration(file, child);
            break;
        case AST_ASSIGN:
            generate_variable_assignment(file, child);
            break;
        case AST_CALL:
            generate_function_call(file, child);
            break;
        default:
            printf("Unknown tag: %d\n", child->tag);
            break;
        }
    }
}
void generate_function_call(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    char *function_name = ast->data.AST_CALL.name;
    fprintf(file, "call i32 @%s(", function_name);
    for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    {
        AST *child = ast->data.AST_CALL.arguments[i];
        generate_expression(file, child);
        if(i < ast->data.AST_CALL.array_size - 1)
        {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ")\n");
}
void generate_variable_assignment(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    fprintf(file, "store i32 ");
    generate_expression(file, ast->data.AST_ASSIGN.value);
    fprintf(file, ", i32* %%%s\n", ast->data.AST_ASSIGN.name);
}

void generate_variable_declaration(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    fprintf(file, "%%%s = alloca i32\n", ast->data.AST_DECLARATION.name);
    fprintf(file, "store i32 ");
    generate_expression(file, ast->data.AST_DECLARATION.value);
    fprintf(file, ", i32* %%%s\n", ast->data.AST_DECLARATION.name);
}
void generate_return(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    fprintf(file, "ret i32 ");
    AST *expr = ast->data.AST_RETURN.expr;
    generate_expression(file, expr);
}
void generate_expression(FILE *file, AST *ast)
{
    if(!file_and_ast_valid(file, ast)) return;
    switch (ast->tag)
    {
    case AST_NUMBER:
        fprintf(file, "%d", ast->data.AST_NUMBER.number);
        break;
    case AST_VARIABLE:
        fprintf(file, "%%%s", ast->data.AST_VARIABLE.name);
        break;
    case AST_ADD: // merge in binary tag
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
        generate_binary_expression(file, ast);
        break;
    case AST_CALL:
        generate_function_call(file, ast);
        break;
    default:
        printf("Unknown tag: %d\n", ast->tag);
        break;
    }
}
void generate_binary_expression(FILE *file, AST *ast)
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
    fprintf(file, "%s i32 ", op);
    // free(op);
    generate_expression(file, ast->data.AST_TUPLE.left);
    fprintf(file, ", ");
    generate_expression(file, ast->data.AST_TUPLE.right);
}