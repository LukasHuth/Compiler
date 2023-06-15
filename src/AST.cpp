#include "headers/AST.h"

namespace Ast
{

    const bool DEBUG = false;
    AST *new_node()
    {
        AST *ast = new AST;
        ast->tag = NODE;
        ast->data.NODE = { .children = (AST**) calloc(sizeof(AST), 0), .array_size = 0 };
        // AST ast = { .tag = NODE, .data = { .NODE = { .children = (AST**) calloc(sizeof(AST), 0), .array_size = 0 } } };
        return ast;
    }

    void add_child(AST* ast, AST* child)
    {
        // not finished
        if (ast->tag == NODE)
        {
            ast->data.NODE.array_size++;
            ast->data.NODE.children = (AST**) realloc(ast->data.NODE.children, sizeof(AST*) * (ast->data.NODE.array_size));
            ast->data.NODE.children[ast->data.NODE.array_size - 1] = child;
            return;
        }
    }
    void add_argument(AST* ast, AST* argument)
    {
        if(ast->tag == FUNCTION)
        {
            ast->data.FUNCTION.array_size++;
            ast->data.FUNCTION.arguments = (AST**) realloc(ast->data.FUNCTION.arguments, sizeof(AST*) * (ast->data.FUNCTION.array_size));
            ast->data.FUNCTION.arguments[ast->data.FUNCTION.array_size - 1] = argument;
            return;
        }
        if(ast->tag == CALL)
        {
            ast->data.CALL.array_size++;
            ast->data.CALL.arguments = (AST**) realloc(ast->data.CALL.arguments, sizeof(AST*) * (ast->data.CALL.array_size));
            ast->data.CALL.arguments[ast->data.CALL.array_size - 1] = argument;
            return;
        }
    }

    AST *new_expr(AST *expr)
    {
        AST *ast = new AST;
        ast->tag = EXPR;
        ast->data.EXPR = { .expr = expr };
        // AST ast = { .tag = EXPR, .data = { .EXPR = { .expr = expr } } };
        return ast;
    }
    AST *new_return(AST *expr)
    {
        AST *ast = new AST;
        ast->tag = RETURN;
        ast->data.RETURN = { .expr = expr };
        // AST ast = { .tag = RETURN, .data = { .RETURN = { .expr = expr } } };
        return ast;
    }
    AST *new_argument(char* name, AST *type)
    {
        AST *ast = new AST;
        ast->tag = ARGUMENT;
        ast->data.ARGUMENT = { .name = name, .type = type };
        // AST ast = { .tag = ARGUMENT, .data = { .ARGUMENT = { .name = name, .type = type } } };
        return ast;
    }
    AST *new_function(char* name, AST *type, AST *body, AST* *arguments, size_t array_size)
    {
        AST *ast = new AST;
        ast->tag = FUNCTION;
        ast->data.FUNCTION = { .name = name, .type = type, .body = body, .arguments = arguments, .array_size = array_size };
        // AST ast = { .tag = FUNCTION, .data = { .FUNCTION = { .name = name, .type = type, .body = body, .arguments = arguments, .array_size = array_size } } };
        return ast;
    }
    AST *new_type(char* name, bool is_array, AST* array_size)
    {
        AST *ast = new AST;
        ast->tag = TYPE;
        ast->data.TYPE = { .name = name, .is_array = is_array, .array_size = array_size };
        // AST ast = { .tag = TYPE, .data = { .TYPE = { .name = name, .is_array = is_array, .array_size = array_size } } };
        return ast;
    }
    AST *new_declaration(char* name, AST *type)
    {
        AST *ast = new AST;
        ast->tag = DECLARATION;
        ast->data.DECLARATION = { .name = name, .type = type };
        // AST ast = { .tag = DECLARATION, .data = { .DECLARATION = { .name = name, .type = type } } };
        return ast;
    }
    AST *new_assign(char* name, AST *value)
    {
        AST *ast = new AST;
        ast->tag = ASSIGN;
        ast->data.ASSIGN = { .name = name, .value = value };
        // AST ast = { .tag = ASSIGN, .data = { .ASSIGN = { .name = name, .value = value } } };
        return ast;
    }
    AST *new_if(AST *condition, AST *body)
    {
        AST *ast = new AST;
        ast->tag = IF;
        ast->data.IF = { .condition = condition, .body = body };
        // AST ast = { .tag = IF, .data = { .IF = { .condition = condition, .body = body } } };
        return ast;
    }
    AST *new_while(AST *condition, AST *body)
    {
        AST *ast = new AST;
        ast->tag = WHILE;
        ast->data.WHILE = { .condition = condition, .body = body };
        // AST ast = { .tag = WHILE, .data = { .WHILE = { .condition = condition, .body = body } } };
        return ast;
    }
    AST *new_for(AST *init, AST *condition, AST *increment, AST *body)
    {
        AST *ast = new AST;
        ast->tag = FOR;
        ast->data.FOR = { .init = init, .condition = condition, .increment = increment, .body = body };
        // AST ast = { .tag = FOR, .data = { .FOR = { .init = init, .condition = condition, .increment = increment, .body = body } } };
        return ast;
    }
    AST *new_break()
    {
        AST *ast = new AST;
        ast->tag = BREAK;
        // AST ast = { .tag = BREAK, .data = { NULL } };
        return ast;
    }
    AST *new_noop()
    {
        AST *ast = new AST;
        ast->tag = NOOP;
        // AST ast = { .tag = NOOP, .data = { NULL } };
        return ast;
    }
    AST *new_continue()
    {
        AST *ast = new AST;
        ast->tag = CONTINUE;
        // AST ast = { .tag = CONTINUE, .data = { NULL } };
        return ast;
    }
    AST *new_call(char* name, AST* *arguments, size_t array_size)
    {
        AST *ast = new AST;
        ast->tag = CALL;
        ast->data.CALL = { .name = name, .arguments = arguments, .array_size = array_size };
        // AST ast = { .tag = CALL, .data = { .CALL = { .name = name, .arguments = arguments, .array_size = array_size } } };
        return ast;
    }
    AST *new_variable(char* name, bool is_arg, int arg_index)
    {
        AST *ast = new AST;
        ast->tag = VARIABLE;
        ast->data.VARIABLE = { .name = name, .is_arg = is_arg, .arg_index = arg_index };
        // AST ast = { .tag = VARIABLE, .data = { .VARIABLE = { .name = name, .is_arg = is_arg, .arg_index = arg_index } } };
        return ast;
    }

