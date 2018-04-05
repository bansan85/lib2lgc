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

#include <2lgc/software/gdb/backtrace.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <limits>
#include <memory>
#include <string>

int main(int /* argc */, char* /* argv */ [])  // NS
{
  std::unique_ptr<llgc::software::gdb::Backtrace> bt =
      llgc::software::gdb::Backtrace::Factory(
          "#4  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
          "interface_=<optimized out>, ext=<optimized out>) at "
          "libraries/libdxfrw/src/libdxfrw.cpp:99");
  assert(bt != nullptr);
  assert(bt->HasSource());
  assert(bt->GetName() == "dxfRW::read");
  assert(bt->GetFile() == "libraries/libdxfrw/src/libdxfrw.cpp");
  assert(bt->GetIndex() == 4);
  assert(bt->GetLine() == 99);

  // Line to small
  bt = llgc::software::gdb::Backtrace::Factory("1");
  assert(bt == nullptr);

  // Invalid line
  bt = llgc::software::gdb::Backtrace::Factory("11");
  assert(bt == nullptr);

  // Invalid line
  bt = llgc::software::gdb::Backtrace::Factory("# ");
  assert(bt == nullptr);

  // Invalid line
  bt = llgc::software::gdb::Backtrace::Factory("#i ");
  assert(bt == nullptr);

  // Invalid line
  bt = llgc::software::gdb::Backtrace::Factory("#243");
  assert(bt == nullptr);

  // Index to big
  bt = llgc::software::gdb::Backtrace::Factory(
      "#5555555555  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:99");
  assert(bt == nullptr);

  // Address to big
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x55555571cb4c00000 in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:99");
  assert(bt == nullptr);

  // Bad address
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x00005o555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:99");
  assert(bt == nullptr);

  // Invalid function
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x000055555571cb4c in dxfRW::read(this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:614891481347470131200");
  assert(bt == nullptr);

  // Double argument
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x000055555571cb4c in dxfRW::read (this=this_=0x7fffffffd4f0, "
      "interf=interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:6");
  assert(bt != nullptr);

  // Argument with ", "
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x000055555571cb4c in dxfRW::read (this=this_=0x7fffffffd4f0, "
      "fileFormat=std::shared_ptr (count 3, weak 0) 0x555557048130, "
      "ext=<optimized out>) at libraries/libdxfrw/src/libdxfrw.cpp:6");
  assert(bt != nullptr);

  // Wrong argument
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x000055555571cb4c in dxfRW::read (this=this_=0x7fffffffd4f0, "
      "interf, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:6");
  assert(bt == nullptr);

  // Line to big
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:614891481347470131200");
  assert(bt == nullptr);

  // Invalid line
  bt = llgc::software::gdb::Backtrace::Factory(
      "#4  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
      "interface_=<optimized out>, ext=<optimized out>) at "
      "libraries/libdxfrw/src/libdxfrw.cpp:aa");
  assert(bt == nullptr);

  // from in bt
  bt = llgc::software::gdb::Backtrace::Factory(
      "#10 0x401d7877 in gtk_marshal_NONE__NONE () "
      "from /usr/lib/libgtk-1.2.so.0");
  assert(bt != nullptr);
  assert(bt->HasSource());
  assert(bt->GetFile() == "/usr/lib/libgtk-1.2.so.0");
  assert(bt->GetName() == "gtk_marshal_NONE__NONE");
  assert(bt->GetIndex() == 10);
  assert(bt->GetLine() == std::numeric_limits<size_t>::max());

  // No source
  bt = llgc::software::gdb::Backtrace::Factory(
      "#8  0x00002b2153382d4a in ?? ()");
  assert(bt != nullptr);
  assert(!bt->HasSource());
  assert(bt->GetName() == "??");
  assert(bt->GetIndex() == 8);
  assert(bt->GetLine() == std::numeric_limits<size_t>::max());

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
