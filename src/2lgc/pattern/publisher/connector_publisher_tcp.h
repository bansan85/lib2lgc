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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_H_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/connector_interface.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
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
class ConnectorPublisherTcp : public ConnectorInterface<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  ConnectorPublisherTcp(std::shared_ptr<SubscriberInterface<T>> subscriber,
                        std::string ip, uint16_t port);
  ~ConnectorPublisherTcp() override;

  bool Send(const T &message) override CHK;
  bool AddSubscriber(uint32_t id_message) override CHK;
  bool RemoveSubscriber(uint32_t id_message) override CHK;

  void Receiver();

#ifndef SWIG
  ConnectorPublisherTcp(ConnectorPublisherTcp &&other) = delete;
  ConnectorPublisherTcp(ConnectorPublisherTcp const &other) = delete;
  ConnectorPublisherTcp &operator=(ConnectorPublisherTcp &&other) = delete;
  ConnectorPublisherTcp &operator=(ConnectorPublisherTcp const &other) = delete;
#endif  // !SWIG

 protected:
  std::thread receiver_;
  int socket_;
  virtual bool Connect() CHK = 0;
  const std::string &GetIp() const { return ip_; }
  uint16_t GetPort() const { return port_; }

 private:
  const std::string ip_;
  uint16_t port_;
  std::atomic<bool> disposing_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/connector_publisher_tcp.cc>
#endif

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
