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
#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/function.h>
#include <2lgc/software/gdb/stack.h>
#include <iostream>
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

  BUGCONT(std::cout,
          retval->DecodeBacktrace(line, &index, &address, &function, &file),
          std::unique_ptr<Backtrace>(nullptr));
  BUGCONT(std::cout, retval->ReadIndex(index),
          std::unique_ptr<Backtrace>(nullptr));
  BUGCONT(std::cout, address.length() == 0 || retval->ReadAddress(address),
          std::unique_ptr<Backtrace>(nullptr));
  BUGCONT(std::cout, function.length() == 0 || retval->ReadFunction(function),
          std::unique_ptr<Backtrace>(nullptr));
  BUGCONT(std::cout, file.length() == 0 || retval->ReadSource(file),
          std::unique_ptr<Backtrace>(nullptr));

  return retval;
}

bool llgc::software::gdb::Backtrace::DecodeBacktrace(const std::string& line,
                                                     std::string* index,
                                                     std::string* address,
                                                     std::string* function,
                                                     std::string* file)
{
  BUGPARAM(std::cout, index, index != nullptr, false);
  BUGPARAM(std::cout, address, address != nullptr, false);
  BUGPARAM(std::cout, function, function != nullptr, false);
  BUGPARAM(std::cout, file, file != nullptr, false);

  // Regex: "^#(\\d+) *((0x.*) in )?((.*\\)) at )?(.*)$"
  // Size for the beginning #\d
  size_t line_length = line.length();
  BUGUSER(std::cout, line_length >= 2, false,
          "Length of line to small '" << line << "'.\n");
  // First char: '#'
  BUGUSER(std::cout, line[0] == '#', false,
          "Line doesn't start with '#' '" << line << "'.\n");

  // Next chars: decimal until space.
  size_t i = 1;
  while (i < line_length)
  {
    if (line[i] == ' ')
    {
      // No decimal.
      BUGUSER(std::cout, i != 1, false,
              "Line doesn't have a number after '#' '" << line << "'.\n");
      break;
    }
    BUGUSER(std::cout, '0' <= line[i] && line[i] <= '9', false,
            "Line doesn't have a valid number after '#' '" << line << "'.\n");
    i++;
  }

  BUGUSER(std::cout, i != line_length, false,
          "Line truncated '" << line << "'.\n");

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
      BUGUSER(
          std::cout,
          ('0' <= line[ibis] && line[ibis] <= '9') ||
              ('a' <= line[ibis] && line[ibis] <= 'f'),
          false,
          "Line doesn't have a valid hexadecimal number '" << line << "'.\n");
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
  BUGUSER(std::cout, function->find('(') != std::string::npos, false,
          "Function doesn't have arguments '" << line << "'.\n");
  return true;
}

bool llgc::software::gdb::Backtrace::ReadIndex(const std::string& number)
{
  try
  {
    index_ = static_cast<size_t>(std::stoi(number, nullptr, 10));
  }
  catch (const std::out_of_range&)
  {
    BUGUSER(std::cout, false, false,
            "Index doesn't have valid number '" << number << "'.\n");
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
    BUGUSER(std::cout, false, false,
            "Address doesn't have valid number '" << address << "'.\n");
  }
  return true;
}

bool llgc::software::gdb::Backtrace::ReadFunction(
    const std::string& description)
{
  // ' ' is not enought : loader::(anonymous namespace)::createImageImpl (
  size_t pos_space = description.find(" (");

  BUGUSER(std::cout, pos_space != std::string::npos, false,
          "Function doesn't have arguments '" << description << "'.\n");

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
    BUGUSER(std::cout, pos_last_equal != std::string::npos, false,
            "Function doesn't have valid arguments '" << description << "'.\n");
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
    BUGUSER(std::cout, false, false,
            "Source line is invalid '" << file << "'.\n");
  }
  catch (const std::out_of_range&)
  {
    BUGUSER(std::cout, false, false,
            "Source line is out of range '" << file << "'.\n");
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
