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

#ifndef TEXT_STRING_EXT_H_
#define TEXT_STRING_EXT_H_

#include <string>
#include <vector>

namespace llgc::text
{
class StringExt
{
 public:
  static std::string Join(const std::vector<std::string> &v,
                          const std::string &delimiter);
};

}  // namespace llgc::text

#endif  // TEXT_STRING_EXT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
