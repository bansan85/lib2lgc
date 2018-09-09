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
#include <2lgc/software/gdb/set_stack.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <experimental/filesystem>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])  // NS
{
  assert(argc == 3);

  std::experimental::filesystem::path folder(argv[1]);
  std::experimental::filesystem::path folder_bin(argv[2]);
  assert(std::experimental::filesystem::is_directory(folder));

  std::unique_ptr<llgc::software::gdb::SetStack> set_stack =
      std::make_unique<llgc::software::gdb::SetStack>(false, 3, 3, true);

  EXECUTE_AND_ABORT(std::cout,
                    set_stack->AddRecursive(folder, 32, "^.*\\.success$"));
  EXECUTE_AND_ABORT(
      std::cout, !set_stack->AddRecursive("not_found", 32, "^.*\\.success$"));
  EXECUTE_AND_ABORT(std::cout, !set_stack->AddList("not_found", 32));
  EXECUTE_AND_ABORT(std::cout, set_stack->AddList(folder_bin / "list.bt", 32));
  set_stack->Print();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
