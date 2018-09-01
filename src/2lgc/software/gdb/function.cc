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

/** \class llgc::software::gdb::Function
 * \brief Store all informations about a function.
 * \details Contains the name of the function, the arguments and their value.
 *
 *
 * \fn const std::string& llgc::software::gdb::Function::GetName() const
 * \brief Get the name of a function.
 * \return The name of the function.
 *
 *
 * \fn void llgc::software::gdb::Function::SetName(const std::string& name)
 * \brief Setter for the name of a function.
 * \param[in] name The new name of a function.
 *
 *
 * \fn void llgc::software::gdb::Function::AddArgs(std::string name, std::string value)
 * \brief Add argument of the function.
 * \param[in] name Name of the argument.
 * \param[in] value Value of the argument.
 */

namespace llgc::software::gdb
{
namespace
{
/** \brief Check if a string is a valid name for Gdb.
 * \details Only a-z, A-Z, 0-9 and _ is allowed. XX::YY is also right.
 * \param[in] name The name to check.
 * \return true if valid name.
 */
bool IsValidName(const std::string &name)
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
                "Invalid name of argument '" + name + "'.\n");
      }
    }
  }
  return true;
}

/**
 * @brief Check if string is a valid line for Gdb and is not a stack line.
 * @param[in] variable The line to check.
 * @return true if valid line.
 */
bool IsStartVariable(const std::string &variable)
{
  return (variable.compare(0, 8, "        ") == 0) &&
         (variable.find(" = ") != std::string::npos) &&
         (variable.find(" = ") != 8) &&
         (IsValidName(variable.substr(8, variable.find(" = ") - 8)));
}

}  // namespace
}  // namespace llgc::software::gdb

/** \brief Check if a line is a valid variable line.
 * \param[in] variable The line to check.
 * \return true if valid variable line.
 */
bool llgc::software::gdb::Function::IsValidVariableLine(
    const std::string &variable)
{
  // LANG="" must set.
  return (variable == "No locals.") ||
         (variable == "No symbol table info available.") ||
         (variable == "No stack.") || (IsStartVariable(variable));
}

/** \brief Check if a variable line is wrappable.
 * \param[in] variable The line to check.
 * \return true if variable line is wrappable.
 */
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

/** \var llgc::software::gdb::Function::name_
 * \brief Name of the function.
 *
 *
 * \var llgc::software::gdb::Function::args_
 * \brief All the arguments. Key is the name of the arg, value is the value.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
