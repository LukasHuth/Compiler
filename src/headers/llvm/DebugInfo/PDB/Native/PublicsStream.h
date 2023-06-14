//===- PublicsStream.h - PDB Public Symbol Stream -------- ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_PDB_RAW_PUBLICSSTREAM_H
#define LLVM_DEBUGINFO_PDB_RAW_PUBLICSSTREAM_H

#include "DebugInfo/CodeView/SymbolRecord.h"
#include "DebugInfo/MSF/MappedBlockStream.h"
#include "DebugInfo/PDB/Native/GlobalsStream.h"
#include "DebugInfo/PDB/Native/RawConstants.h"
#include "DebugInfo/PDB/Native/RawTypes.h"
#include "DebugInfo/PDB/PDBTypes.h"
#include "Support/BinaryStreamArray.h"
#include "Support/Error.h"

namespace llvm {
namespace pdb {
class DbiStream;
struct GSIHashHeader;
class PDBFile;

class PublicsStream {
public:
  PublicsStream(std::unique_ptr<msf::MappedBlockStream> Stream);
  ~PublicsStream();
  Error reload();

  uint32_t getSymHash() const;
  uint16_t getThunkTableSection() const;
  uint32_t getThunkTableOffset() const;
  const GSIHashTable &getPublicsTable() const { return PublicsTable; }
  FixedStreamArray<support::ulittle32_t> getAddressMap() const {
    return AddressMap;
  }
  FixedStreamArray<support::ulittle32_t> getThunkMap() const {
    return ThunkMap;
  }
  FixedStreamArray<SectionOffset> getSectionOffsets() const {
    return SectionOffsets;
  }

private:
  std::unique_ptr<msf::MappedBlockStream> Stream;
  GSIHashTable PublicsTable;
  FixedStreamArray<support::ulittle32_t> AddressMap;
  FixedStreamArray<support::ulittle32_t> ThunkMap;
  FixedStreamArray<SectionOffset> SectionOffsets;

  const PublicsStreamHeader *Header;
};
}
}

#endif
