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

#ifndef PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_TCP_CC_
#define PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_TCP_CC_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_tcp.h>
#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <string>

/** \class llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp
 * \brief Strategy for server to receive data from TCP.
 * \tparam T Message from protobuf.
 */

/** \brief Constructor for Strategy server TCP.
 * \param[in] server The TCP server.
 * \param[in] client_sock Socket client to communicate by TCP.
 */
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp<T>::
    StrategyPublisherTcpLinuxTcp(
        llgc::pattern::publisher::PublisherTcpLinux<T> *server,
        int &client_sock)
    : llgc::pattern::Strategy<llgc::pattern::publisher::PublisherTcpLinux<T>>(
          server),
      client_sock_(client_sock)
{
}

/** \fn llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp::~StrategyPublisherTcpLinuxTcp()
 * \brief Default destructor.
 */

/** \brief Function that will be executed by the thread that wait instruction
 *         from a client.
 * \return true if no problem.
 */
template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp<T>::Do()
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(&client_sock_);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = client_sock_;
  fd.events = POLLIN;

  do
  {
    int retval = poll(&fd, 1, 50);

    BUGCRIT(std::cout, retval != -1, false,
            "Server client " + std::to_string(client_sock_) +
                " poll failed. Close connection. Errno " +
                std::to_string(errno) + ".\n");

    if (retval == 0)
    {
      continue;
    }

    char client_message[1500];

    ssize_t read_size =
        recv(client_sock_, client_message, sizeof(client_message), 0);

    BUGCRIT(std::cout, read_size != -1, false,
            "Server client " + std::to_string(client_sock_) +
                " recv failed. Close connection. Errno " +
                std::to_string(errno) + ".\n");
    // Empty message.
    if (read_size == 0)
    {
      continue;
    }

    T messages;

    BUGCONT(std::cout,
            this->instance_->PreForward(&messages, &client_message[0],
                                        read_size, client_sock_),
            false);

    BUGCONT(std::cout, this->instance_->Forward(messages), false);
  } while (!this->instance_->GetDisposing());
  return true;
}

/** \var llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp::client_sock_
 * \brief Value of the socket. Turn back to -1 when Do finish.
 */

#endif  // PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_TCP_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
