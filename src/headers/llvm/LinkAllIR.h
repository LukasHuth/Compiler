//===----- LinkAllIR.h - Reference All VMCore Code --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This header file pulls in all the object modules of the VMCore library so
// that tools like llc, opt, and lli can ensure they are linked with all symbols
// from libVMCore.a It should only be used from a tool's main program.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LINKALLIR_H
#define LLVM_LINKALLIR_H

#include "BinaryFormat/Dwarf.h"
#include "IR/InlineAsm.h"
#include "IR/Instructions.h"
#include "IR/LLVMContext.h"
#include "IR/Module.h"
#include "IR/Verifier.h"
#include "Support/DynamicLibrary.h"
#include "Support/MathExtras.h"
#include "Support/Memory.h"
#include "Support/Mutex.h"
#include "Support/Path.h"
#include "Support/Process.h"
#include "Support/Program.h"
#include "Support/Signals.h"
#include <cstdlib>

namespace {
  struct ForceVMCoreLinking {
    ForceVMCoreLinking() {
      // We must reference VMCore in such a way that compilers will not
      // delete it all as dead code, even with whole program optimization,
      // yet is effectively a NO-OP. As the compiler isn't smart enough
      // to know that getenv() never returns -1, this will do the job.
      if (std::getenv("bar") != (char*) -1)
        return;
      llvm::LLVMContext Context;
      (void)new llvm::Module("", Context);
      (void)new llvm::UnreachableInst(Context);
      (void)    llvm::createVerifierPass();
    }
  } ForceVMCoreLinking;
}

#endif
