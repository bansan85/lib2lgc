/* Copyright 2018 LE GARREC Vincen2
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
#include <experimental/filesystem>
#include <regex>

bool Files::SearchRecursive(const std::string& folder, const std::string& regex,
                            std::vector<std::string>* files)
{
  std::regex reg(regex);
  for (auto& p :
       std::experimental::filesystem::recursive_directory_iterator(folder))
  {
    std::string filename(p.path().filename().string());

    if (regex.length() == 0 || std::regex_match(filename, reg))
    {
      files->push_back(p.path().string());
    }
  }

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
