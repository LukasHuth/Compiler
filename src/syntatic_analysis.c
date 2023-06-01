#include "syntatic_analysis.h"

#define STATE SYNTATIC_ANALYSIS_STATE

void syntatic_analysis_children(AST *ast);
void syntatic_analysis_function(AST *ast, STATE *state);
void syntatic_analysis_body(AST *ast, STATE *state);
void syntatic_analysis_while(AST *ast, STATE *state);
void syntatic_analysis_if(AST *ast, STATE *state);

void syntatic_analysis(AST *node)
{
    if(node == NULL)
        return;
    if(node->tag != AST_NODE)
    {
        printf("ERROR: node is not a AST_NODE\n");
        exit(10); // ERROR CODE 10
    }
    for(size_t i = 0; i < node->data.AST_NODE.array_size; i++)
        syntatic_analysis_children(node->data.AST_NODE.children[i]);
}
void syntatic_analysis_children(AST *ast)
{
    if(ast == NULL)
        return;
    if(ast->tag == AST_IMPORT)
        return;
    if(ast->tag != AST_FUNCTION)
    {
        printf("ERROR: node is not a AST_FUNCTION\n");
        exit(11); // ERROR CODE 11
    }
    STATE *state = calloc(1, sizeof(STATE));
    state->in_function = true;
    state->returned = false;
    state->whilecount = 0;
    state->forcount = 0;
    state->ifcount = 0;
    state->in_else = false;
    syntatic_analysis_function(ast, state);
    free(state);
}
void syntatic_analysis_function(AST *ast, STATE *state)
{
    if(ast == NULL)
        return;
    if(ast->tag != AST_FUNCTION)
    {
        printf("ERROR: node is not a AST_FUNCTION\n");
        exit(11); // ERROR CODE 11
    }
    if(ast->data.AST_FUNCTION.name == NULL)
    {
        printf("ERROR: function name is NULL\n");
        exit(12); // ERROR CODE 12
    }
    if(ast->data.AST_FUNCTION.type == NULL)
    {
        printf("ERROR: function type is NULL\n");
        exit(13); // ERROR CODE 13
    }
    if(ast->data.AST_FUNCTION.body == NULL)
    {
        printf("ERROR: function body is NULL\n");
        exit(14); // ERROR CODE 14
    }
    if(ast->data.AST_FUNCTION.arguments == NULL)
    {
        printf("ERROR: function arguments is NULL\n");
        exit(15); // ERROR CODE 15
    }
    AST *body = ast->data.AST_FUNCTION.body;
    if(body->tag != AST_NODE)
    {
        printf("ERROR: function body is not a AST_NODE\n");
        exit(16); // ERROR CODE 16
    }
    for(size_t i = 0; i < body->data.AST_NODE.array_size; i++)
        syntatic_analysis_body(body->data.AST_NODE.children[i], state);
}
void syntatic_analysis_body(AST *ast, STATE *state)
{
    // TODO: variable type checking
    if(state->returned)
    {
        printf("ERROR: code after return statement\n");
        exit(17); // ERROR CODE 17
    }
    if(ast == NULL)
        return;
    if(ast->tag == AST_DECLARATION)
        return;
    if(ast->tag == AST_ASSIGN)
        return;
    if(ast->tag == AST_RETURN)
    {
        state->returned = true;
        return;
    }
    if(ast->tag == AST_IF)
    {
        syntatic_analysis_if(ast, state);
        return;
    }
    if(ast->tag == AST_WHILE)
    {
        syntatic_analysis_while(ast, state);
        return;
    }
    if(ast->tag == AST_FOR)
        return;
    if(ast->tag == AST_BREAK)
    {
        if(state->whilecount == 0 && state->forcount == 0)
        {
            printf("ERROR: break statement outside of loop\n");
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == AST_CONTINUE)
    {
        if(state->whilecount == 0 && state->forcount == 0)
        {
            printf("ERROR: continue statement outside of loop\n");
            exit(17); // ERROR CODE 17
        }
        return;
    }
    if(ast->tag == AST_CALL)
        return;
    if(ast->tag != AST_NODE)
    {
        printf("ERROR: node is not a AST_NODE\n");
        exit(17); // ERROR CODE 17
    }
}
void syntatic_analysis_while(AST *ast, STATE *state)
{
    if(ast == NULL)
        return;
    if(ast->tag != AST_WHILE)
    {
        printf("ERROR: node is not a AST_WHILE\n");
        exit(18); // ERROR CODE 18
    }
    if(ast->data.AST_WHILE.condition == NULL)
    {
        printf("ERROR: while condition is NULL\n");
        exit(19); // ERROR CODE 19
    }
    if(ast->data.AST_WHILE.body == NULL)
    {
        printf("ERROR: while body is NULL\n");
        exit(20); // ERROR CODE 20
    }
    AST *body = ast->data.AST_WHILE.body;
    if(body->tag != AST_NODE)
    {
        printf("ERROR: while body is not a AST_NODE\n");
        exit(21); // ERROR CODE 21
    }
    state->whilecount++;
    for(size_t i = 0; i < body->data.AST_NODE.array_size; i++)
        syntatic_analysis_body(body->data.AST_NODE.children[i], state);
    state->whilecount--;
}
void syntatic_analysis_if(AST *ast, STATE *state)
{
    if(ast == NULL)
        return;
    if(ast->tag != AST_IF)
    {
        printf("ERROR: node is not a AST_IF\n");
        exit(18); // ERROR CODE 18
    }
    if(ast->data.AST_IF.condition == NULL)
    {
        printf("ERROR: if condition is NULL\n");
        exit(19); // ERROR CODE 19
    }
    if(ast->data.AST_IF.body == NULL)
    {
        printf("ERROR: if body is NULL\n");
        exit(20); // ERROR CODE 20
    }
    AST *body = ast->data.AST_IF.body;
    if(body->tag != AST_NODE)
    {
        printf("ERROR: if body is not a AST_NODE\n");
        exit(21); // ERROR CODE 21
    }
    state->ifcount++;
    for(size_t i = 0; i < body->data.AST_NODE.array_size; i++)
    {
        syntatic_analysis_body(body->data.AST_NODE.children[i], state);
        state->returned = false;
    }
    state->ifcount--;
    // add else body option
    /*
    if(ast->data.AST_IF.else_body != NULL)
    {
        AST *else_body = ast->data.AST_IF.else_body;
        if(else_body->tag != AST_NODE)
        {
            printf("ERROR: else body is not a AST_NODE\n");
            exit(22); // ERROR CODE 22
        }
        for(size_t i = 0; i < else_body->data.AST_NODE.array_size; i++)
            syntatic_analysis_if_body(else_body->data.AST_NODE.children[i], state);
    }
    */
}