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

/** \file files.cc
 * Extend manipulation of filesystem.
 */

#include <2lgc/error/show.h>
#include <2lgc/filesystem/files.h>
#include <2lgc/text/string_ext.h>
#include <experimental/filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>

/** \namespace llgc::filesystem
 * \brief Ease manipulation of file and folder in filesystem.
 *
 *
 * \class llgc::filesystem::Files
 * \brief Class that manipulate the filesystem.
 */

/** \brief List all files from a folder.
 * \param[in] folder The root folder.
 * \param[in] regex The regex in ECMAScript regular expression grammar
 *       (javascript).
 * \param[out] files All the files. files is not clear if not empty.
 *       Can't be null.
 * \return true if no problem.
 */
bool llgc::filesystem::Files::SearchRecursiveFiles(
    const std::string &folder, const std::string &regex,
    std::vector<std::string> *files)
{
  try
  {
    // Can throw regex_error
    std::regex reg(regex);

    BUGUSER(std::cout, std::experimental::filesystem::is_directory(folder),
            false, "\"" + folder + "\" is not a folder.\n");

    std::vector<std::string> error_files;

    for (auto &p :
         std::experimental::filesystem::recursive_directory_iterator(folder))
    {
      try
      {
        // Can throw filesystem_error.
        std::string filename(p.path().filename().string());

        if ((regex.length() == 0 || std::regex_match(filename, reg)) &&
            std::experimental::filesystem::is_regular_file(p))
        {
          files->push_back(p.path().string());
        }
      }
      catch (const std::experimental::filesystem::filesystem_error &e)
      {
        error_files.emplace_back(e.what());
      }
    }

    BUGUSER(std::cout, error_files.empty(), false,
            "Failed to read folder \"" + folder + "\": " +
                llgc::text::StringExt::Join(error_files, ", ") + ".\n");

    return true;
  }
  catch (const std::regex_error &e)
  {
    BUGUSER(std::cout, false, false,
            "Invalid rexeg \"" + regex + "\": " + e.what() + "\n");
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
