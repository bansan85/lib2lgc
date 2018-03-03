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

/**
 * @file function.h
 * @brief Store all informations about functions.
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

// C++ system
#include <string>
#include <utility>
#include <vector>

// Compatibility
#include <compat.h>

/**
 * @brief Store all informations about a function.
 *
 * @details Contains the name of the function, the arguments and their value.
 */
class Function
{
 public:
  /**
   * @brief Default constructor.
   */
  Function() : name(), args() {}

  /**
   * @brief Name of the function.
   */
  std::string name;

  /**
   * @brief All the arguments. Key is the name of the arg, value is the value.
   */
  std::vector<std::pair<std::string, std::string>> args;
};

#endif  // FUNCTION_H_
