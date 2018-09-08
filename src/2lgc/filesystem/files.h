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

/** \file files.h
 * Extend manipulation of filesystem.
 */

#ifndef FILESYSTEM_FILES_H_
#define FILESYSTEM_FILES_H_

#include <2lgc/compat.h>
#include <string>
#include <vector>

namespace llgc::filesystem
{
class Files
{
 public:
  static bool SearchRecursiveFiles(const std::string &folder,
                                   const std::string &regex,
                                   std::vector<std::string> *files) CHK;
};

}  // namespace llgc::filesystem

#endif  // FILESYSTEM_FILES_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
