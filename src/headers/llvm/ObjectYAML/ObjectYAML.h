//===- ObjectYAML.h ---------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJECTYAML_OBJECTYAML_H
#define LLVM_OBJECTYAML_OBJECTYAML_H

#include "ObjectYAML/COFFYAML.h"
#include "ObjectYAML/ELFYAML.h"
#include "ObjectYAML/MachOYAML.h"
#include "ObjectYAML/MinidumpYAML.h"
#include "ObjectYAML/WasmYAML.h"
#include "Support/YAMLTraits.h"
#include <memory>

namespace llvm {
namespace yaml {

class IO;

struct YamlObjectFile {
  std::unique_ptr<ELFYAML::Object> Elf;
  std::unique_ptr<COFFYAML::Object> Coff;
  std::unique_ptr<MachOYAML::Object> MachO;
  std::unique_ptr<MachOYAML::UniversalBinary> FatMachO;
  std::unique_ptr<MinidumpYAML::Object> Minidump;
  std::unique_ptr<WasmYAML::Object> Wasm;
};

template <> struct MappingTraits<YamlObjectFile> {
  static void mapping(IO &IO, YamlObjectFile &ObjectFile);
};

} // end namespace yaml
} // end namespace llvm

#endif // LLVM_OBJECTYAML_OBJECTYAML_H
