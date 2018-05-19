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
 * @file function.h
 * @brief Store all informations about functions.
 */

#ifndef SOFTWARE_GDB_FUNCTION_H_
#define SOFTWARE_GDB_FUNCTION_H_

#include <2lgc/compat.h>
#include <string>
#include <utility>
#include <vector>

namespace llgc::software::gdb {

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

}

#endif  // SOFTWARE_GDB_FUNCTION_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
