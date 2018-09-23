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

#ifndef NET_STRATEGY_LISTEN_TCP_LINUX_CC_
#define NET_STRATEGY_LISTEN_TCP_LINUX_CC_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
// IWYU wants to remove it. But without you can't define method.
#include <2lgc/net/strategy_listen_tcp_linux.h>  // IWYU pragma: keep
#include <2lgc/pattern/strategy.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

/** \class llgc::net::StrategyListenTcpLinux
 * \brief Strategy for server to receive data from TCP.
 * \tparam T Message from protobuf.
 * \tparam U Type of the instance.
 */

/** \brief Constructor for Strategy server TCP.
 * \param[in] server The TCP server.
 * \param[in] client_sock Socket client to communicate by TCP.
 * \param[in] function Function to execute after receiving a message.
 */
template <typename T, typename U>
INLINE_TEMPLATE llgc::net::StrategyListenTcpLinux<T, U>::StrategyListenTcpLinux(
    U *server, int *client_sock, std::function<bool(const T &)> function)
    : llgc::pattern::Strategy<U>(server),
      client_sock_(client_sock),
      function_(std::move(function))
{
}

/** \fn llgc::net::StrategyListenTcpLinux::StrategyListenTcpLinux(StrategyListenTcpLinux&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::net::StrategyListenTcpLinux::StrategyListenTcpLinux(StrategyListenTcpLinux const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn StrategyListenTcpLinux& llgc::net::StrategyListenTcpLinux::operator=(StrategyListenTcpLinux&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn StrategyListenTcpLinux& llgc::net::StrategyListenTcpLinux::operator=(StrategyListenTcpLinux const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn llgc::net::StrategyListenTcpLinux::~StrategyListenTcpLinux()
 * \brief Default destructor.
 */

/** \brief Function that will be executed by the thread that wait instruction
 *         from a client.
 * \return true if no problem.
 */
template <typename T, typename U>
INLINE_TEMPLATE bool llgc::net::StrategyListenTcpLinux<T, U>::Do()
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(client_sock_);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = *client_sock_;
  fd.events = POLLIN;

  do
  {
    int retval =
        llgc::net::Linux::RepeteOnEintr([&fd] { return poll(&fd, 1, 50); });

    BUGCRIT(std::cout, retval != -1, false,
            "Server client " + std::to_string(*client_sock_) +
                " poll failed. Close connection. Errno " +
                std::to_string(errno) + ".\n");

    if (retval == 0)
    {
      continue;
    }

    char client_message[1500];

    ssize_t read_size =
        recv(*client_sock_, client_message, sizeof(client_message), 0);

    BUGCRIT(std::cout, read_size != -1, false,
            "Server client " + std::to_string(*client_sock_) +
                " recv failed. Close connection. Errno " +
                std::to_string(errno) + ".\n");
    // Empty message.
    if (read_size == 0)
    {
      continue;
    }

    T messages;
    std::string client_string(client_message, static_cast<size_t>(read_size));
    BUGLIB(std::cout, messages.ParseFromString(client_string), false,
           "protobuf");
    BUGCONT(std::cout, function_(messages), false);
  } while (!this->instance_->GetDisposing());
  return true;
}

/** \var llgc::net::StrategyListenTcpLinux::client_sock_
 * \brief Value of the socket. Turn back to -1 when Do finish.
 *
 *
 * \var llgc::net::StrategyListenTcpLinux::function_
 * \brief Function to execute after receiving a message.
 */

#endif  // NET_STRATEGY_LISTEN_TCP_LINUX_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
