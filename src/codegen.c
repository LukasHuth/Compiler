#include "headers/codegen.h"

LLVMValueRef generate_binary_expression(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables);
LLVMValueRef generate_expression(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables);
void generate_return(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables);
void generate_function_body(AST *ast, LLVMBuilderRef builder);
void generate_variable_declaration(AST *ast, LLVMBuilderRef builder, LLVMValueRef* variables);
void generate_variable_assignment(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables);
LLVMValueRef generate_function_call(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables);
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
    // TODO: generate variables that contain the arguments
    // fprintf(file, ") {\n");
    LLVMTypeRef function_type = LLVMFunctionType(LLVMInt32Type(), param_types, param_count, false);
    LLVMValueRef function = LLVMAddFunction(module, function_name, function_type);
    LLVMSetLinkage(function, LLVMExternalLinkage);
    LLVMSetFunctionCallConv(function, LLVMCCallConv);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    generate_function_body(ast->data.AST_FUNCTION.body, builder);
    // fprintf(file, "}\n");
}
void generate_function_body(AST *ast, LLVMBuilderRef builder)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(builder == NULL){printf("Builder is NULL\n");return;}
    if(ast->tag != AST_NODE) return;
    if(ast->data.AST_NODE.array_size == 0) return;
    if(ast->data.AST_NODE.children == NULL) return;
    size_t* temp_count = calloc(1, sizeof(size_t));
    *temp_count = 0;
    LLVMValueRef* variables = calloc(0, sizeof(LLVMValueRef));
    for(size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
    {
        AST *child = ast->data.AST_NODE.children[i];
        switch (child->tag)
        {
        case AST_RETURN:
            generate_return(child, temp_count, builder, variables);
            break;
        case AST_DECLARATION:
            generate_variable_declaration(child, builder, variables);
            break;
        case AST_ASSIGN:
            generate_variable_assignment(child, temp_count, builder, variables);
            break;
        case AST_CALL:
            generate_function_call(child, temp_count, builder, variables);
            break;
        default:
            printf("Unknown tag: %d\n", child->tag);
            break;
        }
    }
    free(variables);
    free(temp_count);
}
LLVMValueRef generate_function_call(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables)
{
    if(ast == NULL){printf("AST is NULL\n");return NULL;}
    if(builder == NULL){printf("Builder is NULL\n");return NULL;}
    if(variables == NULL){printf("Variables is NULL\n");return NULL;}
    if(ast->tag != AST_CALL){printf("AST is not a call\n");return NULL;}
    char *function_name = ast->data.AST_CALL.name;
    AST **arguments = ast->data.AST_CALL.arguments;
    LLVMModuleRef module = LLVMGetGlobalParent(builder);
    LLVMValueRef function = LLVMGetNamedFunction(module, function_name);
    LLVMValueRef* args = calloc(ast->data.AST_CALL.array_size, sizeof(LLVMValueRef));
    for(size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
    {
        args[i] = generate_expression(arguments[i], temp_count, builder, variables);
    }
    LLVMValueRef result = LLVMBuildCall(builder, function, args, ast->data.AST_CALL.array_size, "call");
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
void generate_variable_assignment(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(builder == NULL){printf("Builder is NULL\n");return;}
    if(variables == NULL){printf("Variables is NULL\n");return;}
    if(ast->tag != AST_ASSIGN){printf("AST is not an assignment\n");return;}
    char* name = ast->data.AST_ASSIGN.name;
    AST *expr = ast->data.AST_ASSIGN.value;
    LLVMValueRef value = generate_expression(expr, temp_count, builder, variables);
    // value = iterate vars
    LLVMValueRef variable = NULL;
    for(size_t i = 0; i < sizeof(variables); i++)
    {
        if(strcmp(name, LLVMGetValueName(variables[i])) == 0)
        {
            variable = variables[i];
            break;
        }
    }
    LLVMBuildStore(builder, value, variable);
}
void generate_variable_declaration(AST *ast, LLVMBuilderRef builder, LLVMValueRef* variables)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(builder == NULL){printf("Builder is NULL\n");return;}
    if(variables == NULL){printf("Variables is NULL\n");return;}
    LLVMValueRef value = LLVMBuildAlloca(builder, LLVMInt32Type(), ast->data.AST_DECLARATION.name);
    LLVMSetInitializer(value, LLVMConstInt(LLVMInt32Type(), 0, 0));
    size_t var_size = sizeof(variables);
    variables = realloc(variables, sizeof(LLVMValueRef) * (var_size + 1));
    variables[var_size] = value;
}
void generate_return(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables)
{
    if(ast == NULL) return;
    if(ast->data.AST_RETURN.expr == NULL) return;
    if(builder == NULL) return;
    AST *expr = ast->data.AST_RETURN.expr;
    LLVMValueRef value = generate_expression(expr, temp_count, builder, variables);
    if(value == NULL) return;
    LLVMBuildRet(builder, value);
}
LLVMValueRef generate_expression(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables)
{
    // if(!file_and_ast_valid(file, ast)) return "";
    if(ast == NULL) return NULL;
    if(builder == NULL) return NULL;
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
            LLVMModuleRef module = LLVMGetGlobalParent(builder); // TODO: FIX THIS
            LLVMValueRef func = LLVMGetNamedFunction(module, "main");
            LLVMValueRef arg = LLVMGetParam(func, ast->data.AST_VARIABLE.arg_index);
            return arg;
        }
        temp_name = realloc(temp_name, 4 + 10);
        sprintf(temp_name, "temp%ld", *temp_count);
        (*temp_count)++;
        temp_name = realloc(temp_name, strlen(temp_name) + 1);
        value = NULL;
        for(size_t i = 0; i < sizeof(variables); i++)
        {
            if(strcmp(ast->data.AST_VARIABLE.name, LLVMGetValueName(variables[i])) == 0)
            {
                value = variables[i];
            }
        }
        if(value == NULL) return NULL;
        LLVMValueRef ret_value = LLVMBuildLoad(builder, value, temp_name);
        return ret_value;
    case AST_BINARY_OP:
        return generate_binary_expression(ast, temp_count, builder, variables);
    case AST_CALL:
        return generate_function_call(ast, temp_count, builder, variables);
    default:
        printf("Unknown tag: %d\n", ast->tag);
        break;
    }
    return NULL;
}
LLVMValueRef generate_binary_expression(AST *ast, size_t* temp_count, LLVMBuilderRef builder, LLVMValueRef* variables)
{
    if(ast == NULL) return NULL;
    if(builder == NULL) return NULL;
    if(ast->data.AST_TUPLE.op == NULL){printf("Operator is NULL\n");return NULL;}
    if(ast->data.AST_TUPLE.left == NULL){printf("Left is NULL\n");return NULL;}
    if(ast->data.AST_TUPLE.right == NULL){printf("Right is NULL\n");return NULL;}
    char *op = ast->data.AST_TUPLE.op;
    LLVMValueRef left = generate_expression(ast->data.AST_TUPLE.left, temp_count, builder, variables);
    LLVMValueRef right = generate_expression(ast->data.AST_TUPLE.right, temp_count, builder, variables);
    char* temp_name = calloc(1, sizeof(char*));
    LLVMValueRef value = NULL;
    sprintf(temp_name, "t%ld", *temp_count);
    (*temp_count)++;
    if(strcmp(op, "+") == 0)
    {
        value = LLVMBuildAdd(builder, left, right, temp_name);
    }
    else if(strcmp(op, "-") == 0)
    {
        value = LLVMBuildSub(builder, left, right, temp_name);
    }
    else if(strcmp(op, "*") == 0)
    {
        value = LLVMBuildMul(builder, left, right, temp_name);
    }
    else if(strcmp(op, "/") == 0)
    {
        value = LLVMBuildSDiv(builder, left, right, temp_name);
    }
    else
    {
        printf("Unknown operator: %s\n", op);
        return NULL;
    }
    return value;
}