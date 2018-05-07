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

/**
 * @brief Namespace that have all function that override classic classes.
 */
namespace llgc::text
{
/**
 * @brief This class extend std::string.
 */
class StringExt
{
 public:
  // Stolen from
  // https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch04s09.html#cplusplusckbk-CHP-4-EX-13

  static std::string Join(const std::vector<std::string>& v,
                          const std::string& delimiter);
};

}  // namespace llgc::text

#endif  // TEXT_STRING_EXT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
