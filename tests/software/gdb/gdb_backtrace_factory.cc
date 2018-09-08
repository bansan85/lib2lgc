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

#include <2lgc/software/gdb/backtrace.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <limits>
#include <memory>
#include <string>

int main(int /* argc */, char * /* argv */ [])  // NS
{
  // Check good lines
  std::unique_ptr<llgc::software::gdb::Backtrace> bt =
      llgc::software::gdb::Backtrace::Factory(
          "#4  0x000055555571cb4c in dxfRW::read (this=0x7fffffffd4f0, "
          "interface_=<optimized out>, ext=<optimized out>) at "
          "libraries/libdxfrw/src/libdxfrw.cpp:99");
  assert(bt != nullptr);
  assert(bt->GetAddress() == 0x000055555571cb4c);
  assert(bt->HasSource());
  assert(bt->GetName() == "dxfRW::read");
  assert(bt->GetFile() == "libraries/libdxfrw/src/libdxfrw.cpp");
  assert(bt->GetIndex() == 4);
  assert(bt->GetLine() == 99);

  // from in bt
  bt = llgc::software::gdb::Backtrace::Factory(
      "#10 0x401d7877 in gtk_marshal_NONE__NONE () "
      "from /usr/lib/libgtk-1.2.so.0");
  assert(bt != nullptr);
  assert(bt->GetAddress() == 0x401d7877);
  assert(bt->HasSource());
  assert(bt->GetFile() == "/usr/lib/libgtk-1.2.so.0");
  assert(bt->GetName() == "gtk_marshal_NONE__NONE");
  assert(bt->GetIndex() == 10);
  assert(bt->GetLine() == std::numeric_limits<size_t>::max());

  // No source
  bt = llgc::software::gdb::Backtrace::Factory(
      "#8  0x00002b2153382d4a in ?? ()");
  assert(bt != nullptr);
  assert(bt->GetAddress() == 0x00002b2153382d4a);
  assert(!bt->HasSource());
  assert(bt->GetName() == "??");
  assert(bt->GetIndex() == 8);
  assert(bt->GetLine() == std::numeric_limits<size_t>::max());

  // No adress
  bt = llgc::software::gdb::Backtrace::Factory(
      "#3  emit_page (g=0x555555607000, job=0x55555557fa50) at emit.c:3537");
  assert(bt != nullptr);
  assert(bt->GetAddress() == 0);
  assert(bt->HasSource());
  assert(bt->GetFile() == "emit.c");
  assert(bt->GetName() == "emit_page");
  assert(bt->GetIndex() == 3);
  assert(bt->GetLine() == 3537);

  // Complex string
  bt = llgc::software::gdb::Backtrace::Factory(
      "#0  findStopColor (colorlist=colorlist@entry=0x555555588c20 \"XXX\", "
      "'p' <repeats 15 times>, \"YYY\"..., clrs=clrs@entry=0x7fffffffd270, "
      "frac=frac@entry=0x7fffffffd26c) at emit.c:4231");
  assert(bt != nullptr);
  assert(bt->GetAddress() == 0);
  assert(bt->HasSource());
  assert(bt->GetFile() == "emit.c");
  assert(bt->GetName() == "findStopColor");
  assert(bt->GetIndex() == 0);
  assert(bt->GetLine() == 4231);

  // Check wrong lines.
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

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
