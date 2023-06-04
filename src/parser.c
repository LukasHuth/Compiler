#include "headers/parser.h"

AST *parse_call(LEXER *lexer, char *name, AST* function);
AST *parse_body(LEXER *lexer, AST* function);
AST *parse_expr(LEXER *lexer, AST* function);
AST *parse_type(LEXER *lexer, AST* function);
AST *parse_argument(LEXER *lexer);
AST *parse_term(LEXER *lexer, AST* function);
AST *parse_factor(LEXER *lexer, AST* function);
AST *parse_func(LEXER *lexer);
AST *parse_declaration(LEXER *lexer, char* name);
AST *parse_assignment(LEXER *lexer, char *name, AST* function);
AST *parse_for(LEXER *lexer, AST* function);
AST *parse_while(LEXER *lexer, AST* function);
AST *parse_if(LEXER *lexer, AST* function);
AST *parse_return(LEXER *lexer, AST* function);

AST* parse(LEXER* lexer)
{
    AST* ast = AST_new_node();
    while (lexer->index < lexer->buffer_size)
    {
        AST* expr = parse_func(lexer);
        AST_add_child(ast, expr);
    }
    return ast;
}

LEXER_TAG peek(LEXER* lexer)
{
    return lexer->tokens[lexer->index]->tag;
}

LEXER_TOKEN *eat(LEXER* lexer, LEXER_TAG tag)
{
    if (peek(lexer) == tag)
    {
        lexer->index++;
        return lexer->tokens[lexer->index - 1];
    }
    else
    {
        printf("Parser(eat): Error: Expected %s, got %s\n", LEXER_TAG_to_string(tag), LEXER_TAG_to_string(peek(lexer)));
        exit(1);
    }
}

AST *parse_func(LEXER *lexer)
{
    eat(lexer, LEXER_KEYWORD);
    char *name = eat(lexer, LEXER_IDENTIFIER)->data;
    eat(lexer, LEXER_OPEN_PAREN);
    AST** arguments = calloc(sizeof(AST*), 0);
    size_t array_size = 0;
    while (peek(lexer) != LEXER_CLOSE_PAREN)
    {
        AST* argument = parse_argument(lexer);
        arguments = realloc(arguments, sizeof(AST*) * (array_size + 1));
        arguments[array_size] = argument;
        array_size++;
        if (peek(lexer) == LEXER_COMMA)
        {
            eat(lexer, LEXER_COMMA);
        }
    }
    eat(lexer, LEXER_CLOSE_PAREN);
    eat(lexer, LEXER_COLON);
    AST *type = parse_type(lexer, NULL);
    eat(lexer, LEXER_ARROW);
    AST *function = AST_new_function(name, type, NULL, arguments, array_size);
    AST *body = parse_body(lexer, function);
    function->data.AST_FUNCTION.body = body;
    return function;
}

AST *parse_body(LEXER *lexer, AST* function)
{
    eat(lexer, LEXER_OPEN_BRACE);
    AST* ast = AST_new_node();
    while (peek(lexer) != LEXER_CLOSE_BRACE)
    {
        AST* expr = parse_expr(lexer, function);
        AST_add_child(ast, expr);
        if(peek(lexer) == LEXER_SEMICOLON)
        {
            eat(lexer, LEXER_SEMICOLON);
            continue;
        }
        if(expr->tag != AST_DECLARATION) continue;
        AST *assignment = parse_assignment(lexer, expr->data.AST_DECLARATION.name, function);
        AST_add_child(ast, assignment);
        eat(lexer, LEXER_SEMICOLON);
    }
    eat(lexer, LEXER_CLOSE_BRACE);
    return ast;
}

