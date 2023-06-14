//===--- yaml2obj.h - -------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// Common declarations for yaml2obj
//===----------------------------------------------------------------------===//
#ifndef LLVM_TOOLS_YAML2OBJ_YAML2OBJ_H
#define LLVM_TOOLS_YAML2OBJ_YAML2OBJ_H

#include "ADT/StringRef.h"
#include "Support/Error.h"
#include <memory>

namespace llvm {
class raw_ostream;
template <typename T> class SmallVectorImpl;
template <typename T> class Expected;

namespace object {
class ObjectFile;
}

namespace COFFYAML {
struct Object;
}

namespace ELFYAML {
struct Object;
}

namespace MinidumpYAML {
struct Object;
}

namespace WasmYAML {
struct Object;
}

namespace yaml {
class Input;
struct YamlObjectFile;

using ErrorHandler = llvm::function_ref<void(const Twine &Msg)>;

bool yaml2coff(COFFYAML::Object &Doc, raw_ostream &Out, ErrorHandler EH);
bool yaml2elf(ELFYAML::Object &Doc, raw_ostream &Out, ErrorHandler EH);
bool yaml2macho(YamlObjectFile &Doc, raw_ostream &Out, ErrorHandler EH);
bool yaml2minidump(MinidumpYAML::Object &Doc, raw_ostream &Out,
                   ErrorHandler EH);
bool yaml2wasm(WasmYAML::Object &Doc, raw_ostream &Out, ErrorHandler EH);

bool convertYAML(Input &YIn, raw_ostream &Out, ErrorHandler ErrHandler,
                 unsigned DocNum = 1);

/// Convenience function for tests.
std::unique_ptr<object::ObjectFile>
yaml2ObjectFile(SmallVectorImpl<char> &Storage, StringRef Yaml,
                ErrorHandler ErrHandler);

} // namespace yaml
} // namespace llvm

#endif
