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

#ifndef BT_H_
#define BT_H_

// C++ system
#include <string_view>

#include "function.h"

class Bt {
 public:
  Bt(const std::string_view& line);

  bool DecodeBacktrace(const std::string_view& line, std::string_view& index,
                       std::string_view& address, std::string_view& function,
                       std::string_view& file);
  bool HasSource() const { return file_.length() != 0; }
  const std::string& GetFile() const { return file_; }
  size_t GetLine() const { return line_; }

 private:
  // The nth backtrace of the stack.
  size_t index_;
  // Address of the PC.
  uint64_t address_;
  Function function_;
  std::string file_;
  size_t line_;

  bool ReadIndex(const std::string& number);
  bool ReadAddress(const std::string& address);
  bool ReadFunction(const std::string_view& description);
  bool ReadSource(const std::string_view& file);
};

#endif  // BT_H_
