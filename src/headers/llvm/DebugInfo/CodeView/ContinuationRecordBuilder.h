//===- ContinuationRecordBuilder.h ------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_CONTINUATIONRECORDBUILDER_H
#define LLVM_DEBUGINFO_CODEVIEW_CONTINUATIONRECORDBUILDER_H

#include "ADT/ArrayRef.h"
#include "ADT/Optional.h"
#include "ADT/SmallVector.h"
#include "DebugInfo/CodeView/CodeView.h"
#include "DebugInfo/CodeView/RecordSerialization.h"
#include "DebugInfo/CodeView/TypeIndex.h"
#include "DebugInfo/CodeView/TypeRecord.h"
#include "DebugInfo/CodeView/TypeRecordMapping.h"
#include "DebugInfo/CodeView/TypeVisitorCallbacks.h"
#include "Support/Allocator.h"
#include "Support/BinaryByteStream.h"
#include "Support/BinaryStreamWriter.h"
#include "Support/Error.h"
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

namespace llvm {
namespace codeview {
enum class ContinuationRecordKind { FieldList, MethodOverloadList };

class ContinuationRecordBuilder {
  SmallVector<uint32_t, 4> SegmentOffsets;
  Optional<ContinuationRecordKind> Kind;
  AppendingBinaryByteStream Buffer;
  BinaryStreamWriter SegmentWriter;
  TypeRecordMapping Mapping;
  ArrayRef<uint8_t> InjectedSegmentBytes;

  uint32_t getCurrentSegmentLength() const;

  void insertSegmentEnd(uint32_t Offset);
  CVType createSegmentRecord(uint32_t OffBegin, uint32_t OffEnd,
                             Optional<TypeIndex> RefersTo);

public:
  ContinuationRecordBuilder();
  ~ContinuationRecordBuilder();

  void begin(ContinuationRecordKind RecordKind);

  // This template is explicitly instantiated in the implementation file for all
  // supported types.  The method itself is ugly, so inlining it into the header
  // file clutters an otherwise straightforward interface.
  template <typename RecordType> void writeMemberType(RecordType &Record);

  std::vector<CVType> end(TypeIndex Index);
};
} // namespace codeview
} // namespace llvm

#endif
