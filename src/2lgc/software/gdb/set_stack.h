/*
 *    Copyright 2018 LE GARREC Vincent
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file set_stack.h
 * @brief Store and sort all stacks. Criterea of sort must be defined on
 * creation.
 */

#ifndef SOFTWARE_GDB_SET_STACK_H_
#define SOFTWARE_GDB_SET_STACK_H_

#include <2lgc/compat.h>
#include <2lgc/software/gdb/gdb_server.h>
#include <2lgc/software/gdb/stack.h>
#include <sys/types.h>
#include <cstddef>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

namespace llgc::protobuf::software
{
class Gdb;
}

namespace llgc::software::gdb
{
class Backtrace;

class SetStack
{
 public:
  SetStack(bool with_source_only, size_t top_frame, size_t bottom_frame,
           bool print_one_by_group);
#ifndef SWIG
  SetStack(SetStack &&other) = delete;
  SetStack(SetStack const &other) = delete;
  SetStack &operator=(SetStack &&other) = delete;
  SetStack &operator=(SetStack const &other) = delete;
#endif  // !SWIG
  ~SetStack() = default;

  bool Add(const std::string &filename) CHK;
  bool AddRecursive(const std::string &folder, unsigned int nthread,
                    const std::string &regex) CHK;
  bool AddList(const std::string &list, unsigned int nthread) CHK;
  size_t Count() const;
  void Print() const;
  const Stack &Get(size_t i) const;

  Stack::Iter begin() const { return Stack::Iter(*this, 0); }
  Stack::Iter end() const { return Stack::Iter(*this, stack_.size()); }

  GdbServer server_;

 private:
  struct LocalCompare
  {
    using FunctionGetBacktrace = const Backtrace &(Stack::*)(size_t i) const;

    LocalCompare(bool with_source_only, size_t top_frame, size_t bottom_frame);
    ssize_t CompareFrom(size_t nb_max_frames,
                        const FunctionGetBacktrace &get_backtraces,
                        const std::unique_ptr<Stack> &i,
                        const std::unique_ptr<Stack> &j) const;
    bool operator()(const std::unique_ptr<Stack> &i,
                    const std::unique_ptr<Stack> &j) const;

    bool with_source_only_;
    size_t top_frame_;
    size_t bottom_frame_;
  };

  bool ParallelAdd(const std::vector<std::string> &all_files,
                   unsigned int nthread) CHK;
  bool TellError(const std::string &filename) CHK;
  bool Forward(const llgc::protobuf::software::Gdb &message) CHK;

  std::multiset<std::unique_ptr<Stack>, LocalCompare> stack_;
  mutable std::mutex mutex_stack_;
  bool print_one_by_group_;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_SET_STACK_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
