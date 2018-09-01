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

#ifndef SOFTWARE_GDB_GDB_H_
#define SOFTWARE_GDB_GDB_H_

#include <2lgc/compat.h>
#include <cstdint>
#include <string>

namespace llgc::software::gdb
{
class GdbServer;

/**
 * @brief Class to run gdb for various purpose.
 */
class Gdb
{
 public:
  static bool RunBtFull(const std::string& filename, unsigned int argc,
                        const char* const argv[], int64_t timeout) CHK;
  static bool RunBtFullRecursive(const std::string& folder,
                                 unsigned int nthread, const std::string& regex,
                                 unsigned int argc, const char* const argv[],
                                 int64_t timeout) CHK;
  static bool RunBtFullList(const std::string& list, unsigned int nthread,
                            unsigned int argc, const char* const argv[],
                            int64_t timeout) CHK;

  static GdbServer server_;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_GDB_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
