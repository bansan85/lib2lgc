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

#ifndef OVERRIDE_PRINTF_H_
#define OVERRIDE_PRINTF_H_

#include <ostream>
#include <stdexcept>
#include <string>

// Stolen from
// https://www.codeproject.com/Articles/514443/Debug-Print-in-Variadic-Template-Style#printf

class Override
{
 public:
  static void SafePrintf(std::ostream &out_stream, const std::string &s);

  template <typename T, typename... Args>
  static void SafePrintf(std::ostream &out_stream, const char *s, T value,
                         Args... args)
  {
    while (*s)
    {
      if (*s == '%')
      {
        if (*(s + 1) == '%')
        {
          ++s;
        }
        else
        {
          out_stream << value;
          SafePrintf(out_stream, s + 1, args...);
          return;
        }
      }
      out_stream << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
  }
};

#endif  // OVERRIDE_PRINTF_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
