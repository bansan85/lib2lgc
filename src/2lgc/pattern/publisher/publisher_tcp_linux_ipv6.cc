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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_IPV6_CC_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_IPV6_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv6.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <iostream>
#include <string>

/** \class llgc::pattern::publisher::PublisherTcpLinuxIpv6
 * \brief Interface to create a TCP server.
 */

/** \brief Constructor with port for the TCP server.
 * \param[in] port The port to listen from.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::PublisherTcpLinuxIpv6<T>::PublisherTcpLinuxIpv6(
    uint16_t port)
    : PublisherTcpLinux<T>(port)
{
}

template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::PublisherTcpLinuxIpv6<T>::Listen()
{
  this->sockfd_ = socket(AF_INET6, SOCK_STREAM, 0);

  BUGCRIT(std::cout, this->sockfd_ != -1, false,
          "Failed to open socket. Errno " + std::to_string(errno) + "\n");

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sockaddr_in6 socket_addr;  // NOLINT(hicpp-member-init)
  socket_addr.sin6_family = AF_INET6;
  socket_addr.sin6_addr = in6addr_any;
  socket_addr.sin6_port = htons(this->GetPort());

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  BUGCONT(std::cout,
          llgc::net::Linux::Bind(
              this->sockfd_, reinterpret_cast<struct sockaddr *>(&socket_addr),
              sizeof(socket_addr), 4 * 60 * 1000),
          false);

  BUGCRIT(std::cout, listen(this->sockfd_, 3) != -1, false,
          "Failed to listen socket. Errno " + std::to_string(errno) + "\n");

  return true;
}

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_IPV6_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
