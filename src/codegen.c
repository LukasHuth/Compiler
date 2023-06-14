#include "headers/codegen.h"

#define InternalInfo struct INTERNAL_INFO

const bool _DEBUG = true;

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

void init_codegen(AST *ast)
{
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    LLVMModuleRef module = LLVMModuleCreateWithName("new_module");
    LLVMSetTarget(module, LLVMGetDefaultTargetTriple()); // saet target triple to x86_64-pc-linux-gnu
    LLVMBuilderRef builder = LLVMCreateBuilder();

    LLVMTypeRef printf_args[] = {LLVMPointerType(LLVMInt8Type(), 0)};

    LLVMAddFunction(module, "puts", LLVMFunctionType(LLVMInt32Type(), printf_args, 1, 0));
    
    CODEGEN *codegen = malloc(sizeof(CODEGEN));
    codegen->ast = ast;

    codegen_generate(codegen, module, builder);

    LLVMWriteBitcodeToFile(module, "main.bc");
    if(_DEBUG) LLVMPrintModuleToFile(module, "main.ll", NULL);
    codegen_free(codegen);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
}
void codegen_free(CODEGEN *codegen)
{
    AST_free(codegen->ast);
    free(codegen);
}
void codegen_generate(CODEGEN *codegen, LLVMModuleRef module, LLVMBuilderRef builder)
{
    if(codegen == NULL){printf("Codegen is NULL\n");return;}
    if(codegen->ast == NULL){printf("AST is NULL\n");return;}
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
}
bool file_and_ast_valid(FILE *file, AST *ast)
{
    return file != NULL && ast != NULL;
}
LLVMTypeRef get_type(AST *ast)
{
    if(ast == NULL){printf("AST is NULL\n");return NULL;}
    char* type_name = ast->data.AST_TYPE.name;
    if(strcmp(type_name, "int") == 0)
    {
        return LLVMInt32Type();
    }
    if(strcmp(type_name, "float") == 0)
    {
        return LLVMDoubleType();
    }
    else if(strcmp(type_name, "void") == 0)
    {
        return LLVMVoidType();
    }
    else
    {
        printf("Unknown type: %s\n", type_name);
        return NULL;
    }
}
void generate_function(AST *ast, LLVMModuleRef module, LLVMBuilderRef builder)
{
    if(ast == NULL){printf("AST is NULL\n");return;}
    if(module == NULL){printf("Module is NULL\n");return;}
    if(builder == NULL){printf("Builder is NULL\n");return;}
    char* function_name = ast->data.AST_FUNCTION.name;
    unsigned int param_count = ast->data.AST_FUNCTION.array_size;
    LLVMTypeRef *param_types = malloc(sizeof(LLVMTypeRef) * param_count);
    for(unsigned int i = 0; i < param_count; i++)
    {
        AST *child = ast->data.AST_FUNCTION.arguments[i];
        AST *type = child->data.AST_ARGUMENT.type;
        LLVMTypeRef type_ref = get_type(type);
        if(type_ref == NULL){printf("Type is NULL\n");return;}
        param_types[i] = type_ref;
    }
    LLVMTypeRef function_type = LLVMFunctionType(LLVMInt32Type(), param_types, param_count, false);
    free(param_types);
    LLVMValueRef function = LLVMAddFunction(module, function_name, function_type);
    LLVMSetLinkage(function, LLVMExternalLinkage);
    LLVMSetFunctionCallConv(function, LLVMCCallConv);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
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
            printf("Call begin\n");
            generate_function_call(child, info);
            printf("Call end\n");
            break;
        default:
            printf("Unknown tag: %d\n", child->tag);
            break;
        }
    }
}
LLVMValueRef get_number_type(AST *ast)
{
    if(ast == NULL) return NULL;
    char* number = ast->data.AST_NUMBER.number;
    float f;
    (void)sscanf(number, "%f", &f);
    LLVMValueRef value = NULL;
    bool is_float = false;
    for(size_t i = 0; i < strlen(number); i++)
    {
        if(number[i] == '.')
        {
            is_float = true;
            break;
        }
    }

    if(is_float)
    {
        printf("----------\nnumber: %f\n", atof(number));
        value = LLVMConstReal(LLVMDoubleType(), atof(number));
        return value;
    } else {
        printf("number: %s\n", number);
        value = LLVMConstInt(LLVMInt32Type(), atoi(number), 0);
    }
    return value;
}
bool is_std_function(char *name)
{
    if(strcmp(name, "print") == 0) return true;
    return false;
}
LLVMTypeRef get_variable_type(char* name, InternalInfo *info)
{
    if(!isValidInfo(info)){printf("Info is invalid (variable_type)\n");return NULL;}
    for(size_t i = 0; i < info->variable_count; i++)
    {
        if(strcmp(name, LLVMGetValueName(info->variables[i])) == 0)
        {
            return LLVMTypeOf(info->variables[i]);
        }
    }
    return NULL;
}
LLVMValueRef generate_std_function_call(AST *ast, InternalInfo *info)
{
    if(!isValidInfo(info)){printf("Info is invalid(std function call)\n");return NULL;}
    if(ast == NULL){printf("AST is NULL\n");return NULL;}
    if(ast->tag != AST_CALL){printf("AST is not a call\n");return NULL;}
    char *function_name = ast->data.AST_CALL.name;
    AST **arguments = ast->data.AST_CALL.arguments;
    size_t argc = ast->data.AST_CALL.array_size;
    if(strcmp(function_name, "print") == 0)
    {
        if(argc != 1){printf("Wrong number of arguments for print\n");return NULL;}
        AST *argument = arguments[0];
        LLVMValueRef value = generate_expression(argument, info);
        if(value == NULL){printf("Value is NULL\n");return NULL;}
        LLVMTypeRef type = LLVMTypeOf(value);
        char* typename = LLVMPrintTypeToString(type);
        if(type == NULL){printf("Type is NULL\n");return NULL;}
        LLVMValueRef function = LLVMGetNamedFunction(info->module, "printf");
        if(function == NULL)
        {
            LLVMTypeRef *printf_args = calloc(1, sizeof(LLVMTypeRef));
            printf_args[0] = LLVMPointerType(LLVMInt8Type(), 0);
            // printf_args[1] = type;
            LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(), printf_args, 1, true);
            function = LLVMAddFunction(info->module, "printf", printf_type);
            LLVMSetLinkage(function, LLVMExternalLinkage);
            LLVMSetFunctionCallConv(function, LLVMCCallConv);
        }

        LLVMValueRef ret = NULL;
        if(strcmp(typename, "i32") == 0)
        {
            // printf("hallo\n");
            LLVMValueRef format_str = LLVMBuildGlobalStringPtr(info->builder, "%d\n", "format_str_d");
            LLVMValueRef args[2] = {format_str, value};
            ret = LLVMBuildCall(info->builder, function, args, 2, "ret");
        } else if(strcmp(typename, "double") == 0)
        {
            // LLVMValueRef function = LLVMGetNamedFunction(info->module, "printf");
            LLVMValueRef format_str = LLVMBuildGlobalStringPtr(info->builder, "%f\n", "format_str_f");
            LLVMValueRef args[2] = {format_str, value};
            ret = LLVMBuildCall(info->builder, function, args, 2, "ret");
        }
        if(ret == NULL) printf("Unknown type for print\n");
        return ret;
    }
    printf("Unknown std function: %s\n", function_name);
    return NULL;
}
LLVMValueRef generate_function_call(AST *ast, InternalInfo *info)
{
    if(!isValidInfo(info)){printf("Info is invalid(function call)\n");return NULL;}
    if(ast == NULL){printf("AST is NULL\n");return NULL;}
    if(ast->tag != AST_CALL){printf("AST is not a call\n");return NULL;}
    char *function_name = ast->data.AST_CALL.name;
    if(is_std_function(function_name)) return generate_std_function_call(ast, info);
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
    sprintf(temp_name, "_%ld", info->temp_count++);
    temp_name = realloc(temp_name, sizeof(char) * (strlen(temp_name) + 1));
    LLVMValueRef result = LLVMBuildCall(info->builder, function, args, argc, temp_name);
    free(args);
    return result;
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
    if(ast->tag != AST_DECLARATION){printf("AST is not a declaration\n");return;}
    LLVMTypeRef type = get_type(ast->data.AST_DECLARATION.type);
    LLVMValueRef value = LLVMBuildAlloca(info->builder, type, ast->data.AST_DECLARATION.name);
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
    if(ast == NULL) return NULL;
    if(!isValidInfo(info)) return NULL;
    char* temp_name = calloc(10, sizeof(char));
    LLVMValueRef value = NULL;
    switch (ast->tag)
    {
    case AST_NUMBER:
        // TODO: Fix problem that will be encountered with Floats
        // value = LLVMConstInt(LLVMInt32Type(), atoi(ast->data.AST_NUMBER.number), 0);
        value = get_number_type(ast);
        break;
    case AST_VARIABLE:
        if(ast->data.AST_VARIABLE.is_arg)
        {
            LLVMValueRef arg = LLVMGetParam(info->function, ast->data.AST_VARIABLE.arg_index);
            return arg;
        }
        sprintf(temp_name, "_%ld", info->temp_count++);
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
        value = ret_value;
        break;
    case AST_BINARY_OP:
        value = generate_binary_expression(ast, info);
        break;
    case AST_CALL:
        value = generate_function_call(ast, info);
        break;
    default:
        printf("Unknown tag: %d\n", ast->tag);
        break;
    }
    free(temp_name);
    return value;
}
LLVMValueRef generate_binary_operation_with_int(char* op, LLVMValueRef left, LLVMValueRef right, char* temp_name, LLVMBuilderRef builder)
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
LLVMValueRef generate_binary_operation_with_double(char* op, LLVMValueRef left, LLVMValueRef right, char* temp_name, LLVMBuilderRef builder)
{
    if (strcmp(op, "+") == 0)
    {
        return LLVMBuildFAdd(builder, left, right, temp_name);
    }
    else if (strcmp(op, "-") == 0)
    {
        return LLVMBuildFSub(builder, left, right, temp_name);
    }
    else if (strcmp(op, "*") == 0)
    {
        return LLVMBuildFMul(builder, left, right, temp_name);
    }
    else if (strcmp(op, "/") == 0)
    {
        return LLVMBuildFDiv(builder, left, right, temp_name);
    }
    else
    {
        printf("Unknown operator: %s\n", op);
        return NULL;
    }
}
LLVMValueRef generate_binary_operation_with_string(char* op, LLVMValueRef left, LLVMValueRef right, char* temp_name, LLVMBuilderRef builder)
{
    LLVMTypeKind typekind = LLVMGetTypeKind(LLVMTypeOf(left));
    if(typekind == LLVMIntegerTypeKind)
    {
        return generate_binary_operation_with_int(op, left, right, temp_name, builder);
    }
    else if(typekind == LLVMDoubleTypeKind)
    {
        return generate_binary_operation_with_double(op, left, right, temp_name, builder);
    }
    else
    {
        printf("Unknown type: %d\n", typekind);
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
    sprintf(temp_name, "_%ld", info->temp_count++);
    LLVMValueRef value = generate_binary_operation_with_string(op, left, right, temp_name, info->builder);
    return value;
}