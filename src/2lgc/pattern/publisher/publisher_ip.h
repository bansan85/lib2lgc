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

#ifndef PATTERN_PUBLISHER_PUBLISHER_IP_H_
#define PATTERN_PUBLISHER_PUBLISHER_IP_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <cstdint>
#include <memory>
#include <thread>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

/**
 * @brief Interface to create a TCP server.
 *
 * @tparam T Message from protobuf.
 *
 * @dotfile pattern/publisher/publisher_tcp.dot
 */
template <typename T>
class PublisherIp
    : public llgc::pattern::publisher::PublisherInterface<
          T, std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit PublisherIp(uint16_t port);

  /**
   * @brief Destructor. Make sure that thread is finished.
   */
  ~PublisherIp() override;

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
   * @brief Stop the thread.
   */
  virtual void Stop() = 0;

  /**
   * @brief Join the waiting thread.
   */
  virtual void JoinWait();

  /**
   * @brief Return the port.
   *
   * @return The port.
   */
  uint16_t GetPort() { return port_; }

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherIp(PublisherIp&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherIp(PublisherIp const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherIp& operator=(PublisherIp&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherIp& operator=(PublisherIp const& other) & = delete;
#endif  // !SWIG

 protected:
  /**
   * @brief Thread that run the Wait function and add socket to thread_sockets_.
   */
  std::thread thread_wait_;

 private:
  /**
   * @brief Port to listen from.
   */
  uint16_t port_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_IP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
