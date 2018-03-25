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

/**
 * @brief Namespace that have all function that override classic classes.
 */
namespace llgc::override
{
/**
 * @brief This class allow use of a safe printf.
 */
class Print
{
 public:
  /**
   * @brief This function override the less safe printf.
   *
   * It's safer because you don't have to know if it's %d %zu …. Just a % is
   * enough.
   *
   * @tparam T Type of the next variable to print.
   * @tparam Args Type of the others variables.
   * @param[out] out_stream The output stream.
   * @param[in] s The message to show with % to show the next variable.
   * @param[in] value The next variable to print.
   * @param[in] args The others variables.
   */
  template <typename T, typename... Args>
  static void F(std::ostream &out_stream, const char *s, T value, Args... args)
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
          F(out_stream, s + 1, args...);
          return;
        }
      }
      out_stream << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
  }

 private:
  /**
   * @brief Print a message to an ouput stream.
   *
   * @param[out] out_stream The output stream (std::cout, …).
   * @param[in] s The string message.
   */
  static void F(std::ostream &out_stream, const std::string &s);
};

}  // namespace llgc::override

#endif  // OVERRIDE_PRINTF_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
