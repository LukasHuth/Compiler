//===- Profile.h - XRay Profile Abstraction -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Defines the XRay Profile class representing the latency profile generated by
// XRay's profiling mode.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_XRAY_PROFILE_H
#define LLVM_XRAY_PROFILE_H

#include "ADT/DenseMap.h"
#include "ADT/SmallVector.h"
#include "ADT/StringRef.h"
#include "Support/Error.h"
#include <list>
#include <utility>
#include <vector>

namespace llvm {
namespace xray {

class Profile;

// We forward declare the Trace type for turning a Trace into a Profile.
class Trace;

/// This function will attempt to load an XRay Profiling Mode profile from the
/// provided |Filename|.
///
/// For any errors encountered in the loading of the profile data from
/// |Filename|, this function will return an Error condition appropriately.
Expected<Profile> loadProfile(StringRef Filename);

/// This algorithm will merge two Profile instances into a single Profile
/// instance, aggregating blocks by Thread ID.
Profile mergeProfilesByThread(const Profile &L, const Profile &R);

/// This algorithm will merge two Profile instances into a single Profile
/// instance, aggregating blocks by function call stack.
Profile mergeProfilesByStack(const Profile &L, const Profile &R);

/// This function takes a Trace and creates a Profile instance from it.
Expected<Profile> profileFromTrace(const Trace &T);

/// Profile instances are thread-compatible.
class Profile {
public:
  using ThreadID = uint64_t;
  using PathID = unsigned;
  using FuncID = int32_t;

  struct Data {
    uint64_t CallCount;
    uint64_t CumulativeLocalTime;
  };

  struct Block {
    ThreadID Thread;
    std::vector<std::pair<PathID, Data>> PathData;
  };

  /// Provides a sequence of function IDs from a previously interned PathID.
  ///
  /// Returns an error if |P| had not been interned before into the Profile.
  ///
  Expected<std::vector<FuncID>> expandPath(PathID P) const;

  /// The stack represented in |P| must be in stack order (leaf to root). This
  /// will always return the same PathID for |P| that has the same sequence.
  PathID internPath(ArrayRef<FuncID> P);

  /// Appends a fully-formed Block instance into the Profile.
  ///
  /// Returns an error condition in the following cases:
  ///
  ///    - The PathData component of the Block is empty
  ///
  Error addBlock(Block &&B);

  Profile() = default;
  ~Profile() = default;

  Profile(Profile &&O) noexcept
      : Blocks(std::move(O.Blocks)), NodeStorage(std::move(O.NodeStorage)),
        Roots(std::move(O.Roots)), PathIDMap(std::move(O.PathIDMap)),
        NextID(O.NextID) {}

  Profile &operator=(Profile &&O) noexcept {
    Blocks = std::move(O.Blocks);
    NodeStorage = std::move(O.NodeStorage);
    Roots = std::move(O.Roots);
    PathIDMap = std::move(O.PathIDMap);
    NextID = O.NextID;
    return *this;
  }

  Profile(const Profile &);
  Profile &operator=(const Profile &);

  friend void swap(Profile &L, Profile &R) {
    using std::swap;
    swap(L.Blocks, R.Blocks);
    swap(L.NodeStorage, R.NodeStorage);
    swap(L.Roots, R.Roots);
    swap(L.PathIDMap, R.PathIDMap);
    swap(L.NextID, R.NextID);
  }

private:
  using BlockList = std::list<Block>;

  struct TrieNode {
    FuncID Func = 0;
    std::vector<TrieNode *> Callees{};
    TrieNode *Caller = nullptr;
    PathID ID = 0;
  };

  // List of blocks associated with a Profile.
  BlockList Blocks;

  // List of TrieNode elements we've seen.
  std::list<TrieNode> NodeStorage;

  // List of call stack roots.
  SmallVector<TrieNode *, 4> Roots;

  // Reverse mapping between a PathID to a TrieNode*.
  DenseMap<PathID, TrieNode *> PathIDMap;

  // Used to identify paths.
  PathID NextID = 1;

public:
  using const_iterator = BlockList::const_iterator;
  const_iterator begin() const { return Blocks.begin(); }
  const_iterator end() const { return Blocks.end(); }
  bool empty() const { return Blocks.empty(); }
};

} // namespace xray
} // namespace llvm

#endif
