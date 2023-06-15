#include "headers/parser.h"


namespace Parser
{

    AST *parse_call(Lexer::Lexer *lexer, char *name, AST* function);
    AST *parse_body(Lexer::Lexer *lexer, AST* function);
    AST *parse_expr(Lexer::Lexer *lexer, AST* function);
    AST *parse_type(Lexer::Lexer *lexer, AST* function);
    AST *parse_argument(Lexer::Lexer *lexer);
    AST *parse_term(Lexer::Lexer *lexer, AST* function);
    AST *parse_factor(Lexer::Lexer *lexer, AST* function);
    AST *parse_func(Lexer::Lexer *lexer);
    AST *parse_declaration(Lexer::Lexer *lexer, char* name);
    AST *parse_assignment(Lexer::Lexer *lexer, char *name, AST* function);
    AST *parse_for(Lexer::Lexer *lexer, AST* function);
    AST *parse_while(Lexer::Lexer *lexer, AST* function);
    AST *parse_if(Lexer::Lexer *lexer, AST* function);
    AST *parse_return(Lexer::Lexer *lexer, AST* function);

    AST* parse(Lexer::Lexer *lexer)
    {
        AST* ast = Ast::new_node();
        while (lexer->index < lexer->buffer_size)
        {
            AST* expr = parse_func(lexer);
            Ast::add_child(ast, expr);
        }
        return ast;
    }
    Lexer::Tag peek(Lexer::Lexer* lexer)
    {
        return lexer->tokens[lexer->index]->tag;
    }
    Lexer::Token *eat(Lexer::Lexer* lexer, Lexer::Tag tag)
    {
        if (peek(lexer) == tag)
        {
            lexer->index++;
            return lexer->tokens[lexer->index - 1];
        }
        else
        {
            printf("Parser(eat): Error: Expected %s, got %s\n", Lexer::Tag_to_string(tag).c_str(), Lexer::Tag_to_string(peek(lexer)).c_str());
            exit(1);
        }
    }

    AST *parse_func(Lexer::Lexer *lexer)
    {
        eat(lexer, Lexer::KEYWORD);
        char *name = eat(lexer, Lexer::IDENTIFIER)->data;
        eat(lexer, Lexer::OPEN_PAREN);
        AST** arguments = (AST**) calloc(sizeof(AST*), 0);
        size_t array_size = 0;
        while (peek(lexer) != Lexer::CLOSE_PAREN)
        {
            AST* argument = parse_argument(lexer);
            arguments = (AST**) realloc(arguments, sizeof(AST*) * (array_size + 1));
            arguments[array_size] = argument;
            array_size++;
            if (peek(lexer) == Lexer::COMMA)
            {
                eat(lexer, Lexer::COMMA);
            }
        }
        eat(lexer, Lexer::CLOSE_PAREN);
        eat(lexer, Lexer::COLON);
        AST *type = parse_type(lexer, NULL);
        eat(lexer, Lexer::ARROW);
        AST *function = Ast::new_function(name, type, NULL, arguments, array_size);
        AST *body = parse_body(lexer, function);
        function->data.FUNCTION.body = body;
        return function;
    }

    AST *parse_body(Lexer::Lexer *lexer, AST* function)
    {
        eat(lexer, Lexer::OPEN_BRACE);
        AST* ast = Ast::new_node();
        while (peek(lexer) != Lexer::CLOSE_BRACE)
        {
            AST* expr = parse_expr(lexer, function);
            Ast::add_child(ast, expr);
            if(peek(lexer) == Lexer::SEMICOLON)
            {
                eat(lexer, Lexer::SEMICOLON);
                continue;
            }
            if(expr->tag != Ast::DECLARATION) continue;
            AST *assignment = parse_assignment(lexer, expr->data.DECLARATION.name, function);
            Ast::add_child(ast, assignment);
            eat(lexer, Lexer::SEMICOLON);
        }
        eat(lexer, Lexer::CLOSE_BRACE);
        return ast;
    }

