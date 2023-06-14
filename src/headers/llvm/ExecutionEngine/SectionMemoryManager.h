//===- SectionMemoryManager.h - Memory manager for MCJIT/RtDyld -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of a section-based memory manager used by
// the MCJIT execution engine and RuntimeDyld.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_EXECUTIONENGINE_SECTIONMEMORYMANAGER_H
#define LLVM_EXECUTIONENGINE_SECTIONMEMORYMANAGER_H

#include "ADT/SmallVector.h"
#include "ADT/StringRef.h"
#include "ExecutionEngine/RTDyldMemoryManager.h"
#include "Support/Memory.h"
#include <cstdint>
#include <string>
#include <system_error>

namespace llvm {

/// This is a simple memory manager which implements the methods called by
/// the RuntimeDyld class to allocate memory for section-based loading of
/// objects, usually those generated by the MCJIT execution engine.
///
/// This memory manager allocates all section memory as read-write.  The
/// RuntimeDyld will copy JITed section memory into these allocated blocks
/// and perform any necessary linking and relocations.
///
/// Any client using this memory manager MUST ensure that section-specific
/// page permissions have been applied before attempting to execute functions
/// in the JITed object.  Permissions can be applied either by calling
/// MCJIT::finalizeObject or by calling SectionMemoryManager::finalizeMemory
/// directly.  Clients of MCJIT should call MCJIT::finalizeObject.
class SectionMemoryManager : public RTDyldMemoryManager {
public:
  /// This enum describes the various reasons to allocate pages from
  /// allocateMappedMemory.
  enum class AllocationPurpose {
    Code,
    ROData,
    RWData,
  };

  /// Implementations of this interface are used by SectionMemoryManager to
  /// request pages from the operating system.
  class MemoryMapper {
  public:
    /// This method attempts to allocate \p NumBytes bytes of virtual memory for
    /// \p Purpose.  \p NearBlock may point to an existing allocation, in which
    /// case an attempt is made to allocate more memory near the existing block.
    /// The actual allocated address is not guaranteed to be near the requested
    /// address.  \p Flags is used to set the initial protection flags for the
    /// block of the memory.  \p EC [out] returns an object describing any error
    /// that occurs.
    ///
    /// This method may allocate more than the number of bytes requested.  The
    /// actual number of bytes allocated is indicated in the returned
    /// MemoryBlock.
    ///
    /// The start of the allocated block must be aligned with the system
    /// allocation granularity (64K on Windows, page size on Linux).  If the
    /// address following \p NearBlock is not so aligned, it will be rounded up
    /// to the next allocation granularity boundary.
    ///
    /// \r a non-null MemoryBlock if the function was successful, otherwise a
    /// null MemoryBlock with \p EC describing the error.
    virtual sys::MemoryBlock
    allocateMappedMemory(AllocationPurpose Purpose, size_t NumBytes,
                         const sys::MemoryBlock *const NearBlock,
                         unsigned Flags, std::error_code &EC) = 0;

    /// This method sets the protection flags for a block of memory to the state
    /// specified by \p Flags.  The behavior is not specified if the memory was
    /// not allocated using the allocateMappedMemory method.
    /// \p Block describes the memory block to be protected.
    /// \p Flags specifies the new protection state to be assigned to the block.
    ///
    /// If \p Flags is MF_WRITE, the actual behavior varies with the operating
    /// system (i.e. MF_READ | MF_WRITE on Windows) and the target architecture
    /// (i.e. MF_WRITE -> MF_READ | MF_WRITE on i386).
    ///
    /// \r error_success if the function was successful, or an error_code
    /// describing the failure if an error occurred.
    virtual std::error_code protectMappedMemory(const sys::MemoryBlock &Block,
                                                unsigned Flags) = 0;

