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

#include <2lgc/text/string_ext.h>
#include <string>
#include <vector>

/** \namespace llgc::text
 * \brief Namespace that have all function that override classic classes.
 *
 *
 * \class llgc::text::StringExt
 * \brief This class extend std::string.
 */

/** \brief Join function for std::vector<std::string>.
 * \param[in] v The vector to join.
 * \param[in] delimiter The delimeter.
 * \return The return value.
 */
std::string llgc::text::StringExt::Join(const std::vector<std::string>& v,
                                        const std::string& delimiter)
{
  std::string retval;
  for (auto p = v.begin(); p != v.end(); ++p)
  {
    retval.append(*p);
    if (p != v.end() - 1)
    {
      retval.append(delimiter);
    }
  }
  return retval;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
