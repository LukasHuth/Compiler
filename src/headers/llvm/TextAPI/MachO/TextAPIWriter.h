//===--- TextAPIWriter.h - Text API Writer ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TEXTAPI_MACHO_WRITER_H
#define LLVM_TEXTAPI_MACHO_WRITER_H

#include "Support/MemoryBuffer.h"

namespace llvm {
namespace MachO {

class InterfaceFile;

class TextAPIWriter {
public:
  TextAPIWriter() = delete;

  static Error writeToStream(raw_ostream &os, const InterfaceFile &);
};

} // end namespace MachO.
} // end namespace llvm.

#endif // LLVM_TEXTAPI_MACHO_WRITER_H
