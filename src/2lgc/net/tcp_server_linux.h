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

#ifndef NET_TCP_SERVER_LINUX_H_
#define NET_TCP_SERVER_LINUX_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/net/tcp_server.h>
#include <cstdint>
#include <type_traits>

/**
 * @brief This is all about net.
 */
namespace llgc::net
{
/**
 * @brief Interface to create a TCP server.
 */
template <typename T>
class TcpServerLinux : public TcpServer<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit TcpServerLinux(uint16_t port);

  /**
   * @brief Wait for client.
   *
   * @return true if no problem.
   */
  bool Wait() override CHK;

 protected:
  /**
   * @brief Socket file descriptor.
   */
  int sockfd_;  // NS

#ifndef SWIG
  /**
   * @brief Internal function to subscribe a socket to an event.
   *
   * @param[in] socket The socket.
   * @param[in] action_tcp The message.
   */
  void AddSubscriberLocal(
      int socket, decltype(std::declval<T>().action(0)) action_tcp) override;
#endif  // !SWIG

 private:
  /**
   * @brief Function that will be executed by the thread that wait instruction
   * from a client.
   *
   * @param[in] socket The socket to the client.
   */
  void WaitThread(int socket);
};

}  // namespace llgc::net

#endif  // NET_TCP_SERVER_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
