#include "headers/AST_TAG.h"
char* get_tag_name(AST_TAG tag)
{
    switch (tag)
    {
    case AST_NUMBER:
        return "AST_NUMBER";
    case AST_BINARY_OP:
        return "AST_BINARY_OP";
    case AST_EXPR:
        return "AST_EXPR";
    case AST_RETURN:
        return "AST_RETURN";
    case AST_ARGUMENT:
        return "AST_ARGUMENT";
    case AST_FUNCTION:
        return "AST_FUNCTION";
    case AST_NODE:
        return "AST_NODE";
    case AST_TYPE:
        return "AST_TYPE";
    case AST_DECLARATION:
        return "AST_DECLARATION";
    case AST_ASSIGN:
        return "AST_ASSIGN";
    case AST_IF:
        return "AST_IF";
    case AST_WHILE:
        return "AST_WHILE";
    case AST_FOR:
        return "AST_FOR";
    case AST_BREAK:
        return "AST_BREAK";
    case AST_CONTINUE:
        return "AST_CONTINUE";
    case AST_CALL:
        return "AST_CALL";
    case AST_VARIABLE:
        return "AST_VARIABLE";
    default:
        printf("AST(get_tag_name): Error: Unknown tag\n");
        return "?";
    }
}
int tag_get_priority(char* tag)
{
    if (strcmp(tag, "==") == 0)
        return 1;
    if(strcmp(tag, "+") == 0 || strcmp(tag, "-") == 0)
        return 2;
    if(strcmp(tag, "*") == 0 || strcmp(tag, "/") == 0)
        return 3;
    return -1;
}