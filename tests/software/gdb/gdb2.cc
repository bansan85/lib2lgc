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
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <experimental/filesystem>
#include <map>
#include <memory>
#include <string>

int main(int argc, char* argv[])  // NS
{
  assert(argc == 2);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  std::experimental::filesystem::path folder(argv[1]);
  assert(std::experimental::filesystem::is_directory(folder));

  std::unique_ptr<llgc::software::gdb::SetStack> set_stack =
      std::make_unique<llgc::software::gdb::SetStack>(true, 4, 4, true);

  assert(!set_stack->Add(folder / "notfound.fail"));
  assert(set_stack->Count() == 0);
  assert(!set_stack->Add(folder / "btfull-wrong_number.fail"));
  assert(set_stack->Count() == 0);
  assert(!set_stack->Add(folder / "btfull-local.fail"));
  assert(set_stack->Count() == 0);
  assert(set_stack->Add(folder / "btfull1.success"));
  assert(set_stack->Count() == 1);
  for (auto it = set_stack->begin(); it != set_stack->end(); ++it)
  {
    assert((*it)->GetFilename() == folder / "btfull1.success");
    assert((*it)->NumberOfBacktraces() == 3);
    size_t i = 0;
    // Check range-based loop
    for (auto& it2 : **it)
    {
      switch (i)
      {
        case 0:
        {
          assert(it2.GetFile() == "../sysdeps/unix/sysv/linux/raise.c");
          assert(it2.GetName() == "__GI_raise");
          assert(it2.GetLine() == 51);
          assert(it2.GetIndex() == 0);
          break;
        }
        case 1:
        {
          assert(it2.GetFile() == "abort.c");
          assert(it2.GetName() == "__GI_abort");
          assert(it2.GetLine() == 90);
          assert(it2.GetIndex() == 1);
          break;
        }
        case 2:
        {
          assert(it2.GetFile() == "str_cmp.c");
          assert(it2.GetName() == "main");
          assert(it2.GetLine() == 23);
          assert(it2.GetIndex() == 2);
          break;
        }
        default:
        {
          assert(false);
          break;
        }
      }
      i++;
    }
  }
  assert(set_stack->Add(folder / "btfull2.success"));
  assert(set_stack->Count() == 1);
  assert(set_stack->Add(folder / "btfull3.success"));
  assert(set_stack->Count() == 2);
  assert(set_stack->Add(folder / "btfull4.success"));
  assert(set_stack->Count() == 3);
  assert(set_stack->Add(folder / "btfull5.success"));
  assert(set_stack->Count() == 4);
  assert(set_stack->Add(folder / "btfull6.success"));
  assert(set_stack->Count() == 5);
  assert(set_stack->Add(folder / "btfull7.success"));
  assert(set_stack->Count() == 6);
  assert(set_stack->Add(folder / "btfull8.success"));
  assert(set_stack->Count() == 7);
  assert(set_stack->Add(folder / "btfull9.success"));
  assert(set_stack->Count() == 7);
  assert(set_stack->Add(folder / "btfull1.success"));
  assert(set_stack->Count() == 7);

  set_stack =
      std::make_unique<llgc::software::gdb::SetStack>(false, 3, 1, true);
  assert(set_stack->Add(folder / "btfull5.success"));
  assert(set_stack->Count() == 1);
  assert(set_stack->Add(folder / "btfull10.success"));
  assert(set_stack->Count() == 2);
  assert(set_stack->Add(folder / "btfull11.success"));
  assert(set_stack->Count() == 3);
  assert(set_stack->Add(folder / "btfull8.success"));
  assert(set_stack->Count() == 4);
  assert(set_stack->Add(folder / "btfull12.success"));
  assert(set_stack->Count() == 4);
  assert(set_stack->Add(folder / "btfull13.success"));
  assert(set_stack->Count() == 5);
  assert(set_stack->Add(folder / "btfull14.success"));
  assert(set_stack->Count() == 6);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
