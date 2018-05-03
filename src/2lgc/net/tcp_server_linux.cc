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

#include <2lgc/net/tcp_server.h>
#include <2lgc/net/tcp_server_linux.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <functional>
#include <iostream>
#include <thread>
#include <type_traits>
#include <utility>

template <typename T>
llgc::net::TcpServerLinux<T>::TcpServerLinux(uint16_t port)
    : TcpServer<T>(port), sockfd_(-1)
{
}

template <typename T>
bool llgc::net::TcpServerLinux<T>::Wait(
    std::function<void(llgc::net::TcpServer<T> *, int)> wait_function)
{
  if (sockfd_ == -1)
  {
    return false;
  }

  std::thread t([this, wait_function]() {
    int iResult;  // NS
    fd_set rfds;

    std::cout << "Server is waiting for client" << std::endl;

    do
    {
      // NOLINTNEXTLINE(hicpp-no-assembler)
      FD_ZERO(&rfds);
      FD_SET(sockfd_, &rfds);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
      struct timeval tv;  // NOLINT(hicpp-member-init)
      tv.tv_sec = 0L;
      tv.tv_usec = 50000L;

      iResult = select(sockfd_ + 1, &rfds, nullptr, nullptr, &tv);
      if (iResult > 0)
      {
        int client_sock = accept4(sockfd_, nullptr, nullptr, 0);  // NS
        if (client_sock > 0)
        {
          std::cout << "Server new client" << client_sock << std::endl;
          std::thread t2(std::bind(wait_function, this, client_sock));
          this->thread_sockets_.insert(
              std::pair<int, std::thread>(client_sock, std::move(t2)));
        }
      }
    } while (iResult >= 0 && !this->IsStopping());
    std::cout << "Server stop listening" << std::endl;
  });  // NS

  this->thread_wait_ = std::move(t);

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
