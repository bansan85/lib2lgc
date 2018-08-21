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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_H_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_H_

#include <2lgc/pattern/publisher/publisher_ip.h>
#include <atomic>
#include <cstdint>
#include <map>
#include <thread>
#include <type_traits>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Interface to create a TCP server.
 *
 * @tparam T Message from protobuf.
 *
 * @dotfile pattern/publisher/publisher_tcp.dot
 */
template <typename T>
class PublisherTcp : public PublisherIp<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit PublisherTcp(uint16_t port);

  /**
   * @brief Destructor. Make sure that thread is finished.
   */
  ~PublisherTcp() override;

  /**
   * @brief Join the waiting thread.
   */
  void JoinWait() override;

  /**
   * @brief Stop the thread.
   */
  void Stop() override;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherTcp(PublisherTcp&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherTcp(PublisherTcp const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherTcp& operator=(PublisherTcp&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherTcp& operator=(PublisherTcp const& other) & = delete;
#endif  // !SWIG

 protected:
  /**
   * @brief If thread in trying to stop.
   */
  std::atomic<bool> disposing_;

  /**
   * @brief Store thread based on the socket file descriptor.
   */
  std::map<int, std::thread> thread_sockets_;  // NS

 private:
#ifndef SWIG
  /**
   * @brief Internal function to subscribe a socket to an event.
   *
   * @param[in] socket The socket.
   * @param[in] message The message.
   */
  virtual void AddSubscriberLocal(
      int socket, decltype(std::declval<T>().msg(0)) message) = 0;
#endif  // !SWIG
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