AST *parse_expr(LEXER *lexer, AST* function)
{
    if(peek(lexer) == LEXER_IDENTIFIER)
    {
        char* name = eat(lexer, LEXER_IDENTIFIER)->data;
        if(peek(lexer) == LEXER_EQUALS)
        {
            return parse_assignment(lexer, name, function);
        }
        if(peek(lexer) == LEXER_OPEN_PAREN)
        {
            return parse_call(lexer, name, function);
        }
        if(peek(lexer) == LEXER_COLON)
        {
            return parse_declaration(lexer, name);
        }
        if(peek(lexer) == LEXER_OPEN_BRACKET)
        {
            // eat(lexer, LEXER_OPEN_BRACKET);
            // AST* index = parse_term(lexer);
            // eat(lexer, LEXER_CLOSE_BRACKET);
            // eat(lexer, LEXER_EQUALS);
            // AST* value = parse_term(lexer);
            // return AST_new_index_assign(name, index, value);
            printf("Parser(parse_expr): Error: Index assignment not implemented\n");
            exit(2);
        }
        // TODO: Implement index assignment and add support for -=, +=, *=, /=, etc.
        printf("Parser(parse_expr): Error: Unexpected token %s\n", LEXER_TAG_to_string(peek(lexer)));
    }
    if(peek(lexer) == LEXER_KEYWORD)
    {
        char* name = eat(lexer, LEXER_KEYWORD)->data;
        if(strcmp(name, "return") == 0)
        {
            return parse_return(lexer, function);
        }
        if(strcmp(name, "if") == 0)
        {
            return parse_if(lexer, function);
        }
        if(strcmp(name, "while") == 0)
        {
            return parse_while(lexer, function);
        }
        if(strcmp(name, "for") == 0)
        {
            return parse_for(lexer, function);
        }
        if(strcmp(name, "break") == 0)
        {
            // eat(lexer, LEXER_SEMICOLON);
            return AST_new_break();
        }
        if(strcmp(name, "continue") == 0)
        {
            // eat(lexer, LEXER_SEMICOLON);
            return AST_new_continue();
        }
        if(strcmp(name, "import") == 0)
        {
            printf("Parser(parse_expr): Error: Import not implemented\n");
            exit(2);
            // TODO: Implement import
            // char* path = eat(lexer, LEXER_STRING)->data;
            // eat(lexer, LEXER_SEMICOLON);
            // return AST_new_import(path);
        }
        if(strcmp(name, "struct") == 0)
        {
            printf("Parser(parse_expr): Error: Struct not implemented\n");
            exit(2);
            /*
            char* name = eat(lexer, LEXER_IDENTIFIER)->data;
            eat(lexer, LEXER_OPEN_BRACE);
            AST** members = calloc(sizeof(AST*), 0);
            size_t array_size = 0;
            while (peek(lexer) != LEXER_CLOSE_BRACE)
            {
                AST* member = parse_declaration(lexer, NULL);
                members = realloc(members, sizeof(AST*) * (array_size + 1));
                members[array_size] = member;
                array_size++;
            }
            eat(lexer, LEXER_CLOSE_BRACE);
            eat(lexer, LEXER_SEMICOLON);
            return AST_new_struct(name, members, array_size);
            */
        }
        if(strcmp(name, "enum") == 0)
        {
            printf("Parser(parse_expr): Error: Enum not implemented\n");
            exit(2);
            /*
            char* name = eat(lexer, LEXER_IDENTIFIER)->data;
            eat(lexer, LEXER_OPEN_BRACE);
            char** members = calloc(sizeof(char*), 0);
            size_t array_size = 0;
            while (peek(lexer) != LEXER_CLOSE_BRACE)
            {
                char* name = eat(lexer, LEXER_IDENTIFIER)->data;
                eat(lexer, LEXER_COMMA);
                members = realloc(members, sizeof(char*) * (array_size + 1));
                members[array_size] = member;
                array_size++;
            }
            eat(lexer, LEXER_CLOSE_BRACE);
            eat(lexer, LEXER_SEMICOLON);
            return AST_new_enum(name, members, array_size);
            */
        }
    }
    while(peek(lexer) != LEXER_SEMICOLON)
    {
        eat(lexer, peek(lexer));
    }
    eat(lexer, LEXER_SEMICOLON);
    return AST_new_noop();
}

AST *parse_for(LEXER *lexer, AST* function)
{
    lexer->index = lexer->index;
    function->tag = function->tag;
    printf("Parser(parse_for): Error: For not implemented\n");
    exit(2);
    /*
    eat(lexer, LEXER_OPEN_PAREN);
    AST* init = parse_expr(lexer);
    AST* condition = parse_term(lexer);
    eat(lexer, LEXER_SEMICOLON);
    AST* update = parse_expr(lexer);
    eat(lexer, LEXER_CLOSE_PAREN);
    AST* body = parse_body(lexer);
    return AST_new_for(init, condition, update, body);
    */
}

AST *parse_while(LEXER *lexer, AST* function)
{
    eat(lexer, LEXER_OPEN_PAREN);
    AST *condition = parse_term(lexer, function);
    eat(lexer, LEXER_CLOSE_PAREN);
    AST *body = parse_body(lexer, function);
    return AST_new_while(condition, body);
}

AST *parse_if(LEXER *lexer, AST* function)
{
    eat(lexer, LEXER_OPEN_PAREN);
    AST *condition = parse_term(lexer, function);
    eat(lexer, LEXER_CLOSE_PAREN);
    AST *body = parse_body(lexer, function);
    return AST_new_if(condition, body);
}

AST *parse_return(LEXER *lexer, AST* function)
{
    AST* value = parse_term(lexer, function);
    // AST_print(value);
    // eat(lexer, LEXER_SEMICOLON);
    return AST_new_return(value);
}

