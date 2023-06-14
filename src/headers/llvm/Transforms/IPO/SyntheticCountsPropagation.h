#ifndef LLVM_TRANSFORMS_IPO_SYNTHETIC_COUNTS_PROPAGATION_H
#define LLVM_TRANSFORMS_IPO_SYNTHETIC_COUNTS_PROPAGATION_H

#include "ADT/STLExtras.h"
#include "IR/CallSite.h"
#include "IR/PassManager.h"
#include "Support/ScaledNumber.h"

namespace llvm {
class Function;
class Module;

class SyntheticCountsPropagation
    : public PassInfoMixin<SyntheticCountsPropagation> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};
} // namespace llvm
#endif
