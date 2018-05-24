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

#ifndef PATTERN_ABSTRACT_FACTORY_H_
#define PATTERN_ABSTRACT_FACTORY_H_

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Google Protobuf stuff.
 */
namespace google::protobuf
{
class Message;
}

/**
 * @brief Namespace for patterns.
 */
namespace llgc::pattern
{
/**
 * @brief Interface that define an abstract factory.
 *
 * @tparam T The class message from protobuf class.
 * @tparam U The command interface.
 */
template <typename T, typename U>
class AbstractFactory
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,  // NS
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  /**
   * @brief Default constructor. The class that implement this class must fill
   * the map_factory member on constructor.
   *
   * @param[in] size Size of the map factory.
   */
  explicit AbstractFactory(size_t size) : map_factory_(size) {}

  /**
   * @brief Default destructor. Virtual because command is abstract.
   */
  virtual ~AbstractFactory() = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  AbstractFactory(AbstractFactory&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  AbstractFactory(AbstractFactory const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  AbstractFactory& operator=(AbstractFactory&& other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  AbstractFactory& operator=(AbstractFactory const& other) & = delete;
#endif  // !SWIG

  /**
   * @brief Create the command
   *
   * @param[in] message The message in protobuf serialization format.
   *
   * @return The instance is success, nullptr instead.
   */
  std::unique_ptr<U> Create(const std::string& message);

 protected:
  /**
   * @brief Map of function to create an instance of T.
   */
  std::vector<std::function<std::unique_ptr<U>(const T&)>> map_factory_;
};

}  // namespace llgc::pattern

#endif  // PATTERN_ABSTRACT_FACTORY_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
