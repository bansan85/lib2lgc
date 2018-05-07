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
#include <2lgc/text/string_ext.h>
#include <experimental/filesystem>
#include <map>
#include <regex>
#include <sstream>

bool llgc::filesystem::Files::SearchRecursiveFiles(
    const std::string& folder, const std::string& regex,
    std::vector<std::string>* files)
{
  try
  {
    std::regex reg(regex);

    BUGUSER(std::experimental::filesystem::is_directory(folder), false,
            "% is not a folder.\n", folder);

    std::vector<std::string> error_files;

    for (auto& p :
         std::experimental::filesystem::recursive_directory_iterator(folder))
    {
      try
      {
        std::string filename(p.path().filename().string());

        if ((regex.length() == 0 || std::regex_match(filename, reg)) &&
            std::experimental::filesystem::is_regular_file(p))
        {
          files->push_back(p.path().string());
        }
      }
      catch (const std::experimental::filesystem::filesystem_error& e)
      {
        error_files.emplace_back(e.what());
      }
    }

    BUGUSER(error_files.empty(), false, "Failed to read folder % : %.\n",
            folder, llgc::text::StringExt::Join(error_files, ", "));

    return true;
  }
  catch (const std::regex_error& e)
  {
    BUGUSER(false, false, "Invalid rexeg % : %\n", regex, e.what());
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
