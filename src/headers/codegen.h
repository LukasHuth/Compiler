#ifndef H_CODEGEN
#define H_CODEGEN
#include "AST.h"
#include <stdio.h>
#include "llvm-c/Core.h"
#include "llvm-c/Analysis.h"
#include "llvm-c/Target.h"
#include "llvm-c/ExecutionEngine.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/IRReader.h"
void reallocate_string(char **new, char* format, char *orig);

typedef struct
{
    FILE *file;
    AST *ast;
} CODEGEN;

void init_codegen(AST *ast);
void codegen_free(CODEGEN *codegen);
// void codegen_generate(CODEGEN *codegen);

#endif