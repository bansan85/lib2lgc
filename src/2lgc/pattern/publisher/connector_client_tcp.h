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

#ifndef PATTERN_PUBLISHER_CONNECTOR_CLIENT_TCP_H_
#define PATTERN_PUBLISHER_CONNECTOR_CLIENT_TCP_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <cstdint>
#include <memory>
#include <string>

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
class ConnectorClientTcp : public ConnectorInterface<T>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] subscriber Subscriber to communicate with client.
   * @param[in] socket_fd Socket to communicate with client.
   */
  ConnectorClientTcp(
      std::shared_ptr<llgc::pattern::publisher::SubscriberInterface<T>>
          subscriber,  // NS
      int socket_fd);

  /**
   * @brief Default virtual destructor.
   */
  ~ConnectorClientTcp() override;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorClientTcp(ConnectorClientTcp &&other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorClientTcp(ConnectorClientTcp const &other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorClientTcp &operator=(ConnectorClientTcp &&other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorClientTcp &operator=(ConnectorClientTcp const &other) & = delete;

  /**
   * @brief Compare two connectors.
   *
   * @param[in,out] connector The connector to compare with this.
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
   */
  bool Send(const std::string &message) override CHK;

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

 protected:
  /**
   * @brief Socket to the server.
   */
  int socket_;  // NS
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_CLIENT_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
