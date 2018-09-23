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
#include <2lgc/config.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/net/openssl.h>
// std::make_unique needs it.
#include <2lgc/net/strategy_connect_open_ssl.h>  // IWYU pragma: keep
// std::make_unique needs it.
#include <2lgc/net/strategy_listen_tcp_linux.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
// IWYU wants to remove it. But without you can't define method.
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h>  // IWYU pragma: keep
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

namespace llgc
{
namespace pattern::publisher
{
template <typename T>
class SubscriberInterface;
}  // namespace pattern::publisher
}  // namespace llgc

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
  if (this->presentation_ != llgc::net::OpenSsl::Presentation::NONE)
  {
    BUGCONT(std::cout,
            llgc::net::OpenSsl::InitCtxSslClient(this->presentation_,
                                                 &this->ctx_, &this->ssl_),
            false);
    std::thread t([this] {
      auto strategy = std::make_unique<llgc::net::StrategyConnectOpenSsl<T>>(
          this, &this->socket_, this->presentation_, this->ctx_, this->ssl_);
      return strategy->Do();
    });
    this->receiver_ = std::move(t);
  }
  else
#endif  // OPENSSL_FOUND
  {
    std::thread t([this] {
      auto strategy = std::make_unique<llgc::net::StrategyListenTcpLinux<
          T, llgc::pattern::publisher::ConnectorPublisherTcpIpv4<T>>>(
          this, &this->socket_, [this](const T &messages) -> bool {
            return this->subscriber_->Listen(messages);
          });
      return strategy->Do();
    });
    this->receiver_ = std::move(t);
  }

  return true;
}

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
