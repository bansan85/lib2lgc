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

#ifndef PATTERN_PUBLISHER_SUBSCRIBER_LOCAL_H_
#define PATTERN_PUBLISHER_SUBSCRIBER_LOCAL_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/subscriber_interface.h>
#include <cstdint>
#include <memory>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

template <typename T>
class SubscriberLocal : public SubscriberInterface<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  explicit SubscriberLocal(uint32_t id) : id_(id) {}
#ifndef SWIG
  SubscriberLocal(SubscriberLocal&& other) = delete;
  SubscriberLocal(SubscriberLocal const& other) = delete;
  SubscriberLocal& operator=(SubscriberLocal&& other) = delete;
  SubscriberLocal& operator=(SubscriberLocal const& other) = delete;
#endif  // !SWIG
  ~SubscriberLocal() override = default;

  bool Equals(const SubscriberInterface<T>& connector) const override;

  bool SetConnector(std::shared_ptr<ConnectorInterface<T>> conn) CHK;
  bool Send(const T& message) CHK;
  bool AddSubscriber(uint32_t id_message) CHK;
  bool RemoveSubscriber(uint32_t id_message) CHK;

 private:
  const uint32_t id_;
  std::weak_ptr<ConnectorInterface<T>> connector_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/subscriber_local.cc>
#endif

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_LOCAL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
