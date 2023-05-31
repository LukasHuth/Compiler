#include "AST_TAG.h"
char* get_tag_name(AST_TAG tag)
{
    switch (tag)
    {
    case AST_NUMBER:
        return "AST_NUMBER";
    case AST_ADD:
        return "AST_ADD";
    case AST_SUB:
        return "AST_SUB";
    case AST_DIV:
        return "AST_DIV";
    case AST_MUL:
        return "AST_MUL";
    case AST_EXPR:
        return "AST_EXPR";
    case AST_RETURN:
        return "AST_RETURN";
    case AST_BODY:
        return "AST_BODY";
    case AST_ARGUMENT:
        return "AST_ARGUMENT";
    case AST_FUNCTION:
        return "AST_FUNCTION";
    default:
        printf("AST(get_tag_name): Error: Unknown tag\n");
        return "?";
    }
}
int tag_get_priority(AST_TAG tag)
{
    switch (tag)
    {
    case AST_ADD:
    case AST_SUB:
        return 1;
    case AST_DIV:
    case AST_MUL:
        return 2;
    default:
        return -1;
    }
}
char get_op(AST_TAG tag)
{
    switch (tag)
    {
    case AST_ADD:
        return '+';
    case AST_SUB:
        return '-';
    case AST_DIV:
        return '/';
    case AST_MUL:
        return '*';
    default:
        printf("AST(get_op): Error: Unknown tag\n");
        return '?';
    }
}