#include "headers/AST.h"

const bool DEBUG = false;

AST *AST_new(AST ast)
{
    AST *ptr = malloc(sizeof(AST));
    if (ptr) *ptr = ast;
    return ptr;
}

AST *AST_new_node()
{
    AST ast = { .tag = AST_NODE, .data = { .AST_NODE = { .children = calloc(sizeof(AST), 0), .array_size = 0 } } };
    return AST_new(ast);
}

void AST_add_child(AST* ast, AST* child)
{
    // not finished
    if (ast->tag == AST_NODE)
    {
        ast->data.AST_NODE.array_size++;
        ast->data.AST_NODE.children = realloc(ast->data.AST_NODE.children, sizeof(AST*) * (ast->data.AST_NODE.array_size));
        ast->data.AST_NODE.children[ast->data.AST_NODE.array_size - 1] = child;
        return;
    }
}
void AST_add_argument(AST* ast, AST* argument)
{
    if(ast->tag == AST_FUNCTION)
    {
        ast->data.AST_FUNCTION.array_size++;
        ast->data.AST_FUNCTION.arguments = realloc(ast->data.AST_FUNCTION.arguments, sizeof(AST*) * (ast->data.AST_FUNCTION.array_size));
        ast->data.AST_FUNCTION.arguments[ast->data.AST_FUNCTION.array_size - 1] = argument;
        return;
    }
    if(ast->tag == AST_CALL)
    {
        ast->data.AST_CALL.array_size++;
        ast->data.AST_CALL.arguments = realloc(ast->data.AST_CALL.arguments, sizeof(AST*) * (ast->data.AST_CALL.array_size));
        ast->data.AST_CALL.arguments[ast->data.AST_CALL.array_size - 1] = argument;
        return;
    }
}

AST *AST_new_expr(AST *expr)
{
    AST ast = { .tag = AST_EXPR, .data = { .AST_EXPR = { .expr = expr } } };
    return AST_new(ast);
}
AST *AST_new_return(AST *expr)
{
    AST ast = { .tag = AST_RETURN, .data = { .AST_RETURN = { .expr = expr } } };
    return AST_new(ast);
}
AST *AST_new_argument(char* name, AST *type)
{
    AST ast = { .tag = AST_ARGUMENT, .data = { .AST_ARGUMENT = { .name = name, .type = type } } };
    return AST_new(ast);
}
AST *AST_new_function(char* name, AST *type, AST *body, AST* *arguments, size_t array_size)
{
    AST ast = { .tag = AST_FUNCTION, .data = { .AST_FUNCTION = { .name = name, .type = type, .body = body, .arguments = arguments, .array_size = array_size } } };
    return AST_new(ast);
}
AST *AST_new_type(char* name, bool is_array, AST* array_size)
{
    AST ast = { .tag = AST_TYPE, .data = { .AST_TYPE = { .name = name, .is_array = is_array, .array_size = array_size } } };
    return AST_new(ast);
}
AST *AST_new_declaration(char* name, AST *type)
{
    AST ast = { .tag = AST_DECLARATION, .data = { .AST_DECLARATION = { .name = name, .type = type } } };
    return AST_new(ast);
}
AST *AST_new_assign(char* name, AST *value)
{
    AST ast = { .tag = AST_ASSIGN, .data = { .AST_ASSIGN = { .name = name, .value = value } } };
    return AST_new(ast);
}
AST *AST_new_if(AST *condition, AST *body)
{
    AST ast = { .tag = AST_IF, .data = { .AST_IF = { .condition = condition, .body = body } } };
    return AST_new(ast);
}
AST *AST_new_while(AST *condition, AST *body)
{
    AST ast = { .tag = AST_WHILE, .data = { .AST_WHILE = { .condition = condition, .body = body } } };
    return AST_new(ast);
}
AST *AST_new_for(AST *init, AST *condition, AST *increment, AST *body)
{
    AST ast = { .tag = AST_FOR, .data = { .AST_FOR = { .init = init, .condition = condition, .increment = increment, .body = body } } };
    return AST_new(ast);
}
AST *AST_new_break()
{
    AST ast = { .tag = AST_BREAK };
    return AST_new(ast);
}
AST *AST_new_noop()
{
    AST ast = { .tag = AST_NOOP };
    return AST_new(ast);
}
AST *AST_new_continue()
{
    // AST ast = { .tag = AST_CONTINUE, .data = { .AST_CONTINUE = { } } };
    AST ast = { .tag = AST_CONTINUE };
    return AST_new(ast);
}
AST *AST_new_call(char* name, AST* *arguments, size_t array_size)
{
    AST ast = { .tag = AST_CALL, .data = { .AST_CALL = { .name = name, .arguments = arguments, .array_size = array_size } } };
    return AST_new(ast);
}
AST *AST_new_variable(char* name, bool is_arg)
{
    AST ast = { .tag = AST_VARIABLE, .data = { .AST_VARIABLE = { .name = name, .is_arg = is_arg } } };
    return AST_new(ast);
}

