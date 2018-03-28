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

#include <2lgc/software/gdb/function.h>
#include <algorithm>
#include <cstddef>

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
        return false;
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
