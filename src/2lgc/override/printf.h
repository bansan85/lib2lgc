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

#ifndef OVERRIDE_PRINTF_H_
#define OVERRIDE_PRINTF_H_

#include <ostream>
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
   *
   * @return true if number of arguments is compatible with the format.
   */
  template <typename T, typename... Args>
  static bool F(std::ostream &out_stream, const char *s, T value, Args... args)
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
          return F(out_stream, s + 1, args...);
        }
      }
      out_stream << *s++;
    }
    return false;
  }

 private:
  /**
   * @brief Print a message to an ouput stream.
   *
   * @param[out] out_stream The output stream (std::cout, …).
   * @param[in] s The string message.
   *
   * @return true if number of arguments is compatible with the format.
   */
  static bool F(std::ostream &out_stream, const std::string &s);
};

}  // namespace llgc::override

#endif  // OVERRIDE_PRINTF_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
