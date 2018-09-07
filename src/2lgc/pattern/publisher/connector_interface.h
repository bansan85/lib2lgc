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

#ifndef PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_
#define PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <cstdint>
#include <memory>
#include <queue>
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
class ConnectorInterface
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  explicit ConnectorInterface(
      std::shared_ptr<SubscriberInterface<T>> subscriber);
#ifndef SWIG
  ConnectorInterface(ConnectorInterface&& other) = delete;
  ConnectorInterface(ConnectorInterface const& other) = delete;
  ConnectorInterface& operator=(ConnectorInterface&& other) = delete;
  ConnectorInterface& operator=(ConnectorInterface const& other) = delete;
#endif  // !SWIG
  virtual ~ConnectorInterface()
  {
    // typeid of Equals needs the class to be virtual.
    // is_polymorphic works only in destructor.
    static_assert(std::is_polymorphic<ConnectorInterface>::value,
                  "This should not be polymorphic.");
  }

  bool Equals(const ConnectorInterface<T>& connector) const CHK;
  virtual bool Send(const T& message) CHK = 0;
  bool Listen(const T& message, bool hold) CHK;
  bool ListenPending() CHK;
  virtual bool AddSubscriber(uint32_t id_message) CHK = 0;
  virtual bool RemoveSubscriber(uint32_t id_message) CHK = 0;
  const SubscriberInterface<T>* GetSubscriber() const
  {
    return subscriber_.get();
  }

 protected:
  std::shared_ptr<SubscriberInterface<T>> subscriber_;

 private:
  std::queue<T> messages_;
  uint32_t next_id_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/connector_interface.cc>
#endif

#endif  // PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
