#ifndef H_LEXER_UTIL
#define H_LEXER_UTIL
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
bool is_keyword(char* str);
bool is_eof(FILE* file);
char peek_char(FILE* file);
char get_char(FILE* file);
void lexer_add_lexer_token(LEXER *lexer, LEXER_TOKEN* lexer_token);
LEXER_TOKEN* lexer_new_token(LEXER_TAG tag, char* data);
void lexer_print(LEXER *lexer);
#endif