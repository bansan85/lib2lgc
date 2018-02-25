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

#ifndef STACK_H_
#define STACK_H_

// C++ system
#include <memory>
#include <string_view>
#include <vector>

#include "backtrace.h"

class Stack {
 public:
  Stack(const std::string_view& filename);

  bool InterpretLine(const std::string_view& line);
  const std::string& GetFilename() const { return filename_; }

  size_t NumberOfBacktraces() const { return backtraces_.size(); }

  const Bt* GetBacktraceFromTop(size_t i) const { return backtraces_[i].get(); }

  const Bt* GetBacktraceFromBottom(size_t i) const {
    return backtraces_[backtraces_.size() - 1 - i].get();
  }

 private:
  std::string filename_;
  std::vector<std::unique_ptr<Bt>> backtraces_;
};

#endif  // STACK_H_
