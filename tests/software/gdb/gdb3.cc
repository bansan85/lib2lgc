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

#include <2lgc/software/gdb/set_stack.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <experimental/filesystem>
#include <memory>

int main(int argc, char* argv[])  // NS
{
  assert(argc == 3);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  std::experimental::filesystem::path folder(argv[1]);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  std::experimental::filesystem::path folder_bin(argv[2]);
  assert(std::experimental::filesystem::is_directory(folder));

  std::unique_ptr<llgc::software::gdb::SetStack> set_stack =
      std::make_unique<llgc::software::gdb::SetStack>(false, 3, 3, true);

  assert(set_stack->AddRecursive(folder, 32, "^.*\\.success$"));
  assert(!set_stack->AddRecursive("not_found", 32, "^.*\\.success$"));
  assert(!set_stack->AddList("not_found", 32));
  assert(set_stack->AddList(folder_bin / "list.bt", 32));
  set_stack->Print();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
