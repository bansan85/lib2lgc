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

#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/function.h>
#include <2lgc/software/gdb/stack.h>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <string>

llgc::software::gdb::Backtrace::Backtrace()
    : index_(0), address_(0), line_(std::numeric_limits<size_t>::max())
{
}

std::unique_ptr<llgc::software::gdb::Backtrace>
llgc::software::gdb::Backtrace::Factory(const std::string& line)
{
  std::string index, address, function, file;

  std::unique_ptr<Backtrace> retval = std::make_unique<Backtrace>();

  if (!retval->DecodeBacktrace(line, &index, &address, &function, &file))
  {
    return std::unique_ptr<Backtrace>(nullptr);
  }

  if (!retval->ReadIndex(index))
  {
    return std::unique_ptr<Backtrace>(nullptr);
  }

  if (address.length() != 0 && !retval->ReadAddress(address))
  {
    return std::unique_ptr<Backtrace>(nullptr);
  }

  if (function.length() != 0 && !retval->ReadFunction(function))
  {
    return std::unique_ptr<Backtrace>(nullptr);
  }

  if (file.length() != 0 && !retval->ReadSource(file))
  {
    if (!retval->ReadSource(file))
    {
      return std::unique_ptr<Backtrace>(nullptr);
    }
  }

  return retval;
}

bool llgc::software::gdb::Backtrace::DecodeBacktrace(const std::string& line,
                                                     std::string* index,
                                                     std::string* address,
                                                     std::string* function,
                                                     std::string* file)
{
  assert(index);
  assert(address);
  assert(function);
  assert(file);

  // Regex: "^#(\\d+) *((0x.*) in )?((.*\\)) at )?(.*)$"
  // Size for the beginning #\d
  size_t line_length = line.length();
  if (line_length < 2)
  {
    return false;
  }

  // First char: '#'
  if (line[0] != '#')
  {
    return false;
  }

  // Next chars: decimal until space.
  size_t i = 1;
  while (i < line_length)
  {
    if (line[i] == ' ')
    {
      // No decimal.
      if (i == 1)
      {
        return false;
      }
      break;
    }
    if ('0' > line[i] || line[i] > '9')
    {
      return false;
    }
    i++;
  }

  if (i == line_length)
  {
    return false;
  }

  *index = line.substr(1, i - 1);

  while (i < line_length && line[i] == ' ')
  {
    i++;
  }

  // If " in " is there, following chars must be 0xhhhhhh…
  const size_t inpos = line.find(" in ");
  if (inpos != std::string::npos && line[i] == '0' && line[i + 1] == 'x')
  {
    // We have the good pattern.
    size_t ibis;
    for (ibis = i + 2; ibis < inpos; ibis++)
    {
      if (('0' > line[ibis] || line[ibis] > '9') &&
          ('a' > line[ibis] || line[ibis] > 'f'))
      {
        return false;
      }
    }

    *address = line.substr(i, inpos - i);

    i = inpos + 4;
  }

  // " at " state.
  const size_t inas = line.find(") at ", i);
  if (inas != std::string::npos)
  {
    *function = line.substr(i, inas - i + 1);
    i = inas + 5;
    *file = line.substr(i);
  }
  else
  {
    const size_t infrom = line.find(") from ", i);
    if (infrom != std::string::npos)
    {
      *function = line.substr(i, infrom - i + 1);
      i = infrom + 7;
      *file = line.substr(i);
    }
    // No at or from.
    else
    {
      *function = line.substr(i);
    }
  }

  // A function must have a '(' before ')'.
  return function->find('(') != std::string::npos;
}

bool llgc::software::gdb::Backtrace::ReadIndex(const std::string& number)
{
  try
  {
    index_ = static_cast<size_t>(std::stoi(number, nullptr, 10));
  }
  catch (const std::out_of_range&)
  {
    return false;
  }
  return true;
}

bool llgc::software::gdb::Backtrace::ReadAddress(const std::string& address)
{
  try
  {
    address_ = std::stoull(address, nullptr, 0);
  }
  catch (const std::out_of_range&)
  {
    return false;
  }
  return true;
}

bool llgc::software::gdb::Backtrace::ReadFunction(
    const std::string& description)
{
  // ' ' is not enought : loader::(anonymous namespace)::createImageImpl (
  size_t pos_space = description.find(" (");

  if (pos_space == std::string::npos)
  {
    return false;
  }

  function_.SetName(description.substr(0, pos_space));

  // + 2 to remove " ("
  std::string str = description.substr(pos_space + 2,
                                       // - 3 to remove " (" and ")"
                                       description.length() - pos_space - 3);

  // No argument.
  if (str.length() == 0)
  {
    return true;
  }

  // Split with delimiter ", "
  size_t pos_comma = FindNextArg(str);

  while (pos_comma != std::string::npos)
  {
    std::string strcomma = str.substr(0, pos_comma);
    size_t pos_last_equal = strcomma.find_last_of('=');
    if (pos_last_equal == std::string::npos)
    {
      return false;
    }
    size_t pos_equal = strcomma.find('=');
    while (pos_equal != pos_last_equal)
    {
      function_.AddArgs(strcomma.substr(0, pos_equal),
                        strcomma.substr(pos_last_equal + 1));
      strcomma = strcomma.substr(pos_equal + 1);
      pos_last_equal -= pos_equal + 1;
      pos_equal = strcomma.find('=');
    }
    function_.AddArgs(strcomma.substr(0, pos_equal),
                      strcomma.substr(pos_last_equal + 1));

    if (pos_comma != str.length())
    {
      str = str.substr(pos_comma + 2);
      pos_comma = FindNextArg(str);
    }
    else
    {
      pos_comma = std::string::npos;
    }
  }

  return true;
}

size_t llgc::software::gdb::Backtrace::FindNextArg(const std::string& args)
{
  size_t pos_comma;
  size_t start_find = 0;
  size_t nb_start_parenthese = 0;
  size_t nb_end_parenthese = 0;

  // Test case:
  // fileFormat=std::shared_ptr (count 3, weak 0) 0x555557048130
  do
  {
    pos_comma = args.find(", ", start_find);

    if (pos_comma == std::string::npos)
    {
      return args.length();
    }
    for (size_t i = 0; i < pos_comma; i++)
    {
      if (args[i] == '(')
      {
        nb_start_parenthese++;
      }
      else if (args[i] == ')')
      {
        nb_end_parenthese++;
      }
    }

    start_find = pos_comma + 1;
  } while (nb_start_parenthese != nb_end_parenthese);

  return pos_comma;
}

bool llgc::software::gdb::Backtrace::ReadSource(const std::string& file)
{
  size_t pos = file.find_last_of(':');
  // #10 0x401d7877 in gtk_marshal_NONE__NONE () from /usr/lib/libgtk-1.2.so.0
  // No line because no source file but library.
  if (pos == std::string::npos)
  {
    file_ = file;
    return true;
  }

  try
  {
    line_ = static_cast<size_t>(std::stoi(file.substr(pos + 1)));
  }
  catch (const std::invalid_argument&)
  {
    return false;
  }
  catch (const std::out_of_range&)
  {
    return false;
  }

  file_ = file.substr(0, pos);

  return true;
}

const llgc::software::gdb::Backtrace& llgc::software::gdb::Backtrace::Iter::
operator*() const
{
  return data_.GetBacktraceFromTop(pos_);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