AST *AST_new_single(AST_TAG tag, AST *expr)
{
    AST ast = { .tag = tag, .data = { .AST_EXPR = { .expr = expr } } };
    return AST_new(ast);
}
AST *AST_new_number(char* number)
{
    AST ast = { .tag = AST_NUMBER, .data = { .AST_NUMBER = { .number = number } } };
    return AST_new(ast);
}
AST *AST_new_tuple(AST_TAG tag, AST *left, AST *right, char* op)
{
    AST ast = { .tag = tag, .data = { .AST_TUPLE = { .left = left, .right = right, .op = op } } };
    return AST_new(ast);
}
void AST_print(AST *ast)
{
    switch (ast->tag)
    {
    case AST_NUMBER:
        printf("%s", ast->data.AST_NUMBER.number);
        break;
    case AST_BINARY_OP:
        printf("(");
        AST_print(ast->data.AST_TUPLE.left);
        printf("%s", ast->data.AST_TUPLE.op);
        AST_print(ast->data.AST_TUPLE.right);
        printf(")");
        break;
    case AST_EXPR:
        printf("(");
        AST_print(ast->data.AST_EXPR.expr);
        printf(")");
        break;
    case AST_RETURN:
        printf("return ");
        AST_print(ast->data.AST_RETURN.expr);
        break;
    case AST_ARGUMENT:
        AST_print(ast->data.AST_ARGUMENT.type);
        printf(" %s", ast->data.AST_ARGUMENT.name);
        break;
    case AST_FUNCTION:
        AST_print(ast->data.AST_FUNCTION.type);
        printf(" %s(", ast->data.AST_FUNCTION.name);
        for (size_t i = 0; i < ast->data.AST_FUNCTION.array_size; i++)
        {
            AST_print(ast->data.AST_FUNCTION.arguments[i]);
            if (i != ast->data.AST_FUNCTION.array_size - 1)
            {
                printf(", ");
            }
        }
        printf(") {\n");
        AST_print(ast->data.AST_FUNCTION.body);
        printf("}\n");
        break;
    case AST_NODE:
        for (size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
        {
            AST_print(ast->data.AST_NODE.children[i]);
            printf("\n");
        }
        break;
    case AST_TYPE:
        printf("%s", ast->data.AST_TYPE.name);
        if (ast->data.AST_TYPE.is_array)
        {
            printf("[");
            AST_print(ast->data.AST_TYPE.array_size);
            printf("]");
        }
        break;
    case AST_DECLARATION:
        AST_print(ast->data.AST_DECLARATION.type);
        printf(" %s;", ast->data.AST_DECLARATION.name);
        break;
    case AST_ASSIGN:
        printf("%s = ", ast->data.AST_ASSIGN.name);
        AST_print(ast->data.AST_ASSIGN.value);
        break;
    case AST_IF:
        printf("if (");
        AST_print(ast->data.AST_IF.condition);
        printf(") {\n");
        AST_print(ast->data.AST_IF.body);
        printf("}\n");
        break;
    case AST_WHILE:
        printf("while (");
        AST_print(ast->data.AST_WHILE.condition);
        printf(") {\n");
        AST_print(ast->data.AST_WHILE.body);
        printf("}\n");
        break;
    case AST_FOR:
        printf("for (");
        AST_print(ast->data.AST_FOR.init);
        printf("; ");
        AST_print(ast->data.AST_FOR.condition);
        printf("; ");
        AST_print(ast->data.AST_FOR.increment);
        printf(") {\n");
        AST_print(ast->data.AST_FOR.body);
        printf("}\n");
        break;
    case AST_BREAK:
        printf("break");
        break;
    case AST_CONTINUE:
        printf("continue");
        break;
    case AST_CALL:
        printf("%s(", ast->data.AST_CALL.name);
        for (size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
        {
            AST_print(ast->data.AST_CALL.arguments[i]);
            if (i != ast->data.AST_CALL.array_size - 1)
            {
                printf(", ");
            }
        }
        printf(")");
        break;
    case AST_VARIABLE:
        printf("%s", ast->data.AST_VARIABLE.name);
        break;
    default:
        printf("AST(print): Error: Unknown tag (%s)\n", get_tag_name(ast->tag));
        break;
    }
}
void AST_free(AST *ast)
{
    // still need to free something that i can't find (but its only 68 bytes by 34 leaks, so it should be fixable)
    switch (ast->tag)
    {
    case AST_RETURN:
        AST_free(ast->data.AST_RETURN.expr);
        if(DEBUG) printf("free return\n");
        break;
    case AST_NODE:
        if(DEBUG) printf("count: %ld\n", ast->data.AST_NODE.array_size);
        for (size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
        {
            AST_free(ast->data.AST_NODE.children[i]);
            // free(ast->data.AST_NODE.children[i]);
        }
        free(ast->data.AST_NODE.children);
        if(DEBUG) printf("free node\n");
        break;
    case AST_FUNCTION:
        AST_free(ast->data.AST_FUNCTION.type);
        AST_free(ast->data.AST_FUNCTION.body);
        for (size_t i = 0; i < ast->data.AST_FUNCTION.array_size; i++)
        {
            AST_free(ast->data.AST_FUNCTION.arguments[i]);
        }
        free(ast->data.AST_FUNCTION.arguments);
        if(DEBUG) printf("free function\n");
        break;
    case AST_CALL:
        for (size_t i = 0; i < ast->data.AST_CALL.array_size; i++)
        {
            AST_free(ast->data.AST_CALL.arguments[i]);
        }
        free(ast->data.AST_CALL.arguments);
        if(DEBUG) printf("free call\n");
        break;
    case AST_ARGUMENT:
        AST_free(ast->data.AST_ARGUMENT.type);
        if(DEBUG) printf("free argument\n");
        break;
    case AST_TYPE:
        if(DEBUG) printf("free type\n");
        break;
    case AST_DECLARATION:
        AST_free(ast->data.AST_DECLARATION.type);
        if(DEBUG) printf("free declaration\n");
        break;
    case AST_ASSIGN:
        AST_free(ast->data.AST_ASSIGN.value);
        if(DEBUG) printf("free assign\n");
        break;
    case AST_IF:
        AST_free(ast->data.AST_IF.condition);
        AST_free(ast->data.AST_IF.body);
        if(DEBUG) printf("free if\n");
        break;
    case AST_WHILE:
        AST_free(ast->data.AST_WHILE.condition);
        AST_free(ast->data.AST_WHILE.body);
        if(DEBUG) printf("free while\n");
        break;
    case AST_FOR:
        AST_free(ast->data.AST_FOR.init);
        AST_free(ast->data.AST_FOR.condition);
        AST_free(ast->data.AST_FOR.increment);
        AST_free(ast->data.AST_FOR.body);
        if(DEBUG) printf("free for\n");
        break;
    case AST_BINARY_OP:
        AST_free(ast->data.AST_TUPLE.left);
        AST_free(ast->data.AST_TUPLE.right);
        // free(ast->data.AST_TUPLE.op);
        if(DEBUG) printf("free tuple\n");
        break;
    case AST_EXPR:
        if(DEBUG) printf("free expr(%s)\n", get_tag_name(ast->data.AST_EXPR.expr->tag));
        AST_free(ast->data.AST_EXPR.expr);
        if(DEBUG) printf("free expr\n");
        break;
    default:
        break;
    }
    free(ast);
}
