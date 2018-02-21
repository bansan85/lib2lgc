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

#include "bt.h"

// C++ system
#include <cassert>
#include <regex>
#include <iostream>

std::unique_ptr<Bt> Bt::Factory(const std::string_view &line)
{
  std::unique_ptr<Bt> bt = std::make_unique<Bt>();

  std::string line_it(line);
  std::regex rex("^#(\\d+) *((0x.*) in )?((.*\\)) at )?(.*)$");
  std::smatch sm;

  if (regex_search(line_it, sm, rex))
  {
    // #2  0x00007ffff7a854d5 in __gnu_cxx::__verbose_terminate_handler () at /home/legarrec/info/portage/sys-devel/gcc-7.3.0/work/gcc-7.3.0/libstdc++-v3/libsupc++/vterminate.cc:95
    //
    // [0] : "#2  0x00007ffff7a854d5 in __gnu_cxx::__verbose_terminate_handler () at /home/legarrec/info/portage/sys-devel/gcc-7.3.0/work/gcc-7.3.0/libstdc++-v3/libsupc++/vterminate.cc:95"
    // [1] : "2"
    // [2] : "0x00007ffff7a854d5 in "
    // [3] : "0x00007ffff7a854d5"
    // [4] : "__gnu_cxx::__verbose_terminate_handler () at "
    // [5] : "__gnu_cxx::__verbose_terminate_handler ()"
    // [6] : "/home/legarrec/info/portage/sys-devel/gcc-7.3.0/work/gcc-7.3.0/libstdc++-v3/libsupc++/vterminate.cc:95"
    assert(sm.length() >= 7);
    assert(sm[1].length() != 0);

    bt->ReadIndex(sm[1]);
    std::cout << bt->index << std::endl;

    if (sm[3].length() != 0)
    {
      bt->ReadAddress(sm[3]);
      std::cout << bt->address << std::endl;
    }

    if (sm[5].length() != 0)
    {
      bt->ReadFunction(sm[5]);
      std::cout << bt->function.name << std::endl;
    }

    return bt;
  }
  else
  {
    return nullptr;
  }
}

Bt::Bt() : index(0), address(0)
{
}

void Bt::ReadIndex(const std::string& number)
{
  index = std::stoi(number, nullptr, 10);
}

void Bt::ReadAddress(const std::string& number)
{
  address = std::stol(number, nullptr, 0);
}

bool Bt::ReadFunction(const std::string& description)
{
  size_t pos_space = description.find(' ');

  if (pos_space == std::string::npos)
  {
    return false;
  }

  function.name = description.substr(0, pos_space);

  // + 2 to remove " ("
  std::string_view str(&description.c_str()[pos_space + 2],
  // -3 to remove " (" and ")"
                       description.length()-pos_space - 3);

  std::cout << "ARG : " << str << std::endl;

  return true;
}
