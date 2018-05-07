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

#include <2lgc/text/printf.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <sstream>
#include <string>

int main(int /* argc */, char* /* argv */ [])  // NS
{
  std::ostringstream oss;

  assert(llgc::text::Print::F(oss, "number: %, string: %, double: %", 2, "text",
                              1.5));
  assert(oss.str() == "number: 2, string: text, double: 1.5");
  oss.str("");

  assert(llgc::text::Print::F(oss, "number: %%, string: %.", "text"));
  assert(oss.str() == "number: %, string: text.");
  oss.str("");

  assert(llgc::text::Print::F(oss, "number: %, string: %%.", 124));
  assert(oss.str() == "number: 124, string: %.");
  oss.str("");

  assert(
      !llgc::text::Print::F(oss, "number: %, string: %, double: %", 2, "text"));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
