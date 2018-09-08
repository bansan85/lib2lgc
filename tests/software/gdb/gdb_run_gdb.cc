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

#include <2lgc/error/show.h>
#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/gdb.h>
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <experimental/filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <system_error>

void CheckBtfull(const std::experimental::filesystem::path &path);

void CheckBtfull(const std::experimental::filesystem::path &path)
{
  assert(std::experimental::filesystem::is_regular_file(path));

  std::unique_ptr<llgc::software::gdb::SetStack> set_stack =
      std::make_unique<llgc::software::gdb::SetStack>(false, 3, 0, false);
  EXECUTE_AND_ABORT(std::cout, set_stack->Add(path));
  assert(set_stack->Count() == 1);
  const llgc::software::gdb::Stack &stack = set_stack->Get(0);
  assert(stack.NumberOfBacktraces() == 1);
  const llgc::software::gdb::Backtrace &backtrace =
      stack.GetBacktraceFromTop(0);
  assert(backtrace.GetName() == "main");
  assert(backtrace.GetIndex() == 0);
  // Can't add more test if deadbeef is compiled without -g.
}

int main(int argc, char *argv[])  // NS
{
  assert(argc == 2);

  std::experimental::filesystem::path folder_bin(argv[1]);
  assert(std::experimental::filesystem::is_directory(folder_bin));
  std::experimental::filesystem::path deadbeef_path(folder_bin / "deadbeef");
  assert(std::experimental::filesystem::is_regular_file(deadbeef_path));
  std::experimental::filesystem::path deadbeef_btfull_path(deadbeef_path);
  deadbeef_btfull_path += ".btfull";

  std::string deadbeef_file(deadbeef_path);
  const char *const argv_gdb[1] = {deadbeef_file.c_str()};
  std::error_code err;
  if (!std::experimental::filesystem::remove(deadbeef_btfull_path, err))
  {
    std::cout << "filesystem::remove: " << err << std::endl;
  }
  EXECUTE_AND_ABORT(std::cout, llgc::software::gdb::Gdb::RunBtFull(
                                   deadbeef_file, 1, argv_gdb, 30));
  CheckBtfull(deadbeef_btfull_path);

  EXECUTE_AND_ABORT(
      std::cout, std::experimental::filesystem::remove(deadbeef_btfull_path));
  EXECUTE_AND_ABORT(std::cout, !llgc::software::gdb::Gdb::RunBtFull(
                                   deadbeef_file, 1, argv_gdb, 0));

  EXECUTE_AND_ABORT(std::cout, !llgc::software::gdb::Gdb::RunBtFullList(
                                   "not_found", 32, 1, argv_gdb, 30));

  std::experimental::filesystem::path deadbeef_list(folder_bin /
                                                    "deadbeef.list");
  EXECUTE_AND_ABORT(std::cout, llgc::software::gdb::Gdb::RunBtFullList(
                                   deadbeef_list, 32, 1, argv_gdb, 30));
  CheckBtfull(deadbeef_btfull_path);

  EXECUTE_AND_ABORT(std::cout,
                    !llgc::software::gdb::Gdb::RunBtFullRecursive(
                        "not_found", 32, "deadbeef", 1, argv_gdb, 30));
  EXECUTE_AND_ABORT(std::cout,
                    llgc::software::gdb::Gdb::RunBtFullRecursive(
                        folder_bin, 32, "deadbeef", 1, argv_gdb, 30));
  CheckBtfull(deadbeef_btfull_path);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
