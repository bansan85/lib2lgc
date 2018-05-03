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
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <utility>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

template <typename T>
llgc::pattern::publisher::ConnectorServerTcpIpv4<T>::ConnectorServerTcpIpv4(
    std::shared_ptr<SubscriberInterface<T>> subscriber, const std::string ip,
    uint16_t port)
    : ConnectorServerTcp<T>(subscriber, ip, port)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorServerTcpIpv4<T>::~ConnectorServerTcpIpv4() =
    default;

template <typename T>
bool llgc::pattern::publisher::ConnectorServerTcpIpv4<T>::Connect()
{
  if (this->socket_ != -1)
  {
    return true;
  }

  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socket_ == -1)
  {
    return false;
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sockaddr_in server;  // NOLINT(hicpp-member-init)
  server.sin_addr.s_addr = inet_addr(this->ip_.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(this->port_);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (connect(this->socket_, reinterpret_cast<struct sockaddr *>(&server),
              sizeof(server)) < 0)
  {
    close(this->socket_);
    this->socket_ = -1;
    return false;
  }

  std::thread t(&ConnectorServerTcp<T>::Receiver, this);
  this->receiver_ = std::move(t);

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
