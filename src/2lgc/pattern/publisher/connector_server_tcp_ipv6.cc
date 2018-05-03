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

#include <2lgc/pattern/publisher/connector_server_tcp.h>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv6.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

template <typename T>
llgc::pattern::publisher::ConnectorServerTcpIpv6<T>::ConnectorServerTcpIpv6(
    std::shared_ptr<SubscriberInterface<T>> subscriber, const std::string ip,
    uint16_t port)
    : ConnectorServerTcp<T>(subscriber, ip, port)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorServerTcpIpv6<T>::~ConnectorServerTcpIpv6() =
    default;

template <typename T>
int llgc::pattern::publisher::ConnectorServerTcpIpv6<T>::SizeOfSocket()  // NS
{
  return sizeof(struct sockaddr_in6);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
