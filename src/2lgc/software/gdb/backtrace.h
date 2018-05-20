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
#include <memory>
#include <string>

namespace llgc::software::gdb
{
class Stack;
/**
 * @brief Store all informations about one backtrace.
 */
class Backtrace
{
 public:
  /**
   * @brief Iterator for Backtrace.
   */
  class Iter : public llgc::pattern::Iterator<Stack, Backtrace>
  {
   public:
    /**
     * @brief The constructor
     *
     * @param[in] stack The stack contains the list of backtraces.
     * @param[in] pos The position of the current backtrace.
     */
    Iter(const Stack& stack, size_t pos)
        : llgc::pattern::Iterator<Stack, Backtrace>(stack, pos)
    {
    }

    /**
     * @brief Dereference an iterator return the current backtrace.
     *
     * @return Return the current backtrace.
     */
    const Backtrace& operator*() const override;
  };

  /**
   * @brief Default constructor.
   */
  Backtrace();

  /**
   * @brief Create a Backtrace based on the line.
   *
   * @param[in] line The line in format
   *   "#d  0xDEADBEEF in Function (args="") at filename:1234" or
   *   "#d  0xDEADBEEF in Function (args="") from library"
   *
   * @return The new backtrace and null in failed.
   */
  static std::unique_ptr<Backtrace> Factory(const std::string& line);

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
  static bool DecodeBacktrace(const std::string& line, std::string* index,
                              std::string* address, std::string* function,
                              std::string* file) CHK;

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
   * @brief Get the name of the function.
   *
   * @return The name of the function.
   */
  const std::string& GetName() const CHK { return function_.GetName(); }

  /**
   * @brief Get the line of the function. This number is available only if
   * HasSource() is true.
   *
   * @return The number of the line in the file.
   */
  size_t GetLine() const CHK { return line_; }

  /**
   * @brief Get the index of the backtrace.
   *
   * @return The indef of the backtrace.
   */
  size_t GetIndex() const CHK { return index_; }

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
  bool ReadFunction(const std::string& description) CHK;
  /**
   * @brief Helper to find next argument.
   *
   * @param[in] args All arguments.
   *
   * @return Return the pos of the next argument. If failed, the length on the
   * string is return.
   */
  size_t FindNextArg(const std::string& args);
  /**
   * @brief Convert the filename and the number of the line of the backtrace in
   * file_ and line_.
   *
   * @param[in] file The file in format "filename.cpp:1234".
   *
   * @return true if parameter file has the right format.
   */
  bool ReadSource(const std::string& file) CHK;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_BACKTRACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
