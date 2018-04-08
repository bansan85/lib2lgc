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
#include <2lgc/software/gdb/gdb.h>
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <experimental/filesystem>
#include <memory>
#include <string>

void CheckBtfull(const std::experimental::filesystem::path& path);

void CheckBtfull(const std::experimental::filesystem::path& path)
{
  assert(std::experimental::filesystem::is_regular_file(path));

  std::unique_ptr<llgc::software::gdb::SetStack> set_stack =
      std::make_unique<llgc::software::gdb::SetStack>(false, 3, 0, false);
  assert(set_stack->Add(path));
  assert(set_stack->Count() == 1);
  const llgc::software::gdb::Stack& stack = set_stack->Get(0);
  assert(stack.NumberOfBacktraces() == 1);
  const llgc::software::gdb::Backtrace& backtrace =
      stack.GetBacktraceFromTop(0);
  assert(backtrace.GetName() == "main");
  assert(backtrace.GetIndex() == 0);
  // Can't add more test if deadbeef is compiled without -g.
}

int main(int argc, char* argv[])  // NS
{
  assert(argc == 2);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  std::experimental::filesystem::path folder_bin(argv[1]);
  assert(std::experimental::filesystem::is_directory(folder_bin));
  std::experimental::filesystem::path deadbeef_path(folder_bin / "deadbeef");
  assert(std::experimental::filesystem::is_regular_file(deadbeef_path));
  std::experimental::filesystem::path deadbeef_btfull_path(deadbeef_path);
  deadbeef_btfull_path += ".btfull";

  std::string deadbeef_file(deadbeef_path);
  const char* const argv_gdb[1] = {deadbeef_file.c_str()};
  std::error_code err;
  if (!std::experimental::filesystem::remove(deadbeef_btfull_path, err))
  {
    std::cout << "filesystem::remove: " << err << std::endl;
  }
  assert(llgc::software::gdb::Gdb::RunBtFull(deadbeef_file, 1, argv_gdb, 30));
  CheckBtfull(deadbeef_btfull_path);

  assert(std::experimental::filesystem::remove(deadbeef_btfull_path));
  assert(!llgc::software::gdb::Gdb::RunBtFull(deadbeef_file, 1, argv_gdb, 0));

  assert(!llgc::software::gdb::Gdb::RunBtFullList("not_found", 32, 1, argv_gdb,
                                                  30));

  std::experimental::filesystem::path deadbeef_list(folder_bin /
                                                    "deadbeef.list");
  assert(llgc::software::gdb::Gdb::RunBtFullList(deadbeef_list, 32, 1, argv_gdb,
                                                 30));
  CheckBtfull(deadbeef_btfull_path);

  assert(!llgc::software::gdb::Gdb::RunBtFullRecursive(
      "not_found", 32, "deadbeef", 1, argv_gdb, 30));
  assert(llgc::software::gdb::Gdb::RunBtFullRecursive(
      folder_bin, 32, "deadbeef", 1, argv_gdb, 30));
  CheckBtfull(deadbeef_btfull_path);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
