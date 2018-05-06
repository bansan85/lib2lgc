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

#ifndef NET_TCP_SERVER_H_
#define NET_TCP_SERVER_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/publisher.h>
#include <atomic>
#include <cstdint>
#include <map>
#include <memory>
#include <thread>
#include <type_traits>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;
}

/**
 * @brief This is all about net.
 */
namespace llgc::net
{
/**
 * @brief Interface to create a TCP server.
 *
 * @tparam T Message from protobuf.
 */
template <typename T>
class TcpServer
    : public llgc::pattern::publisher::Publisher<
          T, std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit TcpServer(uint16_t port);

  /**
   * @brief Destructor. Make sure that thread is finished.
   */
  virtual ~TcpServer();

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer(TcpServer&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer(TcpServer const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer& operator=(TcpServer&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer& operator=(TcpServer const& other) & = delete;
#endif  // !SWIG

  /**
   * @brief Start the server and the listening the port.
   *
   * @return true if no problem.
   */
  virtual bool Listen() CHK = 0;

  /**
   * @brief Wait for client.
   *
   * @return true if no problem.
   */
  virtual bool Wait() CHK = 0;

  /**
   * @brief If thread is in stopping.
   *
   * @return true if thread in trying to stop.
   */
  bool IsStopping();

  /**
   * @brief Stop the thread.
   */
  void Stop();

  /**
   * @brief Join the waiting thread.
   */
  void JoinWait();

 protected:
  /**
   * @brief Port to listen from.
   */
  uint16_t port_;

  /**
   * @brief Thread that run the Wait function and add socket to thread_sockets_.
   */
  std::thread thread_wait_;

  /**
   * @brief Store thread based on the socket file descriptor.
   */
  std::map<int, std::thread> thread_sockets_;  // NS

#ifndef SWIG
  /**
   * @brief Internal function to subscribe a socket to an event.
   *
   * @param[in] socket The socket.
   * @param[in] action_tcp A const reference of the message.
   */
  virtual void AddSubscriberLocal(
      int socket, decltype(std::declval<T>().action(0)) action_tcp) = 0;
#endif  // !SWIG

 private:
  /**
   * @brief If thread in trying to stop.
   */
  std::atomic<bool> disposing_;
};

}  // namespace llgc::net

#endif  // NET_TCP_SERVER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
