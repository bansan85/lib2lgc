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
#include <unistd.h>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <type_traits>

void llgc::net::Linux::DisableSigPipe()
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sigaction sig;  // NOLINT(hicpp-member-init)

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
  sig.sa_handler = SIG_IGN;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  sig.sa_flags = 0;
  BUGCRIT(std::cout, sigemptyset(&sig.sa_mask) == 0, ,
          "Errno " + std::to_string(errno) + "\n");
  BUGCRIT(std::cout, sigaction(SIGPIPE, &sig, nullptr) == 0, ,
          "Errno " + std::to_string(errno) + "\n");
}

bool llgc::net::Linux::Bind(int sockfd, const struct sockaddr* addr,
                            socklen_t addrlen, size_t timeout)
{
  int retval_bind = bind(sockfd, addr, addrlen);  // NS
  // We have to wait the end of the timeout.
  if (retval_bind == -1 && errno == EADDRINUSE)
  {
    std::cout << "Opening TCP port socket in use. Please wait…" << std::endl;

    // setsockopt(SO_REUSEADDR) doesn't work.
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    do
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      retval_bind = bind(sockfd, addr, addrlen);
      end = std::chrono::system_clock::now();
    } while (
        retval_bind == -1 && errno == EADDRINUSE &&
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count()) < timeout);
  }

  BUGUSER(std::cout, retval_bind == 0, false,
          "Failed to open a TCP port socket. Errno " + std::to_string(errno) +
              ".\n");
  return true;
}

bool llgc::net::Linux::Send(int sockfd, const std::string& message)
{
  ssize_t retval_send = send(sockfd, message.c_str(), message.length(), 0);

  BUGCRIT(std::cout, retval_send != -1, false,
          "Send failed. Errno " + std::to_string(errno) + ".\n");
  BUGCRIT(std::cout, retval_send == static_cast<ssize_t>(message.length()),
          false,
          "Send partial message. Length " + std::to_string(retval_send) +
              ". Errno " + std::to_string(errno) + ".\n");

  return true;
}

int llgc::net::Linux::RepeteOnEintr(const std::function<int()>& function)  // NS
{
  int retval;  // NS
  do
  {
    retval = function();
  } while (retval == -1 && errno == EINTR);
  return retval;
}

llgc::net::Linux::AutoCloseSocket::AutoCloseSocket(int* socket)
    : socket_(socket), delete_socket_(true)
{
}

void llgc::net::Linux::AutoCloseSocket::DontDeleteSocket()
{
  delete_socket_ = false;
}

llgc::net::Linux::AutoCloseSocket::~AutoCloseSocket()
{
  if (!delete_socket_)
  {
    return;
  }
  int socket_tmp = *socket_;  // NS
  *socket_ = -1;
  BUGCRIT(std::cout, close(socket_tmp) == 0, ,
          "Errno " + std::to_string(errno) + "\n");
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
