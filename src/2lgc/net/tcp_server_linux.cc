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
#include <2lgc/net/tcp_server.h>
#include <2lgc/net/tcp_server_linux.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorClientTcp;

template <typename T>
class SubscriberServerTcp;
}  // namespace llgc::pattern::publisher

template <typename T>
llgc::net::TcpServerLinux<T>::TcpServerLinux(uint16_t port)
    : TcpServer<T>(port), sockfd_(-1)
{
}

template <typename T>
bool llgc::net::TcpServerLinux<T>::Wait()
{
  if (sockfd_ == -1)
  {
    return false;
  }

  std::thread t([this]() {
    int iResult;          // NS
    int client_sock = 0;  // NS
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

      iResult = Linux::RepeteOnEintr(
          std::bind(&select, sockfd_ + 1, &rfds, nullptr, nullptr, &tv));
      if (iResult > 0)
      {
        client_sock = Linux::RepeteOnEintr(
            std::bind(accept4, sockfd_, nullptr, nullptr, 0));
        if (client_sock > 0)
        {
          std::cout << "Server new client" << client_sock << std::endl;
          std::thread t2(
              std::bind(&TcpServerLinux<T>::WaitThread, this, client_sock));
          this->thread_sockets_.insert(
              std::pair<int, std::thread>(client_sock, std::move(t2)));
        }
      }
    } while (iResult >= 0 && client_sock >= 0 && !this->IsStopping());
    std::cout << "Server stop listening" << std::endl;
    BUGCRIT(iResult != -1, , "Errno %\n", errno);
    BUGCRIT(client_sock != -1, , "Errno %\n", errno);
  });  // NS

  this->thread_wait_ = std::move(t);

  return true;
}

template <typename T>
void llgc::net::TcpServerLinux<T>::WaitThread(int socket)
{
  Linux::AutoCloseSocket auto_close_socket(&socket);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = socket;
  fd.events = POLLIN;
  int retval;  // NS

  std::cout << "Server Client " << socket << " Start" << std::endl;

  do
  {
    retval = poll(&fd, 1, 50);

    BUGCRIT(retval != -1, ,
            "Server client % poll failed. Close connection. Errno %.\n", socket,
            errno);

    if (retval == 0)
    {
      continue;
    }

    char client_message[1500];

    ssize_t read_size = recv(socket, client_message, sizeof(client_message), 0);

    BUGCRIT(read_size != -1, ,
            "Server client % recv failed. Close connection. Errno %.\n", socket,
            errno);
    if (read_size == 0)
    {
      std::cout << "Server Client " << socket << ", empty message.\n"
                << std::endl;
      continue;
    }

    std::cout << "Server Client " << socket << " Talk" << std::endl;
    T message;

    std::string client_string(client_message, static_cast<size_t>(read_size));
    BUGLIB(message.ParseFromString(client_string), , "protobuf.");

    for (int i = 0; i < message.action_size(); i++)
    {
      auto& action_tcp = message.action(i);

      auto enumeration = action_tcp.data_case();

      if (enumeration ==
          std::remove_reference<decltype(action_tcp)>::type::kAddSubscriber)
      {
        AddSubscriberLocal(socket, action_tcp);
      }
      /*
      case action_tcp.kRemoveSubscriber:
      {
        AddSubscriber(socket, &action_tcp);
        break;
      }*/
    }

    BUGCONT(this->Forward(client_string), );
  } while (!this->IsStopping());

  std::cout << "Server Client " << socket << " End" << std::endl;
}

template <typename T>
void llgc::net::TcpServerLinux<T>::AddSubscriberLocal(
    int socket, decltype(std::declval<T>().action(0)) action_tcp)
{
  BUGCRIT(action_tcp.has_add_subscriber(), , "Failed to add a subscriber.");
  std::shared_ptr<llgc::pattern::publisher::SubscriberServerTcp<T>> subscriber =
      std::make_shared<llgc::pattern::publisher::SubscriberServerTcp<T>>(
          socket);
  std::shared_ptr<llgc::pattern::publisher::ConnectorClientTcp<T>> connector =
      std::make_shared<llgc::pattern::publisher::ConnectorClientTcp<T>>(
          subscriber, socket);
  BUGCRIT(
      this->AddSubscriber(action_tcp.add_subscriber().id_message(), connector),
      , "Failed to add a subscriber.");
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
