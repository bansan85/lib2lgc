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
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <experimental/filesystem>
#include <memory>
#include <string>

int main(int argc, char* argv[])  // NS
{
  assert(argc == 2);

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
  for (const auto& it : *set_stack)
  {
    assert(it.GetFilename() == folder / "btfull1.success");
    assert(it.NumberOfBacktraces() == 3);
    size_t i = 0;
    // Check range-based loop
    for (const auto& it2 : it)
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
