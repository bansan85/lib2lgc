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

#ifndef BACKTRACE_H_
#define BACKTRACE_H_

// C++ system
#include <string_view>

// Compatibility
#include <compat.h>

#include "function.h"

/**
 * @brief Store all informations about one backtrace.
 */
class Bt
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] line The line in format
   *   "#d  0xDEADBEEF in Function (args="") at filename:1234" or
   *   "#d  0xDEADBEEF in Function (args="") from library"
   */
  explicit Bt(const std::string_view& line);

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
  bool DecodeBacktrace(const std::string_view& line, std::string_view* index,
                       std::string_view* address, std::string_view* function,
                       std::string_view* file) CHK;

  /**
   * @brief Check if the source file exists.
   *
   * @return true if the source file is known.
   */
  bool HasSource() const CHK { return file_.length() != 0; }

  /**
   * @brief Get the file where the function of the backtrace is.
   *
   * @return The filename.
   */
  const std::string& GetFile() const CHK { return file_; }

  /**
   * @brief Get the line of the function. This number is available only if
   * HasSource() is true.
   *
   * @return The number of the line in the file.
   */
  size_t GetLine() const CHK { return line_; }

 private:
  /**
   * @brief The nth backtrace of the stack.
   */
  size_t index_;
  /**
   * @brief Address in memory of the backtrace.
   */
  uint64_t address_;
  /**
   * @brief Store all informations about the function of the backtrace.
   */
  Function function_;
  /**
   * @brief The source filename.
   */
  std::string file_;
  /**
   * @brief The line in the source filename.
   */
  size_t line_;

  /**
   * @brief Convert the index of the backtrace in index_.
   *
   * @param[in] number The number in format "0".
   *
   * @return true if the parameter number has the right format.
   */
  bool ReadIndex(const std::string& number) CHK;
  /**
   * @brief Convert the adress number of the backtrace in address_.
   *
   * @param[in] address The address in format "0xDEADBEEF" or
   * "0xDEADBEEFDEADBEEF".
   *
   * @return true if parameter address has the right format.
   */
  bool ReadAddress(const std::string& address) CHK;
  /**
   * @brief Convert the function of the backtrace in function_.
   *
   * @param[in] description The function in format "function (arg1=X, arg2=y)".
   *
   * @return true if parameter description has the right format.
   */
  bool ReadFunction(const std::string_view& description) CHK;
  /**
   * @brief Convert the filename and the number of the line of the backtrace in
   * file_ and line_.
   *
   * @param[in] file The file in format "filename.cpp:1234".
   *
   * @return true if parameter file has the right format.
   */
  bool ReadSource(const std::string_view& file) CHK;
};

#endif  // BACKTRACE_H_
