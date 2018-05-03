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
