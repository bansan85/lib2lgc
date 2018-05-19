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

namespace llgc::software::gdb {

class SetStack;

/**
 * @brief Store all informations about a stack.
 *
 * @details Contains all the backtrace of a dump.
 */
class Stack
{
 public:
  /**
   * @brief Iterator for Stack.
   */
  class Iter : public llgc::pattern::Iterator<SetStack, Stack>
  {
   public:
    /**
     * @brief The constructor
     *
     * @param[in] set_stack The set_stack contains the list of stack.
     * @param[in] pos The position of the current stack.
     */
    Iter(const SetStack& set_stack, size_t pos)
        : llgc::pattern::Iterator<SetStack, Stack>(set_stack, pos)
    {
    }

    /**
     * @brief Dereference an iterator return the current stack.
     *
     * @return Return the current backtrace.
     */
    const Stack& operator*() const override;
  };

  /**
   * @brief Default constructor.
   *
   * @param[in] filename Filename that contains all backtraces.
   */
  explicit Stack(std::string filename);

  /**
   * @brief Abstract factory that convert a line into a backtrace.
   *
   * @param[in] line The line that contains the description of the current
   * backtrace.
   *
   * @return true if convertion is successfull.
   */
  bool InterpretLine(const std::string& line) CHK;

  /**
   * @brief Get the name of the file from where the stack is stored.
   *
   * @return The filename.
   */
  const std::string& GetFilename() const CHK { return filename_; }

  /**
   * @brief Get the number of backtraces of the stack.
   *
   * @return The number of backtraces.
   */
  size_t NumberOfBacktraces() const CHK { return backtraces_.size(); }

  /**
   * @brief Get the backtrace from the top. Exception if out of the range.
   *
   * @param[in] i The nth backtrace. 0 is the top of the stack.
   *
   * @return A const pointer of the backtrace.
   */
  const Backtrace& GetBacktraceFromTop(size_t i) const CHK
  {
    return *backtraces_[i];
  }

  /**
   * @brief Get the backtrace from the bottom. Exception if out of the range.
   *
   * @param[in] i The nth backtrace. 0 is the bottom of the stack.
   *
   * @return A const pointer of the backtrace.
   */
  const Backtrace& GetBacktraceFromBottom(size_t i) const CHK
  {
    return *backtraces_[backtraces_.size() - 1 - i];
  }

  /**
   * @brief Return of the top backtrace.
   *
   * @return Begin of the const iterator.
   */
  Backtrace::Iter begin() const  // NS
  {
    return Backtrace::Iter(*this, 0);
  }

  /**
   * @brief Return after of the last backtrace.
   *
   * @return End of the const iterator.
   */
  Backtrace::Iter end() const  // NS
  {
    return Backtrace::Iter(*this, backtraces_.size());
  }

 private:
  /**
   * @brief Storage of the filename.
   */
  std::string filename_;

  /**
   * @brief Vector that store all backtraces.
   */
  std::vector<std::unique_ptr<Backtrace>> backtraces_;
};

}

#endif  // SOFTWARE_GDB_STACK_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
