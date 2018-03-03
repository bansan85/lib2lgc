/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file stack.h
 * @brief Store all informations about a whole stack.
 */

#ifndef STACK_H_
#define STACK_H_

// C++ system
#include <memory>
#include <string_view>
#include <vector>

// Compatibility
#include <compat.h>

#include "backtrace.h"

/**
 * @brief Store all informations about a stack.
 *
 * @details Contains all the backtrace of a dump.
 */
class Stack
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] filename Filename that contains all backtraces.
   */
  explicit Stack(const std::string_view& filename);

  /**
   * @brief Abstract factory that convert a line into a backtrace.
   *
   * @param line[i] The line that contains the description of the current
   * backtrace.
   *
   * @return true if convertion is successfull.
   */
  bool InterpretLine(const std::string_view& line) CHK;

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
   * @param i[in] The nth backtrace. 0 is the top of the stack.
   *
   * @return A const pointer of the backtrace.
   */
  const Bt* GetBacktraceFromTop(size_t i) const CHK
  {
    return backtraces_[i].get();
  }

  /**
   * @brief Get the backtrace from the bottom. Exception if out of the range.
   *
   * @param i[in] The nth backtrace. 0 is the bottom of the stack.
   *
   * @return A const pointer of the backtrace.
   */
  const Bt* GetBacktraceFromBottom(size_t i) const CHK
  {
    return backtraces_[backtraces_.size() - 1 - i].get();
  }

 private:
  /**
   * @brief Storage of the filename.
   */
  std::string filename_;
  /**
   * @brief Vector that store all backtraces.
   */
  std::vector<std::unique_ptr<Bt>> backtraces_;
};

#endif  // STACK_H_