    AST *parse_expr(Lexer::Lexer *lexer, AST* function)
    {
        if(peek(lexer) == Lexer::IDENTIFIER)
        {
            char* name = eat(lexer, Lexer::IDENTIFIER)->data;
            if(peek(lexer) == Lexer::EQUALS)
            {
                return parse_assignment(lexer, name, function);
            }
            if(peek(lexer) == Lexer::OPEN_PAREN)
            {
                return parse_call(lexer, name, function);
            }
            if(peek(lexer) == Lexer::COLON)
            {
                return parse_declaration(lexer, name);
            }
            if(peek(lexer) == Lexer::OPEN_BRACKET)
            {
                printf("Parser(parse_expr): Error: Index assignment not implemented\n");
                exit(2);
            }
            // TODO: Implement index assignment and add support for -=, +=, *=, /=, etc.
            printf("Parser(parse_expr): Error: Unexpected token %s\n", Lexer::Tag_to_string(peek(lexer)).c_str());
        }
        if(peek(lexer) == Lexer::KEYWORD)
        {
            char* name = eat(lexer, Lexer::KEYWORD)->data;
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
                // eat(lexer, Lexer::SEMICOLON);
                return Ast::new_break();
            }
            if(strcmp(name, "continue") == 0)
            {
                // eat(lexer, Lexer::SEMICOLON);
                return Ast::new_continue();
            }
            if(strcmp(name, "import") == 0)
            {
                printf("Parser(parse_expr): Error: Import not implemented\n");
                exit(2);
                // TODO: Implement import
                // char* path = eat(lexer, Lexer::STRING)->data;
                // eat(lexer, Lexer::SEMICOLON);
                // return AST_new_import(path);
            }
            if(strcmp(name, "struct") == 0)
            {
                printf("Parser(parse_expr): Error: Struct not implemented\n");
                exit(2);
                /*
                char* name = eat(lexer, Lexer::IDENTIFIER)->data;
                eat(lexer, Lexer::OPEN_BRACE);
                AST** members = calloc(sizeof(AST*), 0);
                size_t array_size = 0;
                while (peek(lexer) != Lexer::CLOSE_BRACE)
                {
                    AST* member = parse_declaration(lexer, NULL);
                    members = realloc(members, sizeof(AST*) * (array_size + 1));
                    members[array_size] = member;
                    array_size++;
                }
                eat(lexer, Lexer::CLOSE_BRACE);
                eat(lexer, Lexer::SEMICOLON);
                return AST_new_struct(name, members, array_size);
                */
            }
            if(strcmp(name, "enum") == 0)
            {
                printf("Parser(parse_expr): Error: Enum not implemented\n");
                exit(2);
                /*
                char* name = eat(lexer, Lexer::IDENTIFIER)->data;
                eat(lexer, Lexer::OPEN_BRACE);
                char** members = calloc(sizeof(char*), 0);
                size_t array_size = 0;
                while (peek(lexer) != Lexer::CLOSE_BRACE)
                {
                    char* name = eat(lexer, Lexer::IDENTIFIER)->data;
                    eat(lexer, Lexer::COMMA);
                    members = realloc(members, sizeof(char*) * (array_size + 1));
                    members[array_size] = member;
                    array_size++;
                }
                eat(lexer, Lexer::CLOSE_BRACE);
                eat(lexer, Lexer::SEMICOLON);
                return AST_new_enum(name, members, array_size);
                */
            }
        }
        while(peek(lexer) != Lexer::SEMICOLON)
        {
            eat(lexer, peek(lexer));
        }
        eat(lexer, Lexer::SEMICOLON);
        return Ast::new_noop();
    }

    AST *parse_for(Lexer::Lexer *lexer, AST* function)
    {
        lexer->index = lexer->index;
        function->tag = function->tag;
        printf("Parser(parse_for): Error: For not implemented\n");
        exit(2);
        /*
        eat(lexer, Lexer::OPEN_PAREN);
        AST* init = parse_expr(lexer);
        AST* condition = parse_term(lexer);
        eat(lexer, Lexer::SEMICOLON);
        AST* update = parse_expr(lexer);
        eat(lexer, Lexer::CLOSE_PAREN);
        AST* body = parse_body(lexer);
        return AST_new_for(init, condition, update, body);
        */
    }

    AST *parse_while(Lexer::Lexer *lexer, AST* function)
    {
        eat(lexer, Lexer::OPEN_PAREN);
        AST *condition = parse_term(lexer, function);
        eat(lexer, Lexer::CLOSE_PAREN);
        AST *body = parse_body(lexer, function);
        return Ast::new_while(condition, body);
    }

    AST *parse_if(Lexer::Lexer *lexer, AST* function)
    {
        eat(lexer, Lexer::OPEN_PAREN);
        AST *condition = parse_term(lexer, function);
        eat(lexer, Lexer::CLOSE_PAREN);
        AST *body = parse_body(lexer, function);
        return Ast::new_if(condition, body);
    }

    AST *parse_return(Lexer::Lexer *lexer, AST* function)
    {
        AST* value = parse_term(lexer, function);
        // AST_print(value);
        // eat(lexer, Lexer::SEMICOLON);
        return Ast::new_return(value);
    }

    AST *parse_declaration(Lexer::Lexer *lexer, char* name)
    {
        eat(lexer, Lexer::COLON);
        AST* type = parse_type(lexer, NULL);
        return Ast::new_declaration(name, type);
    }

    AST *parse_assignment(Lexer::Lexer *lexer, char *name, AST* function)
    {
        eat(lexer, Lexer::EQUALS);
        AST *value = parse_term(lexer, function);
        // eat(lexer, Lexer::SEMICOLON);
        return Ast::new_assign(name, value);
    }

    AST *parse_argument(Lexer::Lexer *lexer)
    {
        char *name = eat(lexer, Lexer::IDENTIFIER)->data;
        eat(lexer, Lexer::COLON);
        AST *type = parse_type(lexer, NULL);
        return Ast::new_argument(name, type);
    }

    AST *parse_type(Lexer::Lexer *lexer, AST* function)
    {
        char *name = eat(lexer, Lexer::KEYWORD)->data;
        if (peek(lexer) == Lexer::OPEN_BRACKET)
        {
            eat(lexer, Lexer::OPEN_BRACKET);
            AST *array_size = parse_term(lexer, function);
            eat(lexer, Lexer::CLOSE_BRACKET);
            return Ast::new_type(name, true, array_size);
        }
        else
        {
            return Ast::new_type(name, false, NULL);
        }
    }

    bool is_binary_op(Lexer::Tag tag)
    {
        return tag == Lexer::PLUS || tag == Lexer::MINUS || tag == Lexer::STAR || tag == Lexer::SLASH;
    }

    AST *parse_term(Lexer::Lexer *lexer, AST* function)
    {
        AST *left = parse_factor(lexer, function);
        while (is_binary_op(peek(lexer)))
        {
            Lexer::Tag tag = peek(lexer);
            Lexer::Token *token = eat(lexer, tag);
            if(left->tag == Ast::BINARY_OP)
            {
                if(left->data.TUPLE.op == token->data)
                {
                    AST* right = parse_factor(lexer, function);
                    left = Ast::new_tuple(Ast::BINARY_OP, left, right, token->data);
                    continue;
                }
                if(Ast::tag_get_priority(left->data.TUPLE.op) < Ast::tag_get_priority(token->data))
                {
                    AST* right = parse_factor(lexer, function);
                    AST* new_left = Ast::new_tuple(Ast::BINARY_OP, left->data.TUPLE.left, right, token->data);
                    left->data.TUPLE.left = new_left;
                    continue;
                }
            }
            AST *right = parse_factor(lexer, function);
            left = Ast::new_tuple(Ast::BINARY_OP, left, right, token->data);
        }
        return left;
    }
    bool is_arg(AST* function, char* name)
    {
        if(function == NULL) return false;
        if(function->tag != Ast::FUNCTION) return false;
        for(size_t i = 0; i < function->data.FUNCTION.array_size; i++)
        {
            if(strcmp(function->data.FUNCTION.arguments[i]->data.ARGUMENT.name, name) == 0)
            {
                return true;
            }
        }
        return false;
    }
    size_t get_arg_position(AST *function, char* name)
    {
        if(function == NULL) return false;
        if(function->tag != Ast::FUNCTION) return false;
        for(size_t i = 0; i < function->data.FUNCTION.array_size; i++)
        {
            if(strcmp(function->data.FUNCTION.arguments[i]->data.ARGUMENT.name, name) == 0)
            {
                return i;
            }
        }
        return 0;
    }
    AST *parse_factor(Lexer::Lexer *lexer, AST* function)
    {
        if (peek(lexer) == Lexer::NUMBER)
        {
            char* number = eat(lexer, Lexer::NUMBER)->data;
            return Ast::new_number(number);
        }
        if (peek(lexer) == Lexer::OPEN_PAREN)
        {
            eat(lexer, Lexer::OPEN_PAREN);
            AST *expr = parse_term(lexer, function);
            eat(lexer, Lexer::CLOSE_PAREN);
            return expr;
        }
        if (peek(lexer) == Lexer::IDENTIFIER)
        {
            char *name = eat(lexer, Lexer::IDENTIFIER)->data;
            if (peek(lexer) == Lexer::OPEN_PAREN)
            {
                return parse_call(lexer, name, function);
            }
            bool b_is_arg = is_arg(function, name);
            int position = 0;
            if(b_is_arg) position = get_arg_position(function, name);
            return Ast::new_variable(name, b_is_arg, position);
        }
        printf("Parser(parse_factor): Error: Unexpected token %s\n", Lexer::Tag_to_string(peek(lexer)).c_str());
        exit(1);
    }
    AST *parse_call(Lexer::Lexer *lexer, char *name, AST* function)
    {
        eat(lexer, Lexer::OPEN_PAREN);
        AST** arguments = (AST**) calloc(sizeof(AST*), 0);
        size_t array_size = 0;
        while (peek(lexer) != Lexer::CLOSE_PAREN)
        {
            AST* argument = parse_term(lexer, function);
            arguments = (AST**) realloc(arguments, sizeof(AST*) * (array_size + 1));
            arguments[array_size] = argument;
            array_size++;
            if (peek(lexer) == Lexer::COMMA)
            {
                eat(lexer, Lexer::COMMA);
            }
        }
        eat(lexer, Lexer::CLOSE_PAREN);
        // eat(lexer, Lexer::SEMICOLON);
        return Ast::new_call(name, arguments, array_size);
    }
}
