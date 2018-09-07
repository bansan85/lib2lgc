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

/// /file abstract_factory.h

#ifndef PATTERN_ABSTRACT_FACTORY_H_
#define PATTERN_ABSTRACT_FACTORY_H_

// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern
{
template <typename T, typename U>
class AbstractFactory
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must inherit from ::google::protobuf::Message.");

 public:
  explicit AbstractFactory(size_t size) : map_factory_(size) {}

#ifndef SWIG
  AbstractFactory(AbstractFactory&& other) = delete;
  AbstractFactory(AbstractFactory const& other) = delete;
  AbstractFactory& operator=(AbstractFactory&& other) = delete;
  AbstractFactory& operator=(AbstractFactory const& other) = delete;
#endif  // !SWIG

  virtual ~AbstractFactory() = default;

  std::unique_ptr<U> Create(const std::string& message);

 protected:
  std::vector<std::function<std::unique_ptr<U>(const T&)>> map_factory_;
};

}  // namespace llgc::pattern

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/abstract_factory.cc>
#endif

#endif  // PATTERN_ABSTRACT_FACTORY_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
