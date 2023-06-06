#include "headers/codegen.h"

#define InternalInfo struct INTERNAL_INFO

struct INTERNAL_INFO
{
    size_t temp_count;
    LLVMBuilderRef builder;
    LLVMValueRef function;
    LLVMValueRef* variables;
    LLVMModuleRef module;
    size_t variable_count;
};

bool isValidInfo(InternalInfo *info)
{
    return (
        info != NULL &&
        info->builder != NULL &&
        info->variables != NULL &&
        info->function != NULL &&
        info->module != NULL
    );
}

void generate_function_body(AST *ast, InternalInfo *info);
LLVMValueRef generate_binary_expression(AST *ast, InternalInfo *info);
LLVMValueRef generate_expression(AST *ast, InternalInfo *info);
void generate_return(AST *ast, InternalInfo *info);
void generate_variable_declaration(AST *ast, InternalInfo *info);
void generate_variable_assignment(AST *ast, InternalInfo *info);
LLVMValueRef generate_function_call(AST *ast, InternalInfo *info);
void generate_function(AST *ast, LLVMModuleRef module, LLVMBuilderRef builder);

void codegen_generate(CODEGEN *codegen, LLVMModuleRef module, LLVMBuilderRef builder);

void codegen_free(CODEGEN *codegen);

void reallocate_string(char **new, char* format, char *orig)
{
    *new = realloc(*new, strlen(orig) + strlen(format) + 1);
    sprintf(*new, format, orig);
}

