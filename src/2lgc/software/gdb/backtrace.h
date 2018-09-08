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
 * @file backtrace.h
 * @brief Store all informations about one backtrace.
 */

#ifndef SOFTWARE_GDB_BACKTRACE_H_
#define SOFTWARE_GDB_BACKTRACE_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/iterator.h>
#include <2lgc/software/gdb/function.h>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>

namespace llgc::software::gdb
{
class Stack;

class Backtrace
{
 public:
  class Iter : public llgc::pattern::Iterator<Stack, Backtrace>
  {
   public:
    Iter(const Stack &stack, size_t pos)
        : llgc::pattern::Iterator<Stack, Backtrace>(stack, pos)
    {
    }
    ~Iter() override = default;

    const Backtrace &operator*() const override;
  };

  static std::unique_ptr<Backtrace> Factory(const std::string &line);
  static bool DecodeBacktrace(const std::string &line, std::string *index,
                              std::string *address, std::string *function,
                              std::string *file) CHK;
  bool HasSource() const CHK { return file_.length() != 0; }
  const std::string &GetFile() const CHK { return file_; }
  const std::string &GetName() const CHK { return function_.GetName(); }
  size_t GetLine() const CHK { return line_; }
  size_t GetIndex() const CHK { return index_; }
  uint64_t GetAddress() const CHK { return address_; }

 private:
  size_t index_ = 0;
  uint64_t address_ = 0;
  Function function_;
  std::string file_;
  size_t line_ = std::numeric_limits<size_t>::max();

  bool ReadIndex(const std::string &number) CHK;
  bool ReadAddress(const std::string &address) CHK;
  bool ReadFunction(const std::string &description) CHK;
  bool ReadSource(const std::string &file) CHK;

  static size_t FindNextArg(const std::string &args);
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_BACKTRACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
