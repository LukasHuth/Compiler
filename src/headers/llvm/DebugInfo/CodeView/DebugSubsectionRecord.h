//===- DebugSubsectionRecord.h ----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_DEBUGSUBSECTIONRECORD_H
#define LLVM_DEBUGINFO_CODEVIEW_DEBUGSUBSECTIONRECORD_H

#include "DebugInfo/CodeView/CodeView.h"
#include "Support/BinaryStreamArray.h"
#include "Support/BinaryStreamRef.h"
#include "Support/Endian.h"
#include "Support/Error.h"
#include "Support/MathExtras.h"
#include <cstdint>
#include <memory>

namespace llvm {

class BinaryStreamWriter;

namespace codeview {

class DebugSubsection;

// Corresponds to the `CV_DebugSSubsectionHeader_t` structure.
struct DebugSubsectionHeader {
  support::ulittle32_t Kind;   // codeview::DebugSubsectionKind enum
  support::ulittle32_t Length; // number of bytes occupied by this record.
};

class DebugSubsectionRecord {
public:
  DebugSubsectionRecord();
  DebugSubsectionRecord(DebugSubsectionKind Kind, BinaryStreamRef Data,
                        CodeViewContainer Container);

  static Error initialize(BinaryStreamRef Stream, DebugSubsectionRecord &Info,
                          CodeViewContainer Container);

  uint32_t getRecordLength() const;
  DebugSubsectionKind kind() const;
  BinaryStreamRef getRecordData() const;

private:
  CodeViewContainer Container = CodeViewContainer::ObjectFile;
  DebugSubsectionKind Kind = DebugSubsectionKind::None;
  BinaryStreamRef Data;
};

class DebugSubsectionRecordBuilder {
public:
  DebugSubsectionRecordBuilder(std::shared_ptr<DebugSubsection> Subsection,
                               CodeViewContainer Container);

  /// Use this to copy existing subsections directly from source to destination.
  /// For example, line table subsections in an object file only need to be
  /// relocated before being copied into the PDB.
  DebugSubsectionRecordBuilder(const DebugSubsectionRecord &Contents,
                               CodeViewContainer Container);

  uint32_t calculateSerializedLength();
  Error commit(BinaryStreamWriter &Writer) const;

private:
  /// The subsection to build. Will be null if Contents is non-empty.
  std::shared_ptr<DebugSubsection> Subsection;

  /// The bytes of the subsection. Only non-empty if Subsection is null.
  DebugSubsectionRecord Contents;

  CodeViewContainer Container;
};

} // end namespace codeview

template <> struct VarStreamArrayExtractor<codeview::DebugSubsectionRecord> {
  Error operator()(BinaryStreamRef Stream, uint32_t &Length,
                   codeview::DebugSubsectionRecord &Info) {
    // FIXME: We need to pass the container type through to this function.  In
    // practice this isn't super important since the subsection header describes
    // its length and we can just skip it.  It's more important when writing.
    if (auto EC = codeview::DebugSubsectionRecord::initialize(
            Stream, Info, codeview::CodeViewContainer::Pdb))
      return EC;
    Length = alignTo(Info.getRecordLength(), 4);
    return Error::success();
  }
};

namespace codeview {

using DebugSubsectionArray = VarStreamArray<DebugSubsectionRecord>;

} // end namespace codeview

} // end namespace llvm

#endif // LLVM_DEBUGINFO_CODEVIEW_DEBUGSUBSECTIONRECORD_H
