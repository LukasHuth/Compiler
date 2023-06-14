//===- StringTableBuilder.h - String table building utility -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_STRINGTABLEBUILDER_H
#define LLVM_MC_STRINGTABLEBUILDER_H

#include "../ADT/CachedHashString.h"
#include "../ADT/DenseMap.h"
#include "../ADT/StringRef.h"
#include <cstddef>
#include <cstdint>

namespace llvm {

class raw_ostream;

/// Utility for building string tables with deduplicated suffixes.
class StringTableBuilder {
public:
  enum Kind { ELF, WinCOFF, MachO, RAW, DWARF, XCOFF };

private:
  DenseMap<CachedHashStringRef, size_t> StringIndexMap;
  size_t Size = 0;
  Kind K;
  unsigned Alignment;
  bool Finalized = false;

  void finalizeStringTable(bool Optimize);
  void initSize();

public:
  StringTableBuilder(Kind K, unsigned Alignment = 1);
  ~StringTableBuilder();

  /// Add a string to the builder. Returns the position of S in the
  /// table. The position will be changed if finalize is used.
  /// Can only be used before the table is finalized.
  size_t add(CachedHashStringRef S);
  size_t add(StringRef S) { return add(CachedHashStringRef(S)); }

  /// Analyze the strings and build the final table. No more strings can
  /// be added after this point.
  void finalize();

  /// Finalize the string table without reording it. In this mode, offsets
  /// returned by add will still be valid.
  void finalizeInOrder();

  /// Get the offest of a string in the string table. Can only be used
  /// after the table is finalized.
  size_t getOffset(CachedHashStringRef S) const;
  size_t getOffset(StringRef S) const {
    return getOffset(CachedHashStringRef(S));
  }

  size_t getSize() const { return Size; }
  void clear();

  void write(raw_ostream &OS) const;
  void write(uint8_t *Buf) const;

private:
  bool isFinalized() const { return Finalized; }
};

} // end namespace llvm

#endif // LLVM_MC_STRINGTABLEBUILDER_H