    AST *new_single(Tag tag, AST *expr)
    {
        AST *ast = new AST;
        ast->tag = tag;
        ast->data.EXPR = { .expr = expr };
        return ast;
    }
    AST *new_number(char* number)
    {
        AST *ast = new AST;
        ast->tag = NUMBER;
        ast->data.NUMBER = { .number = number };
        // AST ast = { .tag = NUMBER, .data = { .NUMBER = { .number = number } } };
        return ast;
    }
    AST *new_tuple(Tag tag, AST *left, AST *right, char* op)
    {
        AST *ast = new AST;
        ast->tag = tag;
        ast->data.TUPLE = { .left = left, .right = right, .op = op };
        // AST ast = { .tag = tag, .data = { .TUPLE = { .left = left, .right = right, .op = op } } };
        return ast;
    }
    void print(AST *ast)
    {
        switch (ast->tag)
        {
        case NUMBER:
            printf("%s", ast->data.NUMBER.number);
            break;
        case BINARY_OP:
            printf("(");
            print(ast->data.TUPLE.left);
            printf("%s", ast->data.TUPLE.op);
            print(ast->data.TUPLE.right);
            printf(")");
            break;
        case EXPR:
            printf("(");
            print(ast->data.EXPR.expr);
            printf(")");
            break;
        case RETURN:
            printf("return ");
            print(ast->data.RETURN.expr);
            break;
        case ARGUMENT:
            print(ast->data.ARGUMENT.type);
            printf(" %s", ast->data.ARGUMENT.name);
            break;
        case FUNCTION:
            print(ast->data.FUNCTION.type);
            printf(" %s(", ast->data.FUNCTION.name);
            for (size_t i = 0; i < ast->data.FUNCTION.array_size; i++)
            {
                print(ast->data.FUNCTION.arguments[i]);
                if (i != ast->data.FUNCTION.array_size - 1)
                {
                    printf(", ");
                }
            }
            printf(") {\n");
            print(ast->data.FUNCTION.body);
            printf("}\n");
            break;
        case NODE:
            for (size_t i = 0; i < ast->data.NODE.array_size; i++)
            {
                print(ast->data.NODE.children[i]);
                printf("\n");
            }
            break;
        case TYPE:
            printf("%s", ast->data.TYPE.name);
            if (ast->data.TYPE.is_array)
            {
                printf("[");
                print(ast->data.TYPE.array_size);
                printf("]");
            }
            break;
        case DECLARATION:
            print(ast->data.DECLARATION.type);
            printf(" %s;", ast->data.DECLARATION.name);
            break;
        case ASSIGN:
            printf("%s = ", ast->data.ASSIGN.name);
            print(ast->data.ASSIGN.value);
            break;
        case IF:
            printf("if (");
            print(ast->data.IF.condition);
            printf(") {\n");
            print(ast->data.IF.body);
            printf("}\n");
            break;
        case WHILE:
            printf("while (");
            print(ast->data.WHILE.condition);
            printf(") {\n");
            print(ast->data.WHILE.body);
            printf("}\n");
            break;
        case FOR:
            printf("for (");
            print(ast->data.FOR.init);
            printf("; ");
            print(ast->data.FOR.condition);
            printf("; ");
            print(ast->data.FOR.increment);
            printf(") {\n");
            print(ast->data.FOR.body);
            printf("}\n");
            break;
        case BREAK:
            printf("break");
            break;
        case CONTINUE:
            printf("continue");
            break;
        case CALL:
            printf("%s(", ast->data.CALL.name);
            for (size_t i = 0; i < ast->data.CALL.array_size; i++)
            {
                print(ast->data.CALL.arguments[i]);
                if (i != ast->data.CALL.array_size - 1)
                {
                    printf(", ");
                }
            }
            printf(")");
            break;
        case VARIABLE:
            printf("%s", ast->data.VARIABLE.name);
            break;
        default:
            printf("AST(print): Error: Unknown tag (%s)\n", get_tag_name(ast->tag).c_str());
            break;
        }
    }
    void Free(AST *ast)
    {
        // still need to free something that i can't find (but its only 68 bytes by 34 leaks, so it should be fixable)
        switch (ast->tag)
        {
        case RETURN:
            Free(ast->data.RETURN.expr);
            if(DEBUG) printf("free return\n");
            break;
        case NODE:
            if(DEBUG) printf("count: %ld\n", ast->data.NODE.array_size);
            for (size_t i = 0; i < ast->data.NODE.array_size; i++)
            {
                Free(ast->data.NODE.children[i]);
                // free(ast->data.NODE.children[i]);
            }
            free(ast->data.NODE.children);
            if(DEBUG) printf("free node\n");
            break;
        case FUNCTION:
            Free(ast->data.FUNCTION.type);
            Free(ast->data.FUNCTION.body);
            for (size_t i = 0; i < ast->data.FUNCTION.array_size; i++)
            {
                Free(ast->data.FUNCTION.arguments[i]);
            }
            free(ast->data.FUNCTION.arguments);
            if(DEBUG) printf("free function\n");
            break;
        case CALL:
            for (size_t i = 0; i < ast->data.CALL.array_size; i++)
            {
                Free(ast->data.CALL.arguments[i]);
            }
            free(ast->data.CALL.arguments);
            if(DEBUG) printf("free call\n");
            break;
        case ARGUMENT:
            Free(ast->data.ARGUMENT.type);
            if(DEBUG) printf("free argument\n");
            break;
        case TYPE:
            if(DEBUG) printf("free type\n");
            break;
        case DECLARATION:
            Free(ast->data.DECLARATION.type);
            if(DEBUG) printf("free declaration\n");
            break;
        case ASSIGN:
            Free(ast->data.ASSIGN.value);
            if(DEBUG) printf("free assign\n");
            break;
        case IF:
            Free(ast->data.IF.condition);
            Free(ast->data.IF.body);
            if(DEBUG) printf("free if\n");
            break;
        case WHILE:
            Free(ast->data.WHILE.condition);
            Free(ast->data.WHILE.body);
            if(DEBUG) printf("free while\n");
            break;
        case FOR:
            Free(ast->data.FOR.init);
            Free(ast->data.FOR.condition);
            Free(ast->data.FOR.increment);
            Free(ast->data.FOR.body);
            if(DEBUG) printf("free for\n");
            break;
        case BINARY_OP:
            Free(ast->data.TUPLE.left);
            Free(ast->data.TUPLE.right);
            // free(ast->data.TUPLE.op);
            if(DEBUG) printf("free tuple\n");
            break;
        case EXPR:
            if(DEBUG) printf("free expr(%s)\n", get_tag_name(ast->data.EXPR.expr->tag).c_str());
            Free(ast->data.EXPR.expr);
            if(DEBUG) printf("free expr\n");
            break;
        default:
            break;
        }
        free(ast);
    }
}
