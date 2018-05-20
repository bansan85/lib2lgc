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

#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
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
llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
    T>::ConnectorPublisherTcpIpv4(std::shared_ptr<SubscriberInterface<T>>
                                      subscriber,
                                  const std::string ip, uint16_t port)
    : ConnectorPublisherTcp<T>(subscriber, ip, port)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
    T>::~ConnectorPublisherTcpIpv4() = default;

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcpIpv4<T>::Connect()
{
  if (this->socket_ != -1)
  {
    return true;
  }

  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  BUGCRIT(this->socket_ != -1, false, "Failed to run server. Errno %.\n",
          errno);

  llgc::net::Linux::AutoCloseSocket auto_close_socket(&this->socket_);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sockaddr_in server;  // NOLINT(hicpp-member-init)
                              // Ugly hack to prevent strict aliasing warning.
  BUGUSER(
      inet_pton(AF_INET, this->ip_.c_str(),
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                reinterpret_cast<struct sockaddr_in *>(
                    static_cast<void *>(&server.sin_addr))) == 1,
      false, "Failed to get IP for name %.\n", this->ip_);
  server.sin_family = AF_INET;
  server.sin_port = htons(this->port_);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  BUGCRIT(connect(this->socket_, reinterpret_cast<struct sockaddr *>(&server),
                  sizeof(server)) == 0,
          false, "Failed to start listening. Errno %.\n", errno);

  auto_close_socket.DontDeleteSocket();

  std::thread t(&ConnectorPublisherTcp<T>::Receiver, this);
  this->receiver_ = std::move(t);

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
