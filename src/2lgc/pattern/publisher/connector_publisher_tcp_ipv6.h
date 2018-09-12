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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV6_H_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV6_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

template <typename T, typename U>
class StrategyPublisherTcpLinuxTcp;

template <typename T>
class ConnectorPublisherTcpIpv6 : public ConnectorPublisherTcp<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  ConnectorPublisherTcpIpv6(std::shared_ptr<SubscriberInterface<T>> subscriber,
                            const std::string &ip, uint16_t port);
  ~ConnectorPublisherTcpIpv6() override = default;

#ifndef SWIG
  ConnectorPublisherTcpIpv6(ConnectorPublisherTcpIpv6 &&other) = delete;
  ConnectorPublisherTcpIpv6(ConnectorPublisherTcpIpv6 const &other) = delete;
  ConnectorPublisherTcpIpv6 &operator=(ConnectorPublisherTcpIpv6 &&other) =
      delete;
  ConnectorPublisherTcpIpv6 &operator=(ConnectorPublisherTcpIpv6 const &other) =
      delete;
#endif  // !SWIG

 private:
  std::unique_ptr<llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp<
      T, llgc::pattern::publisher::ConnectorPublisherTcpIpv6<T>>>
      strategy_;
  bool Connect() override CHK;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv6.cc>
#endif

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV6_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
