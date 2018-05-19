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

#include <2lgc/pattern/abstract_factory.h>

#include <iostream>

template <typename T, typename U>
std::unique_ptr<U> llgc::pattern::AbstractFactory<T, U>::Create(
    const std::string& message)
{
  T message_t;

  if (!message_t.ParseFromString(message))
  {
    return nullptr;
  }
  if (static_cast<size_t>(message_t.data_case()) >= map_factory_.size())
  {
    return nullptr;
  }
  if (map_factory_[message_t.data_case()] == nullptr)
  {
    return nullptr;
  }

  std::cout << "Create " << message_t.data_case() << std::endl;
  return std::move(map_factory_[message_t.data_case()](message_t));
}
