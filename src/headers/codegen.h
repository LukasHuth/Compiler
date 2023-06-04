#ifndef H_CODEGEN
#define H_CODEGEN
#include "AST.h"
#include <stdio.h>
void reallocate_string(char **new, char* format, char *orig);

typedef struct
{
    FILE *file;
    AST *ast;
} CODEGEN;

CODEGEN *init_codegen(FILE *file, AST *ast);
void codegen_free(CODEGEN *codegen);
void codegen_generate(CODEGEN *codegen);

#endif