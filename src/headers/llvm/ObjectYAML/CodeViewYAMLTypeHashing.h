//==- CodeViewYAMLTypeHashing.h - CodeView YAMLIO Type hashing ----*- C++-*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines classes for handling the YAML representation of CodeView
// Debug Info.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJECTYAML_CODEVIEWYAMLTYPEHASHING_H
#define LLVM_OBJECTYAML_CODEVIEWYAMLTYPEHASHING_H

#include "ADT/ArrayRef.h"
#include "DebugInfo/CodeView/TypeHashing.h"
#include "ObjectYAML/YAML.h"
#include "Support/Allocator.h"
#include "Support/Error.h"
#include "Support/YAMLTraits.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace llvm {

namespace CodeViewYAML {

struct GlobalHash {
  GlobalHash() = default;
  explicit GlobalHash(StringRef S) : Hash(S) {
    assert(S.size() == 8 && "Invalid hash size!");
  }
  explicit GlobalHash(ArrayRef<uint8_t> S) : Hash(S) {
    assert(S.size() == 8 && "Invalid hash size!");
  }
  yaml::BinaryRef Hash;
};

struct DebugHSection {
  uint32_t Magic;
  uint16_t Version;
  uint16_t HashAlgorithm;
  std::vector<GlobalHash> Hashes;
};

DebugHSection fromDebugH(ArrayRef<uint8_t> DebugH);
ArrayRef<uint8_t> toDebugH(const DebugHSection &DebugH,
                           BumpPtrAllocator &Alloc);

} // end namespace CodeViewYAML

} // end namespace llvm

LLVM_YAML_DECLARE_MAPPING_TRAITS(CodeViewYAML::DebugHSection)
LLVM_YAML_DECLARE_SCALAR_TRAITS(CodeViewYAML::GlobalHash, QuotingType::None)
LLVM_YAML_IS_SEQUENCE_VECTOR(CodeViewYAML::GlobalHash)

#endif // LLVM_OBJECTYAML_CODEVIEWYAMLTYPES_H
