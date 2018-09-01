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

#include <2lgc/compat.h>
#include <cstdint>
#include <memory>
#include <string>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

template <typename T>
class SubscriberInterface
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  SubscriberInterface() = default;
#ifndef SWIG
  SubscriberInterface(SubscriberInterface &&other) = delete;
  SubscriberInterface(SubscriberInterface const &other) = delete;
  SubscriberInterface &operator=(SubscriberInterface &&other) = delete;
  SubscriberInterface &operator=(SubscriberInterface const &other) = delete;
#endif  // !SWIG
  virtual ~SubscriberInterface() = default;
  virtual bool Listen(const T &messages) CHK = 0;
  virtual bool Equals(const SubscriberInterface<T> &connector) const = 0;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
