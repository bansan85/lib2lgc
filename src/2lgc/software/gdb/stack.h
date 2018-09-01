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
 * @file stack.h
 * @brief Store all informations about a whole stack.
 */

#ifndef SOFTWARE_GDB_STACK_H_
#define SOFTWARE_GDB_STACK_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/iterator.h>
#include <2lgc/software/gdb/backtrace.h>
#include <cstddef>
#include <ext/alloc_traits.h>
#include <memory>
#include <string>
#include <vector>

namespace llgc::software::gdb
{
class SetStack;

class Stack
{
 public:
  class Iter : public llgc::pattern::Iterator<SetStack, Stack>
  {
   public:
    Iter(const SetStack& set_stack, size_t pos)
        : llgc::pattern::Iterator<SetStack, Stack>(set_stack, pos)
    {
    }
    ~Iter() override = default;

    const Stack& operator*() const override;
  };

  explicit Stack(std::string filename);

  bool InterpretLine(const std::string& line) CHK;
  const std::string& GetFilename() const CHK { return filename_; }
  size_t NumberOfBacktraces() const CHK { return backtraces_.size(); }
  const Backtrace& GetBacktraceFromTop(size_t i) const CHK
  {
    return *backtraces_[i];
  }
  const Backtrace& GetBacktraceFromBottom(size_t i) const CHK
  {
    return *backtraces_[backtraces_.size() - 1 - i];
  }
  Backtrace::Iter begin() const { return Backtrace::Iter(*this, 0); }
  Backtrace::Iter end() const
  {
    return Backtrace::Iter(*this, backtraces_.size());
  }

 private:
  std::string filename_;
  std::vector<std::unique_ptr<Backtrace>> backtraces_;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_STACK_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
