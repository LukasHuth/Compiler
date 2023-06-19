#include "headers/AST_TAG.h"
namespace Ast
{
    std::string get_tag_name(Tag tag)
    {
        switch (tag)
        {
        case NUMBER:
            return "NUMBER";
        case BINARY_OP:
            return "BINARY_OP";
        case EXPR:
            return "EXPR";
        case RETURN:
            return "RETURN";
        case ARGUMENT:
            return "ARGUMENT";
        case FUNCTION:
            return "FUNCTION";
        case NODE:
            return "NODE";
        case TYPE:
            return "TYPE";
        case DECLARATION:
            return "DECLARATION";
        case ASSIGN:
            return "ASSIGN";
        case IF:
            return "IF";
        case WHILE:
            return "WHILE";
        case FOR:
            return "FOR";
        case BREAK:
            return "BREAK";
        case CONTINUE:
            return "CONTINUE";
        case CALL:
            return "CALL";
        case VARIABLE:
            return "VARIABLE";
        case NOOP:
            return "NOOP";
        default:
            printf("AST(get_tag_name): Error: Unknown tag\n");
            return "?";
        }
    }
    int tag_get_priority(std::string tag)
    {
        if (tag == "==")
            return 1;
        if(tag == "+" || tag == "-")
            return 2;
        if(tag == "*" || tag == "/")
            return 3;
        return -1;
    }
}