#ifndef H_CODEGEN
#define H_CODEGEN
#include "AST.h"
#include <stdio.h>
#include <iostream>
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/InitializePasses.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/AssemblyAnnotationWriter.h"
// #include "llvm-c/Core.h"
// #include "llvm-c/Analysis.h"
// #include "llvm-c/Target.h"
// #include "llvm-c/ExecutionEngine.h"
// #include "llvm-c/BitWriter.h"
// #include "llvm-c/IRReader.h"

namespace Codegen
{
  // typedef struct
  // {
  //     FILE *file;
  //     AST *ast;
  // } Codegen;
  class Codegen
  {
  public:
    AST *ast;
    Codegen(AST *ast) { this->ast = ast; }
    ~Codegen() { delete this->ast; }
  };

  void init(AST *ast);
}

#endif