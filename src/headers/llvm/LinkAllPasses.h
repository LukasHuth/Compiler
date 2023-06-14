//===- llvm/LinkAllPasses.h ------------ Reference All Passes ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This header file pulls in all transformation and analysis passes for tools
// like opt and bugpoint that need this functionality.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LINKALLPASSES_H
#define LLVM_LINKALLPASSES_H

#include "ADT/Statistic.h"
#include "Analysis/AliasAnalysisEvaluator.h"
#include "Analysis/AliasSetTracker.h"
#include "Analysis/BasicAliasAnalysis.h"
#include "Analysis/CFLAndersAliasAnalysis.h"
#include "Analysis/CFLSteensAliasAnalysis.h"
#include "Analysis/CallPrinter.h"
#include "Analysis/DomPrinter.h"
#include "Analysis/GlobalsModRef.h"
#include "Analysis/IntervalPartition.h"
#include "Analysis/Lint.h"
#include "Analysis/Passes.h"
#include "Analysis/PostDominators.h"
#include "Analysis/RegionPass.h"
#include "Analysis/RegionPrinter.h"
#include "Analysis/ScalarEvolution.h"
#include "Analysis/ScalarEvolutionAliasAnalysis.h"
#include "Analysis/ScopedNoAliasAA.h"
#include "Analysis/TargetLibraryInfo.h"
#include "Analysis/TypeBasedAliasAnalysis.h"
#include "CodeGen/Passes.h"
#include "IR/Function.h"
#include "IR/IRPrintingPasses.h"
#include "Support/Valgrind.h"
#include "Transforms/AggressiveInstCombine/AggressiveInstCombine.h"
#include "Transforms/IPO.h"
#include "Transforms/IPO/AlwaysInliner.h"
#include "Transforms/IPO/Attributor.h"
#include "Transforms/IPO/FunctionAttrs.h"
#include "Transforms/InstCombine/InstCombine.h"
#include "Transforms/Instrumentation.h"
#include "Transforms/Instrumentation/BoundsChecking.h"
#include "Transforms/ObjCARC.h"
#include "Transforms/Scalar.h"
#include "Transforms/Scalar/GVN.h"
#include "Transforms/Scalar/InstSimplifyPass.h"
#include "Transforms/Scalar/Scalarizer.h"
#include "Transforms/Utils.h"
#include "Transforms/Utils/SymbolRewriter.h"
#include "Transforms/Utils/UnifyFunctionExitNodes.h"
#include "Transforms/Vectorize.h"
#include <cstdlib>

