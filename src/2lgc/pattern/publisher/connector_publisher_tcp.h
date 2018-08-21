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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_H_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class ConnectorPublisherTcp : public ConnectorInterface<T>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] subscriber The subscriber.
   * @param[in] ip The IP of the server.
   * @param[in] port The port of the server.
   */
  ConnectorPublisherTcp(std::shared_ptr<SubscriberInterface<T>> subscriber,
                        std::string ip, uint16_t port);

  /**
   * @brief Default virtual destructor.
   */
  ~ConnectorPublisherTcp() override;

  /**
   * @brief Compare two connectors.
   *
   * @param[in] connector The connector to compare with this.
   *
   * @return true if same connector.
   */
  bool Equals(const ConnectorInterface<T> &connector) const override CHK;

  /**
   * @brief Send message to the publisher.
   *
   * @param[in] message Data of the message in ProtoBuf, SerializeToString.
   *
   * @return true if no problem.
   *
   * @dotfile pattern/publisher/publisher_tcp_send_message.dot
   */
  bool Send(const T &message) override CHK;

  /**
   * @brief Add a subscriber.
   *
   * @param[in] id_message The id of the message.
   *
   * @return true if no problem.
   */
  bool AddSubscriber(uint32_t id_message) override CHK;

  /**
   * @brief Remove a subscriber.
   *
   * @param[in] id_message The id of the message.
   *
   * @return true if no problem.
   */
  bool RemoveSubscriber(uint32_t id_message) override CHK;

  /**
   * @brief The function used by the thread that receive message from server.
   *
   * Need to be public so thread can use it. Protected is not possible.
   */
  void Receiver();

#ifndef SWIG
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorPublisherTcp(ConnectorPublisherTcp &&other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorPublisherTcp(ConnectorPublisherTcp const &other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorPublisherTcp &operator=(ConnectorPublisherTcp &&other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorPublisherTcp &operator=(ConnectorPublisherTcp const &other) & =
      delete;
#endif  // !SWIG

 protected:
  /**
   * @brief Thread that listen the server.
   */
  std::thread receiver_;

  /**
   * @brief Socket to the server.
   */
  int socket_;  // NS

  /**
   * @brief Start connection with server.
   *
   * @return true if no problem.
   */
  virtual bool Connect() CHK = 0;

  /**
   * @brief Get the ip of the connector.
   *
   * @return The return value;
   */
  const std::string &GetIp() { return ip_; }

  /**
   * @brief Get the port of the connector.
   *
   * @return The return value;
   */
  uint16_t GetPort() { return port_; }

 private:
  /**
   * @brief The IP of the server.
   */
  std::string ip_;

  /**
   * @brief The port of the server.
   */
  uint16_t port_;

  /**
   * @brief If thread in trying to stop.
   */
  std::atomic<bool> disposing_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
