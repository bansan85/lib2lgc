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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_CC_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorSubscriberTcp;

template <typename T>
class SubscriberServerTcp;
}  // namespace llgc::pattern::publisher

/** \class llgc::pattern::publisher::PublisherTcpLinux
 * \brief Interface to create a TCP server.
 */

/** \brief Constructor with port for the TCP server.
 * \param[in] port The port to listen from.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::PublisherTcpLinux<T>::PublisherTcpLinux(uint16_t port)
    : PublisherTcp<T>(port), sockfd_(-1)
{
}

template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::PublisherTcpLinux<T>::Wait()
{
  if (sockfd_ == -1)
  {
    return false;
  }

  std::thread t([this]() {
    int iResult;
    int client_sock = 0;
    fd_set rfds;

    do
    {
      FD_ZERO(&rfds);  // NOLINT(hicpp-no-assembler)
      FD_SET(sockfd_, &rfds);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
      struct timeval tv;  // NOLINT(hicpp-member-init)
      tv.tv_sec = 0L;
      tv.tv_usec = 50000L;

      iResult = llgc::net::Linux::RepeteOnEintr([this, &rfds, &tv] {
        return select(sockfd_ + 1, &rfds, nullptr, nullptr, &tv);
      });
      if (iResult > 0)
      {
        client_sock = llgc::net::Linux::RepeteOnEintr(
            [=] { return accept4(sockfd_, nullptr, nullptr, SOCK_CLOEXEC); });
        if (client_sock > 0)
        {
          std::thread t2(
              [this, client_sock] { return WaitThread(client_sock); });
          this->thread_sockets_.insert(
              std::pair<int, std::thread>(client_sock, std::move(t2)));
        }
      }
    } while (iResult >= 0 && client_sock >= 0 && !this->disposing_);
    BUGCRIT(std::cout, iResult != -1, ,
            "Errno " + std::to_string(errno) + "\n");
    BUGCRIT(std::cout, client_sock != -1, ,
            "Errno " + std::to_string(errno) + "\n");
  });

  this->thread_wait_ = std::move(t);

  return true;
}

/** \var llgc::pattern::publisher::PublisherTcpLinux::sockfd_
 * \brief Socket file descriptor.
 */

template <typename T>
INLINE_TEMPLATE void
llgc::pattern::publisher::PublisherTcpLinux<T>::AddSubscriberLocal(
    int socket, decltype(std::declval<T>().msg(0)) message)
{
  BUGCRIT(std::cout, message.has_add_subscriber(), ,
          "Failed to add a subscriber.");
  std::shared_ptr<llgc::pattern::publisher::SubscriberServerTcp<T>> subscriber =
      std::make_shared<llgc::pattern::publisher::SubscriberServerTcp<T>>(
          socket);
  std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberTcp<T>>
      connector =
          std::make_shared<llgc::pattern::publisher::ConnectorSubscriberTcp<T>>(
              subscriber, socket);
  BUGCONT(
      std::cout,
      this->AddSubscriber(message.add_subscriber().id_message(), connector), );
}

/** \brief Function that will be executed by the thread that wait instruction
 *         from a client.
 * \param[in] socket The socket to the client.
 */
template <typename T>
INLINE_TEMPLATE void llgc::pattern::publisher::PublisherTcpLinux<T>::WaitThread(
    int socket)
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(&socket);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = socket;
  fd.events = POLLIN;

  do
  {
    int retval = poll(&fd, 1, 50);

    BUGCRIT(std::cout, retval != -1, ,
            "Server client " + std::to_string(socket) +
                " poll failed. Close connection. Errno " +
                std::to_string(errno) + ".\n");

    if (retval == 0)
    {
      continue;
    }

    char client_message[1500];

    ssize_t read_size = recv(socket, client_message, sizeof(client_message), 0);

    BUGCRIT(std::cout, read_size != -1, ,
            "Server client " + std::to_string(socket) +
                " recv failed. Close connection. Errno " +
                std::to_string(errno) + ".\n");
    // Empty message.
    if (read_size == 0)
    {
      continue;
    }

    T messages;

    std::string client_string(client_message, static_cast<size_t>(read_size));
    BUGLIB(std::cout, messages.ParseFromString(client_string), , "protobuf.");

    for (int i = 0; i < messages.msg_size(); i++)
    {
      auto& message = messages.msg(i);

      auto enumeration = message.data_case();

      if (enumeration ==
          std::remove_reference<decltype(message)>::type::kAddSubscriber)
      {
        AddSubscriberLocal(socket, message);
      }
      else if (enumeration == std::remove_reference<decltype(
                                  message)>::type::kRemoveSubscriber)
      {
        // We need to create a temporary connector with the socket
        // because the Remover only works with connector.
        std::shared_ptr<llgc::pattern::publisher::SubscriberServerTcp<T>>
            subscriber = std::make_shared<
                llgc::pattern::publisher::SubscriberServerTcp<T>>(socket);
        std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberTcp<T>>
            connector = std::make_shared<
                llgc::pattern::publisher::ConnectorSubscriberTcp<T>>(subscriber,
                                                                     socket);
        BUGCONT(std::cout,
                this->RemoveSubscriber(message.remove_subscriber().id_message(),
                                       connector), );
      }
    }

    BUGCONT(std::cout, this->Forward(messages), );
  } while (!this->disposing_);
}

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
