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

#include "backtrace.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

Bt::Bt()
    : index_(0),
      address_(0),
      function_(),
      file_(),
      line_(std::numeric_limits<size_t>::max())
{
}

std::unique_ptr<Bt> Bt::Factory(const std::string_view& line)
{
  std::string_view index, address, function, file;

  std::unique_ptr<Bt> retval = std::make_unique<Bt>();

  if (!retval->DecodeBacktrace(line, &index, &address, &function, &file))
  {
    return std::unique_ptr<Bt>(nullptr);
  }

  if (!retval->ReadIndex(std::string(index)))
  {
    return std::unique_ptr<Bt>(nullptr);
  }

  if (address.length() != 0 && !retval->ReadAddress(std::string(address)))
  {
    return std::unique_ptr<Bt>(nullptr);
  }

  if (function.length() != 0 && !retval->ReadFunction(function))
  {
    return std::unique_ptr<Bt>(nullptr);
  }

  if (file.length() != 0 && !retval->ReadSource(file))
  {
    return std::unique_ptr<Bt>(nullptr);
  }

  return retval;
}

bool Bt::DecodeBacktrace(const std::string_view& line, std::string_view* index,
                         std::string_view* address, std::string_view* function,
                         std::string_view* file)
{
  assert(index);
  assert(address);
  assert(function);
  assert(file);

  std::string line_it(line);
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
    else if ('0' > line[i] || line[i] > '9')
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
        break;
      }
    }
    if (ibis != inpos)
    {
      return false;
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
      i = infrom + 5;
      *file = line.substr(i);
    }
    // No as or from.
    else
    {
      return false;
    }
  }

  // A function must have a '(' before ')'.
  if (function->find('(') == std::string::npos)
  {
    return false;
  }

  return true;
}

bool Bt::ReadIndex(const std::string& number)
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

bool Bt::ReadAddress(const std::string& number)
{
  try
  {
    address_ = std::stoull(number, nullptr, 0);
  }
  catch (const std::out_of_range&)
  {
    return false;
  }
  return true;
}

bool Bt::ReadFunction(const std::string_view& description)
{
  size_t pos_space = description.find(' ');

  if (pos_space == std::string::npos)
  {
    return false;
  }

  function_.SetName(description.substr(0, pos_space));

  // + 2 to remove " ("
  std::string_view str =
      description.substr(pos_space + 2,
                         // - 3 to remove " (" and ")"
                         description.length() - pos_space - 3);

  // No argument.
  if (str.length() == 0)
  {
    return true;
  }

  // Split with delimiter ", "
  size_t pos_comma = str.find(", ");
  if (pos_comma == std::string::npos)
  {
    pos_comma = str.length();
  }
  while (pos_comma != std::string::npos)
  {
    std::string_view strcomma = str.substr(0, pos_comma);
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
      pos_comma = str.find(", ");
      if (pos_comma == std::string::npos)
      {
        pos_comma = str.length();
      }
    }
    else
    {
      pos_comma = std::string::npos;
    }
  }

  return true;
}

bool Bt::ReadSource(const std::string_view& file)
{
  size_t pos = file.find_last_of(':');
  if (pos == std::string::npos)
  {
    return false;
  }

  try
  {
    line_ = static_cast<size_t>(std::stoi(file.substr(pos + 1).data()));
  }
  catch (const std::invalid_argument&)
  {
    std::cerr << "Invalid file " << file << std::endl;
    return false;
  }
  catch (const std::out_of_range&)
  {
    std::cerr << "Invalid file " << file << std::endl;
    return false;
  }

  file_ = file.substr(0, pos);

  return true;
}
