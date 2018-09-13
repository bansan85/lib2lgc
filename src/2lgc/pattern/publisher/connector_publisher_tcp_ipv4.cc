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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_CC_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <iostream>
#include <thread>
#include <utility>

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

/** \class llgc::pattern::publisher::ConnectorPublisherTcpIpv4
 * \brief IPV4 of ConnectorPublisherTcp.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber The subscriber.
 * \param[in] ip The IP of the server.
 * \param[in] port The port of the server.
 */
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
    T>::ConnectorPublisherTcpIpv4(std::shared_ptr<SubscriberInterface<T>>
                                      subscriber,
                                  const std::string &ip, uint16_t port)
    : ConnectorPublisherTcp<T>(subscriber, ip, port)
{
}

/** \fn llgc::pattern::publisher::ConnectorPublisherTcpIpv4::~ConnectorPublisherTcpIpv4()
 * \brief Default virtual destructor.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorPublisherTcpIpv4::ConnectorPublisherTcpIpv4(ConnectorPublisherTcpIpv4 &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorPublisherTcpIpv4::ConnectorPublisherTcpIpv4(ConnectorPublisherTcpIpv4 const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorPublisherTcpIpv4 & llgc::pattern::publisher::ConnectorPublisherTcpIpv4::operator=(ConnectorPublisherTcpIpv4 &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorPublisherTcpIpv4 & llgc::pattern::publisher::ConnectorPublisherTcpIpv4::operator=(ConnectorPublisherTcpIpv4 const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcpIpv4::strategy_
 * \brief Function to execute. Different if no encryption or use OpenSSL.
 */

template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherTcpIpv4<T>::Connect()
{
  if (this->socket_ != -1)
  {
    return true;
  }

  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  BUGCRIT(std::cout, this->socket_ != -1, false,
          "Failed to run server. Errno " + std::to_string(errno) + ".\n");

  llgc::net::Linux::AutoCloseSocket auto_close_socket(&this->socket_);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sockaddr_in server;  // NOLINT(hicpp-member-init)
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  BUGUSER(std::cout,
          inet_pton(AF_INET, this->GetIp().c_str(),
                    reinterpret_cast<struct sockaddr_in *>(
                        static_cast<void *>(&server.sin_addr))) == 1,
          false, "Failed to get IP for name " + this->GetIp() + ".\n");
  server.sin_family = AF_INET;
  server.sin_port = htons(this->GetPort());

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  BUGCRIT(std::cout,
          connect(this->socket_, reinterpret_cast<struct sockaddr *>(&server),
                  sizeof(server)) == 0,
          false,
          "Failed to start listening. Errno " + std::to_string(errno) + ".\n");

  auto_close_socket.DontDeleteSocket();

#ifdef OPENSSL_FOUND
/*
  if (presentation_ != Presentation::NONE)
  {
    strategy_ = std::make_unique<StrategyPublisherTcpLinuxOpenSsl<T>>(
        this, client_sock, presentation_, cert_, key_);
  }
  else
*/
#endif  // OPENSSL_FOUND
  {
    this->strategy_ = std::make_unique<llgc::net::StrategyListenTcpLinux< T, llgc::pattern::publisher::ConnectorPublisherTcpIpv4<T>>>( this, this->socket_, [this](const T &messages) -> bool { return this->subscriber_->Listen(messages); });
  }

  std::thread t([this] { return this->strategy_->Do(); });
  this->receiver_ = std::move(t);

  return true;
}

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
