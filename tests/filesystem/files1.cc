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
  assert(llgc::filesystem::Files::SearchRecursiveFiles("folder", "^.*\\.btfull",
                                                       &files));
  assert(files.size() == 1);
  assert(files[0] == "folder/output_1.btfull");
  // Test exclude regex
  files.clear();
  assert(llgc::filesystem::Files::SearchRecursiveFiles(
      "folder", "^(?!.*btfull).*$", &files));
  assert(files.size() == 1);
  assert(files[0] == "folder/2/output_1");

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
