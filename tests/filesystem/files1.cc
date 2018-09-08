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
#include <2lgc/filesystem/files.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <experimental/filesystem>
#include <ext/alloc_traits.h>
#include <fstream>
#include <string>
#include <vector>

int main(int /* argc */, char* /* argv */ [])  // NS
{
  std::experimental::filesystem::create_directory("folder");
  std::experimental::filesystem::create_directory("folder/2");

  std::fstream output;
  output.open("folder/2/output_1", std::ios::out);
  output.close();
  output.open("folder/output_1.btfull", std::ios::out);
  output.close();

  std::vector<std::string> files;
  // Test standard regex
  EXECUTE_AND_ABORT(std::cout, llgc::filesystem::Files::SearchRecursiveFiles(
                                   "folder", "^.*\\.btfull", &files));
  assert(files.size() == 1);
  assert(files[0] == "folder/output_1.btfull");
  // Test exclude regex
  files.clear();
  EXECUTE_AND_ABORT(std::cout, llgc::filesystem::Files::SearchRecursiveFiles(
                                   "folder", "^(?!.*btfull).*$", &files));
  assert(files.size() == 1);
  assert(files[0] == "folder/2/output_1");

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