AST *parse_declaration(LEXER *lexer, char* name)
{
    eat(lexer, LEXER_COLON);
    AST* type = parse_type(lexer, NULL);
    return AST_new_declaration(name, type);
}

AST *parse_assignment(LEXER *lexer, char *name, AST* function)
{
    eat(lexer, LEXER_EQUALS);
    AST *value = parse_term(lexer, function);
    // eat(lexer, LEXER_SEMICOLON);
    return AST_new_assign(name, value);
}

AST *parse_argument(LEXER *lexer)
{
    char *name = eat(lexer, LEXER_IDENTIFIER)->data;
    eat(lexer, LEXER_COLON);
    AST *type = parse_type(lexer, NULL);
    return AST_new_argument(name, type);
}

AST *parse_type(LEXER *lexer, AST* function)
{
    char *name = eat(lexer, LEXER_KEYWORD)->data;
    if (peek(lexer) == LEXER_OPEN_BRACKET)
    {
        eat(lexer, LEXER_OPEN_BRACKET);
        AST *array_size = parse_term(lexer, function);
        eat(lexer, LEXER_CLOSE_BRACKET);
        return AST_new_type(name, true, array_size);
    }
    else
    {
        return AST_new_type(name, false, NULL);
    }
}

bool is_binary_op(LEXER_TAG tag)
{
    return tag == LEXER_PLUS || tag == LEXER_MINUS || tag == LEXER_STAR || tag == LEXER_SLASH;
}

AST *parse_term(LEXER *lexer, AST* function)
{
    AST *left = parse_factor(lexer, function);
    while (is_binary_op(peek(lexer)))
    {
        LEXER_TAG tag = peek(lexer);
        LEXER_TOKEN *token = eat(lexer, tag);
        if(left->tag == AST_BINARY_OP)
        {
            if(left->data.AST_TUPLE.op == token->data)
            {
                AST* right = parse_factor(lexer, function);
                left = AST_new_tuple(AST_BINARY_OP, left, right, token->data);
                continue;
            }
            if(tag_get_priority(left->data.AST_TUPLE.op) < tag_get_priority(token->data))
            {
                AST* right = parse_factor(lexer, function);
                AST* new_left = AST_new_tuple(AST_BINARY_OP, left->data.AST_TUPLE.left, right, token->data);
                left->data.AST_TUPLE.left = new_left;
                continue;
            }
        }
        AST *right = parse_factor(lexer, function);
        left = AST_new_tuple(AST_BINARY_OP, left, right, token->data);
    }
    return left;
}
bool is_arg(AST* function, char* name)
{
    if(function == NULL) return false;
    if(function->tag != AST_FUNCTION) return false;
    for(size_t i = 0; i < function->data.AST_FUNCTION.array_size; i++)
    {
        if(strcmp(function->data.AST_FUNCTION.arguments[i]->data.AST_ARGUMENT.name, name) == 0)
        {
            return true;
        }
    }
    return false;
}
AST *parse_factor(LEXER *lexer, AST* function)
{
    if (peek(lexer) == LEXER_NUMBER)
    {
        char* number = eat(lexer, LEXER_NUMBER)->data;
        return AST_new_number(number);
    }
    if (peek(lexer) == LEXER_OPEN_PAREN)
    {
        eat(lexer, LEXER_OPEN_PAREN);
        AST *expr = parse_term(lexer, function);
        eat(lexer, LEXER_CLOSE_PAREN);
        return expr;
    }
    if (peek(lexer) == LEXER_IDENTIFIER)
    {
        char *name = eat(lexer, LEXER_IDENTIFIER)->data;
        if (peek(lexer) == LEXER_OPEN_PAREN)
        {
            return parse_call(lexer, name, function);
        }
        return AST_new_variable(name, is_arg(function, name));
    }
    printf("Parser(parse_factor): Error: Unexpected token %s\n", LEXER_TAG_to_string(peek(lexer)));
    exit(1);
}
AST *parse_call(LEXER *lexer, char *name, AST* function)
{
    eat(lexer, LEXER_OPEN_PAREN);
    AST** arguments = calloc(sizeof(AST*), 0);
    size_t array_size = 0;
    while (peek(lexer) != LEXER_CLOSE_PAREN)
    {
        AST* argument = parse_term(lexer, function);
        arguments = realloc(arguments, sizeof(AST*) * (array_size + 1));
        arguments[array_size] = argument;
        array_size++;
        if (peek(lexer) == LEXER_COMMA)
        {
            eat(lexer, LEXER_COMMA);
        }
    }
    eat(lexer, LEXER_CLOSE_PAREN);
    // eat(lexer, LEXER_SEMICOLON);
    return AST_new_call(name, arguments, array_size);
}