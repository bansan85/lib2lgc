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

/**
 * @file compare_decimal.cc
 * @brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#include <2lgc/error/show.h>
#include <2lgc/override/printf.h>
#include <cstddef>

bool llgc::override::Print::F(std::ostream &out_stream, const std::string &s)
{
  size_t i = 0;
  while (s[i] != 0)
  {
    if (s[i] == '%')
    {
      BUGCRIT(s[i + 1] == '%', false, "Missing argument to convert %%.\n");
      ++i;
    }
    out_stream << s[i];
    i++;
  }
  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
