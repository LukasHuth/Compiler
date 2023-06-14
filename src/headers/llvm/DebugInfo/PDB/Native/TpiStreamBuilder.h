//===- TpiStreamBuilder.h - PDB Tpi Stream Creation -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_PDB_RAW_PDBTPISTREAMBUILDER_H
#define LLVM_DEBUGINFO_PDB_RAW_PDBTPISTREAMBUILDER_H

#include "ADT/Optional.h"
#include "DebugInfo/CodeView/TypeRecord.h"
#include "DebugInfo/PDB/Native/RawConstants.h"
#include "DebugInfo/PDB/Native/RawTypes.h"
#include "Support/Allocator.h"
#include "Support/BinaryByteStream.h"
#include "Support/BinaryItemStream.h"
#include "Support/BinaryStreamRef.h"
#include "Support/Error.h"

#include <vector>

namespace llvm {
class BinaryByteStream;
class WritableBinaryStreamRef;

template <> struct BinaryItemTraits<llvm::codeview::CVType> {
  static size_t length(const codeview::CVType &Item) { return Item.length(); }
  static ArrayRef<uint8_t> bytes(const codeview::CVType &Item) {
    return Item.data();
  }
};

namespace codeview {
class TypeRecord;
}
namespace msf {
class MSFBuilder;
struct MSFLayout;
}
namespace pdb {
class PDBFile;
class TpiStream;
struct TpiStreamHeader;

class TpiStreamBuilder {
public:
  explicit TpiStreamBuilder(msf::MSFBuilder &Msf, uint32_t StreamIdx);
  ~TpiStreamBuilder();

  TpiStreamBuilder(const TpiStreamBuilder &) = delete;
  TpiStreamBuilder &operator=(const TpiStreamBuilder &) = delete;

  void setVersionHeader(PdbRaw_TpiVer Version);
  void addTypeRecord(ArrayRef<uint8_t> Type, Optional<uint32_t> Hash);

  Error finalizeMsfLayout();

  uint32_t getRecordCount() const { return TypeRecords.size(); }

  Error commit(const msf::MSFLayout &Layout, WritableBinaryStreamRef Buffer);

  uint32_t calculateSerializedLength();

private:
  uint32_t calculateHashBufferSize() const;
  uint32_t calculateIndexOffsetSize() const;
  Error finalize();

  msf::MSFBuilder &Msf;
  BumpPtrAllocator &Allocator;

  size_t TypeRecordBytes = 0;

  PdbRaw_TpiVer VerHeader = PdbRaw_TpiVer::PdbTpiV80;
  std::vector<ArrayRef<uint8_t>> TypeRecords;
  std::vector<uint32_t> TypeHashes;
  std::vector<codeview::TypeIndexOffset> TypeIndexOffsets;
  uint32_t HashStreamIndex = kInvalidStreamIndex;
  std::unique_ptr<BinaryByteStream> HashValueStream;

  const TpiStreamHeader *Header;
  uint32_t Idx;
};
}
}

#endif