namespace {
  struct ForcePassLinking {
    ForcePassLinking() {
      // We must reference the passes in such a way that compilers will not
      // delete it all as dead code, even with whole program optimization,
      // yet is effectively a NO-OP. As the compiler isn't smart enough
      // to know that getenv() never returns -1, this will do the job.
      if (std::getenv("bar") != (char*) -1)
        return;

      (void) llvm::createAAEvalPass();
      (void) llvm::createAggressiveDCEPass();
      (void) llvm::createAggressiveInstCombinerPass();
      (void) llvm::createBitTrackingDCEPass();
      (void) llvm::createArgumentPromotionPass();
      (void) llvm::createAlignmentFromAssumptionsPass();
      (void) llvm::createBasicAAWrapperPass();
      (void) llvm::createSCEVAAWrapperPass();
      (void) llvm::createTypeBasedAAWrapperPass();
      (void) llvm::createScopedNoAliasAAWrapperPass();
      (void) llvm::createBoundsCheckingLegacyPass();
      (void) llvm::createBreakCriticalEdgesPass();
      (void) llvm::createCallGraphDOTPrinterPass();
      (void) llvm::createCallGraphViewerPass();
      (void) llvm::createCFGSimplificationPass();
      (void) llvm::createCFLAndersAAWrapperPass();
      (void) llvm::createCFLSteensAAWrapperPass();
      (void) llvm::createStructurizeCFGPass();
      (void) llvm::createLibCallsShrinkWrapPass();
      (void) llvm::createCalledValuePropagationPass();
      (void) llvm::createConstantMergePass();
      (void) llvm::createConstantPropagationPass();
      (void) llvm::createControlHeightReductionLegacyPass();
      (void) llvm::createCostModelAnalysisPass();
      (void) llvm::createDeadArgEliminationPass();
      (void) llvm::createDeadCodeEliminationPass();
      (void) llvm::createDeadInstEliminationPass();
      (void) llvm::createDeadStoreEliminationPass();
      (void) llvm::createDependenceAnalysisWrapperPass();
      (void) llvm::createDomOnlyPrinterPass();
      (void) llvm::createDomPrinterPass();
      (void) llvm::createDomOnlyViewerPass();
      (void) llvm::createDomViewerPass();
      (void) llvm::createGCOVProfilerPass();
      (void) llvm::createPGOInstrumentationGenLegacyPass();
      (void) llvm::createPGOInstrumentationUseLegacyPass();
      (void) llvm::createPGOInstrumentationGenCreateVarLegacyPass();
      (void) llvm::createPGOIndirectCallPromotionLegacyPass();
      (void) llvm::createPGOMemOPSizeOptLegacyPass();
      (void) llvm::createInstrProfilingLegacyPass();
      (void) llvm::createFunctionImportPass();
      (void) llvm::createFunctionInliningPass();
      (void) llvm::createAlwaysInlinerLegacyPass();
      (void) llvm::createGlobalDCEPass();
      (void) llvm::createGlobalOptimizerPass();
      (void) llvm::createGlobalsAAWrapperPass();
      (void) llvm::createGuardWideningPass();
      (void) llvm::createLoopGuardWideningPass();
      (void) llvm::createIPConstantPropagationPass();
      (void) llvm::createIPSCCPPass();
      (void) llvm::createInductiveRangeCheckEliminationPass();
      (void) llvm::createIndVarSimplifyPass();
      (void) llvm::createInstSimplifyLegacyPass();
      (void) llvm::createInstructionCombiningPass();
      (void) llvm::createInternalizePass();
      (void) llvm::createLCSSAPass();
      (void) llvm::createLegacyDivergenceAnalysisPass();
      (void) llvm::createLICMPass();
      (void) llvm::createLoopSinkPass();
      (void) llvm::createLazyValueInfoPass();
      (void) llvm::createLoopExtractorPass();
      (void) llvm::createLoopInterchangePass();
      (void) llvm::createLoopPredicationPass();
      (void) llvm::createLoopSimplifyPass();
      (void) llvm::createLoopSimplifyCFGPass();
      (void) llvm::createLoopStrengthReducePass();
      (void) llvm::createLoopRerollPass();
      (void) llvm::createLoopUnrollPass();
      (void) llvm::createLoopUnrollAndJamPass();
      (void) llvm::createLoopUnswitchPass();
      (void) llvm::createLoopVersioningLICMPass();
      (void) llvm::createLoopIdiomPass();
      (void) llvm::createLoopRotatePass();
      (void) llvm::createLowerConstantIntrinsicsPass();
      (void) llvm::createLowerExpectIntrinsicPass();
      (void) llvm::createLowerInvokePass();
      (void) llvm::createLowerSwitchPass();
      (void) llvm::createNaryReassociatePass();
      (void) llvm::createObjCARCAAWrapperPass();
      (void) llvm::createObjCARCAPElimPass();
      (void) llvm::createObjCARCExpandPass();
      (void) llvm::createObjCARCContractPass();
      (void) llvm::createObjCARCOptPass();
      (void) llvm::createPAEvalPass();
      (void) llvm::createPromoteMemoryToRegisterPass();
      (void) llvm::createDemoteRegisterToMemoryPass();
      (void) llvm::createPruneEHPass();
      (void) llvm::createPostDomOnlyPrinterPass();
      (void) llvm::createPostDomPrinterPass();
      (void) llvm::createPostDomOnlyViewerPass();
      (void) llvm::createPostDomViewerPass();
      (void) llvm::createReassociatePass();
      (void) llvm::createRedundantDbgInstEliminationPass();
      (void) llvm::createRegionInfoPass();
      (void) llvm::createRegionOnlyPrinterPass();
      (void) llvm::createRegionOnlyViewerPass();
      (void) llvm::createRegionPrinterPass();
      (void) llvm::createRegionViewerPass();
      (void) llvm::createSCCPPass();
      (void) llvm::createSafeStackPass();
      (void) llvm::createSROAPass();
      (void) llvm::createSingleLoopExtractorPass();
      (void) llvm::createStripSymbolsPass();
      (void) llvm::createStripNonDebugSymbolsPass();
      (void) llvm::createStripDeadDebugInfoPass();
      (void) llvm::createStripDeadPrototypesPass();
      (void) llvm::createTailCallEliminationPass();
      (void) llvm::createJumpThreadingPass();
      (void) llvm::createUnifyFunctionExitNodesPass();
      (void) llvm::createInstCountPass();
      (void) llvm::createConstantHoistingPass();
      (void) llvm::createCodeGenPreparePass();
      (void) llvm::createEntryExitInstrumenterPass();
      (void) llvm::createPostInlineEntryExitInstrumenterPass();
      (void) llvm::createEarlyCSEPass();
      (void) llvm::createGVNHoistPass();
      (void) llvm::createMergedLoadStoreMotionPass();
      (void) llvm::createGVNPass();
      (void) llvm::createNewGVNPass();
      (void) llvm::createMemCpyOptPass();
      (void) llvm::createLoopDeletionPass();
      (void) llvm::createPostDomTree();
      (void) llvm::createInstructionNamerPass();
      (void) llvm::createMetaRenamerPass();
      (void) llvm::createAttributorLegacyPass();
      (void) llvm::createPostOrderFunctionAttrsLegacyPass();
      (void) llvm::createReversePostOrderFunctionAttrsPass();
      (void) llvm::createMergeFunctionsPass();
      (void) llvm::createMergeICmpsLegacyPass();
      (void) llvm::createExpandMemCmpPass();
      std::string buf;
      llvm::raw_string_ostream os(buf);
      (void) llvm::createPrintModulePass(os);
      (void) llvm::createPrintFunctionPass(os);
      (void) llvm::createModuleDebugInfoPrinterPass();
      (void) llvm::createPartialInliningPass();
      (void) llvm::createLintPass();
      (void) llvm::createSinkingPass();
      (void) llvm::createLowerAtomicPass();
      (void) llvm::createCorrelatedValuePropagationPass();
      (void) llvm::createMemDepPrinter();
      (void) llvm::createLoopVectorizePass();
      (void) llvm::createSLPVectorizerPass();
      (void) llvm::createLoadStoreVectorizerPass();
      (void) llvm::createPartiallyInlineLibCallsPass();
      (void) llvm::createScalarizerPass();
      (void) llvm::createSeparateConstOffsetFromGEPPass();
      (void) llvm::createSpeculativeExecutionPass();
      (void) llvm::createSpeculativeExecutionIfHasBranchDivergencePass();
      (void) llvm::createRewriteSymbolsPass();
      (void) llvm::createStraightLineStrengthReducePass();
      (void) llvm::createMemDerefPrinter();
      (void) llvm::createMustExecutePrinter();
      (void) llvm::createMustBeExecutedContextPrinter();
      (void) llvm::createFloat2IntPass();
      (void) llvm::createEliminateAvailableExternallyPass();
      (void) llvm::createScalarizeMaskedMemIntrinPass();
      (void) llvm::createWarnMissedTransformationsPass();
      (void) llvm::createHardwareLoopsPass();
      (void)llvm::createInjectTLIMappingsLegacyPass();

      (void)new llvm::IntervalPartition();
      (void)new llvm::ScalarEvolutionWrapperPass();
      llvm::Function::Create(nullptr, llvm::GlobalValue::ExternalLinkage)->viewCFGOnly();
      llvm::RGPassManager RGM;
      llvm::TargetLibraryInfoImpl TLII;
      llvm::TargetLibraryInfo TLI(TLII);
      llvm::AliasAnalysis AA(TLI);
      llvm::AliasSetTracker X(AA);
      X.add(nullptr, llvm::LocationSize::unknown(),
            llvm::AAMDNodes()); // for -print-alias-sets
      (void) llvm::AreStatisticsEnabled();
      (void) llvm::sys::RunningOnValgrind();
    }
  } ForcePassLinking; // Force link by creating a global definition.
}

#endif