    /// This method releases a block of memory that was allocated with the
    /// allocateMappedMemory method. It should not be used to release any memory
    /// block allocated any other way.
    /// \p Block describes the memory to be released.
    ///
    /// \r error_success if the function was successful, or an error_code
    /// describing the failure if an error occurred.
    virtual std::error_code releaseMappedMemory(sys::MemoryBlock &M) = 0;

    virtual ~MemoryMapper();
  };

  /// Creates a SectionMemoryManager instance with \p MM as the associated
  /// memory mapper.  If \p MM is nullptr then a default memory mapper is used
  /// that directly calls into the operating system.
  SectionMemoryManager(MemoryMapper *MM = nullptr);
  SectionMemoryManager(const SectionMemoryManager &) = delete;
  void operator=(const SectionMemoryManager &) = delete;
  ~SectionMemoryManager() override;

  /// Allocates a memory block of (at least) the given size suitable for
  /// executable code.
  ///
  /// The value of \p Alignment must be a power of two.  If \p Alignment is zero
  /// a default alignment of 16 will be used.
  uint8_t *allocateCodeSection(uintptr_t Size, unsigned Alignment,
                               unsigned SectionID,
                               StringRef SectionName) override;

  /// Allocates a memory block of (at least) the given size suitable for
  /// executable code.
  ///
  /// The value of \p Alignment must be a power of two.  If \p Alignment is zero
  /// a default alignment of 16 will be used.
  uint8_t *allocateDataSection(uintptr_t Size, unsigned Alignment,
                               unsigned SectionID, StringRef SectionName,
                               bool isReadOnly) override;

  /// Update section-specific memory permissions and other attributes.
  ///
  /// This method is called when object loading is complete and section page
  /// permissions can be applied.  It is up to the memory manager implementation
  /// to decide whether or not to act on this method.  The memory manager will
  /// typically allocate all sections as read-write and then apply specific
  /// permissions when this method is called.  Code sections cannot be executed
  /// until this function has been called.  In addition, any cache coherency
  /// operations needed to reliably use the memory are also performed.
  ///
  /// \returns true if an error occurred, false otherwise.
  bool finalizeMemory(std::string *ErrMsg = nullptr) override;

  /// Invalidate instruction cache for code sections.
  ///
  /// Some platforms with separate data cache and instruction cache require
  /// explicit cache flush, otherwise JIT code manipulations (like resolved
  /// relocations) will get to the data cache but not to the instruction cache.
  ///
  /// This method is called from finalizeMemory.
  virtual void invalidateInstructionCache();

private:
  struct FreeMemBlock {
    // The actual block of free memory
    sys::MemoryBlock Free;
    // If there is a pending allocation from the same reservation right before
    // this block, store it's index in PendingMem, to be able to update the
    // pending region if part of this block is allocated, rather than having to
    // create a new one
    unsigned PendingPrefixIndex;
  };

  struct MemoryGroup {
    // PendingMem contains all blocks of memory (subblocks of AllocatedMem)
    // which have not yet had their permissions applied, but have been given
    // out to the user. FreeMem contains all block of memory, which have
    // neither had their permissions applied, nor been given out to the user.
    SmallVector<sys::MemoryBlock, 16> PendingMem;
    SmallVector<FreeMemBlock, 16> FreeMem;

    // All memory blocks that have been requested from the system
    SmallVector<sys::MemoryBlock, 16> AllocatedMem;

    sys::MemoryBlock Near;
  };

  uint8_t *allocateSection(AllocationPurpose Purpose, uintptr_t Size,
                           unsigned Alignment);

  std::error_code applyMemoryGroupPermissions(MemoryGroup &MemGroup,
                                              unsigned Permissions);

  void anchor() override;

  MemoryGroup CodeMem;
  MemoryGroup RWDataMem;
  MemoryGroup RODataMem;
  MemoryMapper &MMapper;
};

} // end namespace llvm

#endif // LLVM_EXECUTION_ENGINE_SECTION_MEMORY_MANAGER_H
