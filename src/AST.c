#include "AST.h"

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
AST *AST_new_declaration(char* name, AST *type, AST *value)
{
    AST ast = { .tag = AST_DECLARATION, .data = { .AST_DECLARATION = { .name = name, .type = type, .value = value } } };
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
    AST ast = { .tag = AST_BREAK, .data = { .AST_BREAK = { } } };
    return AST_new(ast);
}
AST *AST_new_continue()
{
    AST ast = { .tag = AST_CONTINUE, .data = { .AST_CONTINUE = { } } };
    return AST_new(ast);
}
AST *AST_new_call(char* name, AST* *arguments, size_t array_size)
{
    AST ast = { .tag = AST_CALL, .data = { .AST_CALL = { .name = name, .arguments = arguments, .array_size = array_size } } };
    return AST_new(ast);
}

AST *AST_new_single(AST_TAG tag, AST *expr)
{
    AST ast = { .tag = tag, .data = { .AST_EXPR = { .expr = expr } } };
    return AST_new(ast);
}
AST *AST_new_number(int number)
{
    AST ast = { .tag = AST_NUMBER, .data = { .AST_NUMBER = { .number = number } } };
    return AST_new(ast);
}
AST *AST_new_tuple(AST_TAG tag, AST *left, AST *right)
{
    AST ast = { .tag = tag, .data = { .AST_TUPLE = { .left = left, .right = right } } };
    return AST_new(ast);
}
void AST_print(AST *ast)
{
    switch (ast->tag)
    {
    case AST_NUMBER:
        printf("%d", ast->data.AST_NUMBER.number);
        break;
    case AST_ADD:
    case AST_SUB:
    case AST_DIV:
    case AST_MUL:
        printf("(");
        AST_print(ast->data.AST_TUPLE.left);
        printf("%c", get_op(ast->tag));
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
    default:
        printf("AST(print): Error: Unknown tag\n");
        break;
    }
}
void AST_free(AST *ast)
{
//     switch (ast->tag)
//     {
//     case AST_NUMBER:
//         break;
//     case AST_ADD:
//     case AST_SUB:
//     case AST_DIV:
//     case AST_MUL:
//         AST_free(ast->data.AST_TUPLE.left);
//         AST_free(ast->data.AST_TUPLE.right);
//         break;
//     case AST_EXPR:
//         AST_free(ast->data.AST_EXPR.expr);
//         break;
//     case AST_RETURN:
//         AST_free(ast->data.AST_RETURN.expr);
//         break;
//     case AST_ARGUMENT:
//         AST_free(ast->data.AST_ARGUMENT.type);
//         break;
//     case AST_FUNCTION:
//         AST_free(ast->data.AST_FUNCTION.type);
//         AST_free(ast->data.AST_FUNCTION.body);
//         for (size_t i = 0; i < ast->data.AST_FUNCTION.array_size; i++)
//         {
//             AST_free(ast->data.AST_FUNCTION.arguments[i]);
//         }
//         free(ast->data.AST_FUNCTION.arguments);
//         break;
//     case AST_NODE:
//         for (size_t i = 0; i < ast->data.AST_NODE.array_size; i++)
//         {
//             AST_free(ast->data.AST_NODE.children[i]);
//         }
//         free(ast->data.AST_NODE.children);
//         break;
//     case AST_TYPE:
//         break;
//     case AST_DECLARATION:
//         AST_free(ast->data.AST_DECLARATION.type);
//         AST_free(ast->data.AST_DECLARATION.value);
//         break;
//     case AST_ASSIGN:
//         AST_free(ast->data.AST_ASSIGN.value);
//         break;
//     default:
//         printf("AST(free): Error: Unknown tag (%s)\n", get_tag_name(ast->tag));
//         break;
//     }
    free(ast);
}
