//===- FileHeaderReader.h - XRay Trace File Header Reading Function -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares functions that can load an XRay log header from various
// sources.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_XRAY_FILEHEADERREADER_H_
#define LLVM_LIB_XRAY_FILEHEADERREADER_H_

#include "Support/DataExtractor.h"
#include "Support/Error.h"
#include "XRay/XRayRecord.h"
#include <cstdint>

namespace llvm {
namespace xray {

/// Convenience function for loading the file header given a data extractor at a
/// specified offset.
Expected<XRayFileHeader> readBinaryFormatHeader(DataExtractor &HeaderExtractor,
                                                uint64_t &OffsetPtr);

} // namespace xray
} // namespace llvm

#endif // LLVM_LIB_XRAY_FILEHEADERREADER_H_
