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

#include <2lgc/filesystem/files.h>
#include <experimental/filesystem>
#include <map>
#include <regex>
#include <sstream>

bool llgc::filesystem::Files::SearchRecursiveFiles(
    const std::string& folder, const std::string& regex,
    std::vector<std::string>* files)
{
  std::regex reg(regex);

  if (!std::experimental::filesystem::is_directory(folder))
  {
    return false;
  }

  for (auto& p :
       std::experimental::filesystem::recursive_directory_iterator(folder))
  {
    std::string filename(p.path().filename().string());

    if ((regex.length() == 0 || std::regex_match(filename, reg)) &&
        std::experimental::filesystem::is_regular_file(p))
    {
      files->push_back(p.path().string());
    }
  }

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
