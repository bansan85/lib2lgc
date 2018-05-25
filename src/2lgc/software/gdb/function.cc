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

#include <2lgc/error/show.h>
#include <2lgc/software/gdb/function.h>
#include <algorithm>
#include <cstddef>
#include <iostream>

static bool IsValidName(const std::string &name)
{
  for (size_t i = 0; i < name.length(); i++)
  {
    if (('a' > name[i] || name[i] > 'z') && ('A' > name[i] || name[i] > 'Z') &&
        ('0' > name[i] || name[i] > '9') && (name[i] != '_'))
    {
      // for example: "llvm::None = <optimized out>"
      if (name[i] == ':' && i != name.length() - 1 && name[i + 1] == ':')
      {
        i++;
      }
      else
      {
        BUGUSER(std::cout, false, false,
                "Invalid name of argument '" << name << "'.\n");
      }
    }
  }
  return true;
}

static bool IsStartVariable(const std::string &variable)
{
  return (variable.compare(0, 8, "        ") == 0) &&
         (variable.find(" = ") != std::string::npos) &&
         (variable.find(" = ") != 8) &&
         (IsValidName(variable.substr(8, variable.find(" = ") - 8)));
}

bool llgc::software::gdb::Function::IsValidVariableLine(
    const std::string &variable)
{
  // LANG="" must set.
  return (variable == "No locals.") ||
         (variable == "No symbol table info available.") ||
         (IsStartVariable(variable));
}

bool llgc::software::gdb::Function::IsVariableLineWrappable(
    const std::string &variable)
{
  std::string three_dots("...");
  if (std::equal(three_dots.rbegin(), three_dots.rend(), variable.rbegin()))
  {
    return false;
  }
  // Wrappable possible if exception in Gdb.
  return (variable.find(" Cannot find type ") != std::string::npos) ||
         (variable.find(" Python Exception ") != std::string::npos);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
