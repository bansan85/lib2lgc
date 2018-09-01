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

#ifndef PATTERN_PUBLISHER_PUBLISHER_ALL_H_
#define PATTERN_PUBLISHER_PUBLISHER_ALL_H_

#include <functional>
#include <thread>

namespace llgc::pattern::publisher::test
{
class Publisher
{
 public:
  template <typename T, typename U, typename V>
  static void All(U* subscriber, V* server, size_t delay);

  static void WaitUpToTenSecond(const std::function<bool()>& test);
};
}

#endif  // PATTERN_PUBLISHER_PUBLISHER_ALL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
