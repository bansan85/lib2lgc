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

#ifndef PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_
#define PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_

#include <2lgc/compat.h>
#include <cstdint>
#include <memory>
#include <queue>
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
class ConnectorInterface
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in,out] subscriber Subscriber.
   */
  explicit ConnectorInterface(
      std::shared_ptr<SubscriberInterface<T>> subscriber);

  /**
   * @brief Default destructor.
   */
  virtual ~ConnectorInterface() = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface(ConnectorInterface&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface(ConnectorInterface const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface& operator=(ConnectorInterface&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface& operator=(ConnectorInterface const& other) & = delete;
#endif  // !SWIG

  /**
   * @brief Compare in connector is the same than the object.
   *
   * @param[in] connector The connector to compare with.
   *
   * @return true if the same.
   */
  virtual bool Equals(const ConnectorInterface<T>& connector) const CHK = 0;

  /**
   * @brief Send message to the publisher.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   *
   * @return true if no problem.
   */
  virtual bool Send(const std::string& message) CHK = 0;

  /**
   * @brief Listen message from the publisher.
   *
   * @param[in] messages Data of the message in ProtoBuf, SerializeToString.
   * @param[in] hold If true, message is not send to subscriber be will be
   * pending until ListenPending is called.
   *
   * @return true if no problem.
   */
  bool Listen(const T& messages, const bool hold) CHK;

  /**
   * @brief Listen pending messages from the publisher.
   *
   * @return true if no problem.
   */
  bool ListenPending() CHK;

  /**
   * @brief Get the subscriber that manager this interface.
   *
   * @return return the subscriber.
   */
  const SubscriberInterface<T>* GetSubscriber() const
  {
    return subscriber_.get();
  }

  /**
   * @brief Add a new subscriber.
   *
   * @param[in] id_message The add of the message.
   *
   * @return true if no problem.
   *
   * @dotfile pattern/publisher/publisher_add_subscriber.dot
   */
  virtual bool AddSubscriber(uint32_t id_message) CHK = 0;

  /**
   * @brief Remove a subscriber.
   *
   * @param[in] id_message id of the message.
   *
   * @return true if no problem.
   */
  virtual bool RemoveSubscriber(uint32_t id_message) CHK = 0;

 protected:
  /**
   * @brief Messages on hold.
   */
  std::queue<T> messages_;

  /**
   * @brief id of the next message.
   */
  uint32_t next_id_;

  /**
   * @brief The subscriber.
   */
  std::shared_ptr<SubscriberInterface<T>> subscriber_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
