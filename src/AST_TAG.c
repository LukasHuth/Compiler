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
    default:
        printf("AST(get_tag_name): Error: Unknown tag\n");
        return "?";
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