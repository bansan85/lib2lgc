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

#ifndef PATTERN_PUBLISHER_SUBSCRIBER_INTERFACE_H_
#define PATTERN_PUBLISHER_SUBSCRIBER_INTERFACE_H_

#include <2lgc/compatibility/visual_studio.h>
#include <cstdint>
#include <memory>
#include <string>

/**
 * @brief Google Protobuf stuff.
 */
namespace google::protobuf
{
class Message;
}

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class SubscriberInterface
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,  // NS
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  /**
   * @brief Send message.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   *
   * @return true if no problem.
   */
  virtual bool Listen(const T& message) CHK = 0;

  /**
   * @brief Compare in connector is the same than the object.
   *
   * @param[in] connector The connector to compare with.
   *
   * @return true if the same.
   */
  virtual bool Equals(const SubscriberInterface<T>& connector) const = 0;

  /**
   * @brief Default virtual destructor.
   */
  virtual ~SubscriberInterface() {}
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
