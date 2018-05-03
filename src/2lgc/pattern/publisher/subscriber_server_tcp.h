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

#ifndef PATTERN_PUBLISHER_SUBSCRIBER_SERVER_TCP_H_
#define PATTERN_PUBLISHER_SUBSCRIBER_SERVER_TCP_H_

#include <2lgc/pattern/publisher/subscriber_interface.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class SubscriberServerTcp : public SubscriberInterface<T>
{
 public:
  /**
   * @brief Default constructor
   *
   * @param[in] socket Socket to communicate with server.
   */
  explicit SubscriberServerTcp(int socket) : socket_(socket) {}

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp(SubscriberServerTcp&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp(SubscriberServerTcp const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp& operator=(SubscriberServerTcp&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp& operator=(SubscriberServerTcp const& other) & = delete;
  /**
   * @brief Receive message from publisher.
   *
   * @param[in] message message from the publisher in protobuf format.
   */

  void Listen(const T& message) override;

  /**
   * @brief Compare in connector is the same than the object.
   *
   * @param[in,out] connector The connector to compare with.
   *
   * @return true if the same.
   */
  bool Equals(const SubscriberInterface<T>& connector) const override
      __attribute__((pure));

  /**
   * @brief Default virtual destructor.
   */
  ~SubscriberServerTcp() override {}

 private:
  /**
   * @brief The id of the connector.
   */
  const int socket_;  // NS
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_SERVER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
