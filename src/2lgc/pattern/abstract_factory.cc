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

/// \file abstract_factory.cc

#include <2lgc/pattern/abstract_factory.h>

/** \namespace llgc::pattern
 * \brief Namespace for patterns.
 *
 * \class llgc::pattern::AbstractFactory
 * \brief Interface that define an abstract factory.
 * \tparam T The class message that inherit from protobuf class.
 * \tparam U The command interface.
 */

/** \fn llgc::pattern::AbstractFactory::AbstractFactory(size_t size)
 * \brief Default constructor. The class that implement this class must fill the
 *        map_factory member on constructor.
 * \param[in] size Size of the map factory.
 *
 *
 * \fn llgc::pattern::AbstractFactory::AbstractFactory(AbstractFactory&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::AbstractFactory::AbstractFactory(AbstractFactory const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn AbstractFactory& llgc::pattern::AbstractFactory::operator=(AbstractFactory&& other);
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn AbstractFactory& llgc::pattern::AbstractFactory::operator=(AbstractFactory const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn llgc::pattern::AbstractFactory::~AbstractFactory()
 * \brief Default destructor. Virtual because command is abstract.
 */

/** \brief Create the command.
 * \param[in] message The message in protobuf serialization format.
 * \return The instance is success, nullptr instead.
 */
template <typename T, typename U>
std::unique_ptr<U> llgc::pattern::AbstractFactory<T, U>::Create(
    const std::string& message)
{
  T message_t;

  if (!message_t.ParseFromString(message) ||
      static_cast<size_t>(message_t.data_case()) >= map_factory_.size() ||
      map_factory_[message_t.data_case()] == nullptr)
  {
    return nullptr;
  }

  return std::move(map_factory_[message_t.data_case()](message_t));
}

/** \var llgc::pattern::AbstractFactory::map_factory_
 * \brief Map of functions to create an instance of T. The index used to create
 *        the object is the return of the `data_case()` method from the
 *        protobuf message.
 */
