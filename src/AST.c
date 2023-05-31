#include "AST.h"

AST *AST_new(AST ast)
{
    AST *ptr = malloc(sizeof(AST));
    if (ptr) *ptr = ast;
    return ptr;
}

AST *AST_new_node()
{
    // not the finished version, this is just a placeholder
    AST ast = { .tag = AST_EXPR, .data = { .AST_EXPR = { .expr = NULL } } };
    return AST_new(ast);
}

void AST_add_child(AST* ast, AST* child)
{
    if (ast->tag == AST_BODY)
    {
        ast->data.AST_BODY.array_size++;
        ast->data.AST_BODY.children = realloc(ast->data.AST_BODY.children, sizeof(AST*) * (ast->data.AST_BODY.array_size));
        ast->data.AST_BODY.children[ast->data.AST_BODY.array_size - 1] = child;
        return;
    }
    
    // not finished
    if (ast->tag == AST_NODE)
    {
        ast->data.AST_NODE.array_size++;
        ast->data.AST_NODE.children = realloc(ast->data.AST_NODE.children, sizeof(AST*) * (ast->data.AST_NODE.array_size));
        ast->data.AST_NODE.children[ast->data.AST_NODE.array_size - 1] = child;
        return;
    }
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
    switch (ast->tag)
    {
    case AST_NUMBER:
        break;
    case AST_ADD:
    case AST_SUB:
    case AST_DIV:
    case AST_MUL:
        AST_free(ast->data.AST_TUPLE.left);
        AST_free(ast->data.AST_TUPLE.right);
        break;
    case AST_EXPR:
        AST_free(ast->data.AST_EXPR.expr);
        break;
    case AST_RETURN:
        AST_free(ast->data.AST_RETURN.expr);
        break;
    default:
        printf("AST(free): Error: Unknown tag (%s)\n", get_tag_name(ast->tag));
        break;
    }
    free(ast);
}
