//===- RelocVisitor.h - Visitor for object file relocations -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides a wrapper around all the different types of relocations
// in different file formats, such that a client can handle them in a unified
// manner by only implementing a minimal number of functions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJECT_RELOCVISITOR_H
#define LLVM_OBJECT_RELOCVISITOR_H

#include "ADT/Triple.h"
#include "BinaryFormat/ELF.h"
#include "BinaryFormat/MachO.h"
#include "Object/COFF.h"
#include "Object/ELFObjectFile.h"
#include "Object/MachO.h"
#include "Object/ObjectFile.h"
#include "Object/Wasm.h"
#include "Support/Casting.h"
#include "Support/ErrorHandling.h"
#include <cstdint>
#include <system_error>

namespace llvm {
namespace object {

using RelocationResolver = uint64_t (*)(RelocationRef R, uint64_t S, uint64_t A);

std::pair<bool (*)(uint64_t), RelocationResolver>
getRelocationResolver(const ObjectFile &Obj);

} // end namespace object
} // end namespace llvm

#endif // LLVM_OBJECT_RELOCVISITOR_H
