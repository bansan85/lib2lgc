/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
  struct sockaddr_in socket_addr;
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
