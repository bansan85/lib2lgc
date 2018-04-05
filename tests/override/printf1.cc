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

#include <2lgc/override/printf.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <sstream>

int main(int /* argc */, char* /* argv */ [])  // NS
{
  std::ostringstream oss;

  assert(llgc::override::Print::F(oss, "number: %, string: %, double: %", 2,
                                  "text", 1.5));
  assert(oss.str() == "number: 2, string: text, double: 1.5");
  oss.str("");

  assert(llgc::override::Print::F(oss, "number: %%, string: %.", "text"));
  assert(oss.str() == "number: %, string: text.");
  oss.str("");

  assert(llgc::override::Print::F(oss, "number: %, string: %%.", 124));
  assert(oss.str() == "number: 124, string: %.");
  oss.str("");

  assert(!llgc::override::Print::F(oss, "number: %, string: %, double: %", 2,
                                   "text"));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
