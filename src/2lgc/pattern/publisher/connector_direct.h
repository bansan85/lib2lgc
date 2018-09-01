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

#ifndef PATTERN_PUBLISHER_CONNECTOR_DIRECT_H_
#define PATTERN_PUBLISHER_CONNECTOR_DIRECT_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/connector_interface.h>
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
class SubscriberInterface;

template <typename T>
class PublisherDirect;

template <typename T>
class ConnectorDirect : public ConnectorInterface<T>,
                        public std::enable_shared_from_this<ConnectorDirect<T>>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  ConnectorDirect(std::shared_ptr<SubscriberInterface<T>> subscriber,
                  std::shared_ptr<PublisherDirect<T>> server);
#ifndef SWIG
  ConnectorDirect(ConnectorDirect &&other) = delete;
  ConnectorDirect(ConnectorDirect const &other) = delete;
  ConnectorDirect &operator=(ConnectorDirect &&other) = delete;
  ConnectorDirect &operator=(ConnectorDirect const &other) = delete;
#endif  // !SWIG
  ~ConnectorDirect() override = default;

  bool Send(const T &message) override CHK;
  bool AddSubscriber(uint32_t id_message) override CHK;
  bool RemoveSubscriber(uint32_t id_message) override CHK;

 private:
  std::shared_ptr<PublisherDirect<T>> server_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_DIRECT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