void init_codegen(FILE *file, AST *ast)
{
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    LLVMModuleRef module = LLVMModuleCreateWithName("add_module");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    
    CODEGEN *codegen = malloc(sizeof(CODEGEN));
    codegen->file = file;
    codegen->ast = ast;

    codegen_generate(codegen, module, builder);

    LLVMWriteBitcodeToFile(module, "main.bc");
    codegen_free(codegen);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
}
void codegen_free(CODEGEN *codegen)
{
    AST_free(codegen->ast);
    fclose(codegen->file);
    free(codegen);
}
void codegen_generate(CODEGEN *codegen, LLVMModuleRef module, LLVMBuilderRef builder)
{
    if(codegen == NULL){printf("Codegen is NULL\n");return;}
    if(codegen->ast == NULL){printf("AST is NULL\n");return;}
    if(codegen->file == NULL){printf("File is NULL\n");return;}
    if(module == NULL){printf("Module is NULL\n");return;}
    if(builder == NULL){printf("Builder is NULL\n");return;}
    AST *ast = codegen->ast;
    // FILE *file = codegen->file;
    for(size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
    {
        AST *child = ast->data.AST_NODE.children[i];
        switch (child->tag)
        {
        case AST_FUNCTION:
            generate_function(child, /*file, */module, builder);
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
void generate_function(AST *ast, LLVMModuleRef module, LLVMBuilderRef builder)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(module == NULL){printf("Module is NULL\n");return;}
    if(builder == NULL){printf("Builder is NULL\n");return;}
    char* function_name = ast->data.AST_FUNCTION.name;
    // fprintf(file, "define i32 @%s(", function_name);
    unsigned int param_count = ast->data.AST_FUNCTION.array_size;
    LLVMTypeRef *param_types = malloc(sizeof(LLVMTypeRef) * param_count);
    for(unsigned int i = 0; i < param_count; i++)
    {
        // AST *child = ast->data.AST_FUNCTION.arguments[i];
        // char* name = child->data.AST_ARGUMENT.name;
        // AST *type = child->data.AST_ARGUMENT.type;
        // char* type_name = type->data.AST_TYPE.name;
        param_types[i] = LLVMInt32Type();
    }
    LLVMTypeRef function_type = LLVMFunctionType(LLVMInt32Type(), param_types, param_count, false);
    LLVMValueRef function = LLVMAddFunction(module, function_name, function_type);
    LLVMSetLinkage(function, LLVMExternalLinkage);
    LLVMSetFunctionCallConv(function, LLVMCCallConv);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    /*// only needed if i dont implement the index of the arguments in the AST
    for(size_t i = 0; i < param_count; i++)
    {
        AST *child = ast->data.AST_FUNCTION.arguments[i];
        char* name = child->data.AST_ARGUMENT.name;
        LLVMValueRef variable = LLVMBuildAlloca(builder, LLVMInt32Type(), name);
        LLVMSetAlignment(variable, 4);
        LLVMValueRef argument = LLVMGetParam(function, i);
        LLVMBuildStore(builder, argument, variable);
    }
    */
    InternalInfo *info = malloc(sizeof(InternalInfo));
    info->builder = builder;
    info->temp_count = 0;
    info->variables = NULL;
    info->variable_count = 0;
    info->function = function;
    info->module = module;
    generate_function_body(ast->data.AST_FUNCTION.body, info);
    free(info->variables);
    free(info);
}
void generate_function_body(AST *ast, InternalInfo *info)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(info->variables != NULL) free(info->variables);
    info->variables = calloc(0, sizeof(LLVMValueRef));
    if(!isValidInfo(info)){printf("Info is invalid (function_body)\n");return;}
    if(ast->tag != AST_NODE) return;
    if(ast->data.AST_NODE.array_size == 0) return;
    if(ast->data.AST_NODE.children == NULL) return;

    info->temp_count = 0;
    info->variable_count = 0;

    for(size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
    {
        AST *child = ast->data.AST_NODE.children[i];
        switch (child->tag)
        {
        case AST_RETURN:
            generate_return(child, info);
            break;
        case AST_DECLARATION:
            generate_variable_declaration(child, info);
            break;
        case AST_ASSIGN:
            generate_variable_assignment(child, info);
            break;
        case AST_CALL:
            generate_function_call(child, info);
            break;
        default:
            printf("Unknown tag: %d\n", child->tag);
            break;
        }
    }
}
LLVMValueRef generate_function_call(AST *ast, InternalInfo *info)
{
    if(!isValidInfo(info)){printf("Info is invalid(function call)\n");return NULL;}
    if(ast == NULL){printf("AST is NULL\n");return NULL;}
    if(ast->tag != AST_CALL){printf("AST is not a call\n");return NULL;}
    char *function_name = ast->data.AST_CALL.name;
    AST **arguments = ast->data.AST_CALL.arguments;
    size_t argc = ast->data.AST_CALL.array_size;
    LLVMValueRef function = LLVMGetNamedFunction(info->module, function_name);
    LLVMValueRef* args = calloc(ast->data.AST_CALL.array_size, sizeof(LLVMValueRef));
    for(size_t i = 0; i < argc; i++)
    {
        args[i] = generate_expression(arguments[i], info);
        if(args[i] == NULL) return NULL;
    }
    char* temp_name = calloc(10, sizeof(char));
    sprintf(temp_name, "_t%ld", info->temp_count++);
    temp_name = realloc(temp_name, sizeof(char) * (strlen(temp_name) + 1));
    LLVMValueRef result = LLVMBuildCall(info->builder, function, args, argc, temp_name);
    free(args);
    return result;
    // fprintf(file, "%%t%ld = call i32 @%s(", *temp_count, function_name);
    // for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    // {
    //     fprintf(file, "i32 %s", arguments[i]);
    //     if(i < ast->data.AST_CALL.array_size - 1)
    //     {
    //         fprintf(file, ", ");
    //     }
    // }
    // fprintf(file, ")\n");
    // for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    // {
    //     free(arguments[i]);
    // }
    // free(arguments);
}
void generate_variable_assignment(AST *ast, InternalInfo *info)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(!isValidInfo(info)){printf("Info is invalid(variable assignment)\n");return;}
    if(ast->tag != AST_ASSIGN){printf("AST is not an assignment\n");return;}
    char* name = ast->data.AST_ASSIGN.name;
    AST *expr = ast->data.AST_ASSIGN.value;
    LLVMValueRef value = generate_expression(expr, info);
    if(value == NULL) return;
    // value = iterate vars
    LLVMValueRef variable = NULL;
    for(size_t i = 0; i < info->variable_count; i++)
    {
        if(strcmp(name, LLVMGetValueName(info->variables[i])) == 0)
        {
            variable = info->variables[i];
            break;
        }
    }
    LLVMBuildStore(info->builder, value, variable);
}
void generate_variable_declaration(AST *ast, InternalInfo *info)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(!isValidInfo(info)){printf("Info is invalid(variable_declaration)\n");return;}
    LLVMValueRef value = LLVMBuildAlloca(info->builder, LLVMInt32Type(), ast->data.AST_DECLARATION.name);
    LLVMSetInitializer(value, LLVMConstInt(LLVMInt32Type(), 0, 0));
    info->variable_count++;
    info->variables = realloc(info->variables, sizeof(LLVMValueRef) * info->variable_count);
    info->variables[info->variable_count - 1] = value;
}
void generate_return(AST *ast, InternalInfo* info)
{
    if(ast == NULL) return;
    if(ast->data.AST_RETURN.expr == NULL) return;
    if(!isValidInfo(info)) return;
    AST *expr = ast->data.AST_RETURN.expr;
    LLVMValueRef value = generate_expression(expr, info);
    if(value == NULL) return;
    LLVMBuildRet(info->builder, value);
}
LLVMValueRef generate_expression(AST *ast, InternalInfo *info)
{
    // if(!file_and_ast_valid(file, ast)) return "";
    if(ast == NULL) return NULL;
    if(!isValidInfo(info)) return NULL;
    char* temp_name = calloc(10, sizeof(char));
    LLVMValueRef value = NULL;
    switch (ast->tag)
    {
    case AST_NUMBER:
        // TODO: Fix problem that will be encountered with Floats
        value = LLVMConstInt(LLVMInt32Type(), atoi(ast->data.AST_NUMBER.number), 0);
        return value;
    case AST_VARIABLE:
        if(ast->data.AST_VARIABLE.is_arg)
        {
            LLVMValueRef arg = LLVMGetParam(info->function, ast->data.AST_VARIABLE.arg_index);
            return arg;
        }
        sprintf(temp_name, "_t%ld", info->temp_count++);
        temp_name = realloc(temp_name, sizeof(char) * (strlen(temp_name) + 1));
        value = NULL;
        for(size_t i = 0; i < info->variable_count; i++)
        {
            if(strcmp(ast->data.AST_VARIABLE.name, LLVMGetValueName(info->variables[i])) == 0)
            {
                value = info->variables[i];
            }
        }
        if(value == NULL) return NULL;
        LLVMValueRef ret_value = LLVMBuildLoad(info->builder, value, temp_name);
        return ret_value;
    case AST_BINARY_OP:
        return generate_binary_expression(ast, info);
    case AST_CALL:
        return generate_function_call(ast, info);
    default:
        printf("Unknown tag: %d\n", ast->tag);
        break;
    }
    return NULL;
}
LLVMValueRef generate_binary_operation_with_string(char* op, LLVMValueRef left, LLVMValueRef right, char* temp_name, LLVMBuilderRef builder)
{
    if (strcmp(op, "+") == 0)
    {
        return LLVMBuildAdd(builder, left, right, temp_name);
    }
    else if (strcmp(op, "-") == 0)
    {
        return LLVMBuildSub(builder, left, right, temp_name);
    }
    else if (strcmp(op, "*") == 0)
    {
        return LLVMBuildMul(builder, left, right, temp_name);
    }
    else if (strcmp(op, "/") == 0)
    {
        return LLVMBuildSDiv(builder, left, right, temp_name);
    }
    else
    {
        printf("Unknown operator: %s\n", op);
        return NULL;
    }
}
LLVMValueRef generate_binary_expression(AST *ast, InternalInfo *info)
{
    if(ast == NULL) return NULL;
    if(!isValidInfo(info)) return NULL;
    if(ast->data.AST_TUPLE.op == NULL){printf("Operator is NULL\n");return NULL;}
    if(ast->data.AST_TUPLE.left == NULL){printf("Left is NULL\n");return NULL;}
    if(ast->data.AST_TUPLE.right == NULL){printf("Right is NULL\n");return NULL;}
    char *op = ast->data.AST_TUPLE.op;
    LLVMValueRef left = generate_expression(ast->data.AST_TUPLE.left, info);
    LLVMValueRef right = generate_expression(ast->data.AST_TUPLE.right, info);
    if(left == NULL){printf("Left is NULL\n");return NULL;}
    if(right == NULL){printf("Right is NULL\n");return NULL;}
    char* temp_name = calloc(1, sizeof(char*));
    sprintf(temp_name, "_t%ld", info->temp_count++);
    LLVMValueRef value = generate_binary_operation_with_string(op, left, right, temp_name, info->builder);
    return value;
}