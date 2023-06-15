#ifndef H_CODEGEN
#define H_CODEGEN
#include "AST.h"
#include <stdio.h>
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm-c/Core.h"
#include "llvm-c/Analysis.h"
#include "llvm-c/Target.h"
#include "llvm-c/ExecutionEngine.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/IRReader.h"

namespace Codegen
{
    typedef struct
    {
        FILE *file;
        AST *ast;
    } Codegen;

    void init(AST *ast);
    void Free(Codegen *codegen);
}

// void codegen_generate(CODEGEN *codegen);

#endif