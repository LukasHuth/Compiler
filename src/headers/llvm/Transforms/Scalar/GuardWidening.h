//===- GuardWidening.h - ----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Guard widening is an optimization over the @llvm.experimental.guard intrinsic
// that (optimistically) combines multiple guards into one to have fewer checks
// at runtime.
//
//===----------------------------------------------------------------------===//


#ifndef LLVM_TRANSFORMS_SCALAR_GUARD_WIDENING_H
#define LLVM_TRANSFORMS_SCALAR_GUARD_WIDENING_H

#include "Analysis/LoopInfo.h"
#include "IR/PassManager.h"
#include "Transforms/Scalar/LoopPassManager.h"

namespace llvm {

class Function;

struct GuardWideningPass : public PassInfoMixin<GuardWideningPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};
}


#endif  // LLVM_TRANSFORMS_SCALAR_GUARD_WIDENING_H
