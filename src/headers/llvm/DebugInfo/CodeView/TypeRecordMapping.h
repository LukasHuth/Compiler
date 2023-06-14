//===- TypeRecordMapping.h --------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_TYPERECORDMAPPING_H
#define LLVM_DEBUGINFO_CODEVIEW_TYPERECORDMAPPING_H

#include "ADT/Optional.h"
#include "DebugInfo/CodeView/CVTypeVisitor.h"
#include "DebugInfo/CodeView/CodeViewRecordIO.h"
#include "DebugInfo/CodeView/TypeVisitorCallbacks.h"
#include "Support/Error.h"

namespace llvm {
class BinaryStreamReader;
class BinaryStreamWriter;

namespace codeview {
class TypeRecordMapping : public TypeVisitorCallbacks {
public:
  explicit TypeRecordMapping(BinaryStreamReader &Reader) : IO(Reader) {}
  explicit TypeRecordMapping(BinaryStreamWriter &Writer) : IO(Writer) {}
  explicit TypeRecordMapping(CodeViewRecordStreamer &Streamer) : IO(Streamer) {}

  using TypeVisitorCallbacks::visitTypeBegin;
  Error visitTypeBegin(CVType &Record) override;
  Error visitTypeBegin(CVType &Record, TypeIndex Index) override;
  Error visitTypeEnd(CVType &Record) override;

  Error visitMemberBegin(CVMemberRecord &Record) override;
  Error visitMemberEnd(CVMemberRecord &Record) override;

#define TYPE_RECORD(EnumName, EnumVal, Name)                                   \
  Error visitKnownRecord(CVType &CVR, Name##Record &Record) override;
#define MEMBER_RECORD(EnumName, EnumVal, Name)                                 \
  Error visitKnownMember(CVMemberRecord &CVR, Name##Record &Record) override;
#define TYPE_RECORD_ALIAS(EnumName, EnumVal, Name, AliasName)
#define MEMBER_RECORD_ALIAS(EnumName, EnumVal, Name, AliasName)
#include "DebugInfo/CodeView/CodeViewTypes.def"

private:
  Optional<TypeLeafKind> TypeKind;
  Optional<TypeLeafKind> MemberKind;

  CodeViewRecordIO IO;
};
}
}

#endif
