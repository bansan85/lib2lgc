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

/** \file linux.h
 * \brief Add various function specific for the network on Linux.
 */

#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <unistd.h>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
// No for GCC-8. GCC-6 wants.
#include <type_traits>  // IWYU pragma: keep

/** \namespace llgc::net
 * \brief This is all about net.
 *
 *
 * \class llgc::net::Linux
 * \brief Class to manipulate Linux functions.
 */

/** \brief Disable interrupt on signal `PIPE`.
 * \return `false` if something wrong.
 */
bool llgc::net::Linux::DisableSigPipe()
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sigaction sig;  // NOLINT(hicpp-member-init)

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
  sig.sa_handler = SIG_IGN;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  sig.sa_flags = 0;
  BUGCRIT(std::cout, sigemptyset(&sig.sa_mask) == 0, false,
          "Errno " + std::to_string(errno) + "\n");
  BUGCRIT(std::cout, sigaction(SIGPIPE, &sig, nullptr) == 0, false,
          "Errno " + std::to_string(errno) + "\n");
  return true;
}

/** \brief `bind` function with timeout.
 * \param[in] sockfd Socket file descriptor.
 * \param[in] addr Address of the socket description.
 * \param[in] addrlen Length of the socket description.
 * \param[in] timeout Timeout in millisecond.
 * \return `false` if timeout or failed to bind.
 */
bool llgc::net::Linux::Bind(int sockfd, const struct sockaddr *addr,
                            socklen_t addrlen, size_t timeout)
{
  int retval_bind = bind(sockfd, addr, addrlen);
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

/** \brief `send` function with error handle.
 * \param[in] sockfd Socket file descriptor.
 * \param[in] message Message to send.
 * \return `true` if no problem.
 */
bool llgc::net::Linux::Send(int sockfd, const std::string &message)
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

/** \brief Execute a function. If this function return -1, check if errno ==
 *         EINTR. If so, reexecute the function. `RepeteOnEintr` should be used
 *         with select and accept4.
 * \param[in] function Function to execute.
 * \return The value returned by function.
 */
int llgc::net::Linux::RepeteOnEintr(const std::function<int()> &function)
{
  int retval;
  do
  {
    retval = function();
  } while (retval == -1 && errno == EINTR);
  return retval;
}

/** \class llgc::net::Linux::AutoCloseSocket
 * \brief Class that auto close a socket on destructor.
 */

/** \brief Constructor with the socket to handle.
 * \param[in] socket The socket.
 */
llgc::net::Linux::AutoCloseSocket::AutoCloseSocket(int *socket)
    : socket_(socket)
{
}

/** \fn llgc::net::Linux::AutoCloseSocket::AutoCloseSocket(AutoCloseSocket &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::net::Linux::AutoCloseSocket::AutoCloseSocket(AutoCloseSocket const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn AutoCloseSocket &llgc::net::Linux::AutoCloseSocket::operator=(AutoCloseSocket &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn AutoCloseSocket &llgc::net::Linux::AutoCloseSocket::operator=(AutoCloseSocket const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/// \brief Destructor that close the socket.
llgc::net::Linux::AutoCloseSocket::~AutoCloseSocket()
{
  if (!delete_socket_)
  {
    return;
  }
  int socket_tmp = *socket_;
  *socket_ = -1;
  BUGCRIT(std::cout, close(socket_tmp) == 0, ,
          "Errno " + std::to_string(errno) + "\n");
}

/// \brief Don't delete the socket on destrutor.
void llgc::net::Linux::AutoCloseSocket::DontDeleteSocket()
{
  delete_socket_ = false;
}

/** \var llgc::net::Linux::AutoCloseSocket::socket_
 * \brief The socket to handle.
 *
 *
 * \var llgc::net::Linux::AutoCloseSocket::delete_socket_
 * \brief If the socket must be deleted on destructor. Default is true.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
