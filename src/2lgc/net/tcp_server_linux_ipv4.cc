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

#include <2lgc/net/linux.h>
#include <2lgc/net/tcp_server_linux.h>
#include <2lgc/net/tcp_server_linux_ipv4.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <iostream>

template <typename T>
llgc::net::TcpServerLinuxIpv4<T>::TcpServerLinuxIpv4(uint16_t port)
    : TcpServerLinux<T>(port)
{
}

template <typename T>
bool llgc::net::TcpServerLinuxIpv4<T>::Listen()
{
  this->sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (this->sockfd_ == -1)
  {
    return false;
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sockaddr_in socket_addr;  // NOLINT(hicpp-member-init)
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_addr.s_addr = INADDR_ANY;
  socket_addr.sin_port = htons(this->port_);

  if (!llgc::net::Linux::Bind(
          this->sockfd_,
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          reinterpret_cast<struct sockaddr *>(&socket_addr),
          sizeof(socket_addr), 4 * 60 * 1000))
  {
    std::cout << "Failed to bind socket. Error n°" << errno << "." << std::endl;
    return false;
  }

  return listen(this->sockfd_, 3) != -1;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
