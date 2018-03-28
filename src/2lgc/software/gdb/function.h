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
 * @file function.h
 * @brief Store all informations about functions.
 */

#ifndef SOFTWARE_GDB_FUNCTION_H_
#define SOFTWARE_GDB_FUNCTION_H_

#include <2lgc/compatibility/visual_studio.h>
#include <string>
#include <utility>
#include <vector>

namespace llgc::software::gdb
{
/**
 * @brief Store all informations about a function.
 *
 * @details Contains the name of the function, the arguments and their value.
 */
class Function
{
 public:
  /**
   * @brief Default constructor.
   */
  Function() : name_(), args_() {}

  /**
   * @brief Get the name of a function.
   *
   * @return The name of the function.
   */
  const std::string& GetName() const CHK { return name_; }

  /**
   * @brief Setter for the name of a function.
   *
   * @param[in] name The new name of a function.
   */
  void SetName(const std::string& name) { name_.assign(name); }

  /**
   * @brief Add argument of the function.
   *
   * @param[in] name Name of the argument.
   * @param[in] value Value of the argument.
   */
  void AddArgs(const std::string& name, const std::string& value)
  {
    args_.push_back(std::pair<std::string, std::string>(name, value));
  }

  /**
   * @brief Check if a line is a valid variable line.
   *
   * @param[in] variable The line to check.
   *
   * @return true if valid variable line.
   */
  static bool IsValidVariableLine(const std::string& variable);

  /**
   * @brief Check if a variable line is wrappable.
   *
   * @param[in] variable The line to check.
   *
   * @return true if variable line is wrappable.
   */
  static bool IsVariableLineWrappable(const std::string& variable);

 private:
  /**
   * @brief Name of the function.
   */
  std::string name_;

  /**
   * @brief All the arguments. Key is the name of the arg, value is the value.
   */
  std::vector<std::pair<std::string, std::string>> args_;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_FUNCTION_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
