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
 * @file backtrace.h
 * @brief Store all informations about one backtrace.
 */

#ifndef BT_H_
#define BT_H_

// C++ system
#include <string_view>

// Compatibility
#include <compat.h>

#include "function.h"

/**
 * @brief Store all informations about one backtrace.
 */
class Bt {
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] line The line in format
   *   "#d  0xDEADBEEF in Function (args="") at filename:1234" or
   *   "#d  0xDEADBEEF in Function (args="") from library"
   */
  Bt(const std::string_view& line);

  /**
   * @brief From a line, decode and extract the index, the address, the
   * function and the filename.
   *
   * @param[in] line The line from gdb in format
   *   "#d  0xDEADBEEF in Function (args="") at filename:1234"
   * @param[out] index The index extracted (d).
   * @param[out] address The address extracted (0xDEADBEEF).
   * @param[out] function The function extracted (Function (args="")).
   * @param[out] file The file extracted (filename:1234).
   *
   * @return true if no error.
   */
  bool DecodeBacktrace(const std::string_view& line, std::string_view& index,
                       std::string_view& address, std::string_view& function,
                       std::string_view& file) CHK;

  bool HasSource() const CHK { return file_.length() != 0; }
  const std::string& GetFile() const CHK { return file_; }
  size_t GetLine() const CHK { return line_; }

 private:
  // The nth backtrace of the stack.
  size_t index_;
  // Address of the PC.
  uint64_t address_;
  Function function_;
  std::string file_;
  size_t line_;

  bool ReadIndex(const std::string& number) CHK;
  bool ReadAddress(const std::string& address) CHK;
  bool ReadFunction(const std::string_view& description) CHK;
  bool ReadSource(const std::string_view& file) CHK;
};

#endif  // BT_H_
