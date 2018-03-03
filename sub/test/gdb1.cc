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

// C++ system
#include <cassert>
#include <memory>

// lib2lgcgdb
#include <backtrace.h>

int main(int /* argc */, char* /* argv */ [])
{
  std::string line(
      "#4  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:99");
  std::unique_ptr<Bt> bt = std::make_unique<Bt>(
      "#4  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:99");

  return 0;
}
