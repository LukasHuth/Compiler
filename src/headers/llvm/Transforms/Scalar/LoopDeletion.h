//===- LoopDeletion.h - Loop Deletion ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides the interface for the Loop Deletion Pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_LOOPDELETION_H
#define LLVM_TRANSFORMS_SCALAR_LOOPDELETION_H

#include "Analysis/LoopAnalysisManager.h"
#include "Analysis/LoopInfo.h"
#include "Analysis/ScalarEvolution.h"
#include "IR/PassManager.h"
#include "Transforms/Scalar/LoopPassManager.h"

namespace llvm {

class LoopDeletionPass : public PassInfoMixin<LoopDeletionPass> {
public:
  LoopDeletionPass() = default;

  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPDELETION_H
