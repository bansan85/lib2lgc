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

#ifndef SOFTWARE_GDB_FUNCTION_H_
#define SOFTWARE_GDB_FUNCTION_H_

#include <2lgc/compat.h>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace llgc::software::gdb
{
class Function
{
 public:
  const std::string& GetName() const CHK { return name_; }
  void SetName(const std::string& name) { name_.assign(name); }
  void AddArgs(std::string name, std::string value)
  {
    args_.emplace_back(std::make_pair(std::move(name), std::move(value)));
  }

  static bool IsValidVariableLine(const std::string& variable);
  static bool IsVariableLineWrappable(const std::string& variable);

 private:
  std::string name_;
  std::vector<std::pair<std::string, std::string>> args_;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_FUNCTION_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
