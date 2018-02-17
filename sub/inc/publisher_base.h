/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PUBLISHER_BASE_H_
#define PUBLISHER_BASE_H_

// C++ system
#include <cstdint>
#include <map>
#include <memory>
#include <queue>

// Current project
#include "connector_interface.h"

namespace pattern {
namespace publisher {

/**
 * @brief Server that will be used to managed subscribers and to keep and send
 *        messages.
 */
template <typename M>
class PublisherBase {
 public:
  /**
   * @brief Options of the server.
   */
  struct Options {
    /**
     * @brief To allow a subscriber to subscribe twice to the same message.
     *        Default: false.
     */
    bool add_fail_if_already_subscribed;

    /**
     * @brief Default constructor.
     */
    Options() : add_fail_if_already_subscribed(false) {}
  };

  /**
   * @brief Default constructor.
   */
  PublisherBase();

  /**
   * @brief Add a subscriber to the server.
   *
   * @param id_message The message to subscribe.
   * @param subscriber The subscriber.
   *
   * @return true if success. May failed if add_fail_if_already_subscribed is
   * true and the subscriber is already registered.
   */
  virtual bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) CHK = 0;

  /**
   * @brief Send the message to all subscribers.
   *
   * @param id_message The message.
   * @param message Data of the message in ProtoBuf, SerializeToString.
   */
  void Forward(const std::shared_ptr<const std::string> &message);

  /**
   * @brief Remove a subscriber of the server.
   *
   * @param id_message The message to unsubscriber.
   * @param subscriber The subscriber.
   *
   * @return true if subscriber is unsubscribed successfully.
   *         May failed if suscriber is not subscribe to the specific message.
   */
  virtual bool RemoveSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) CHK = 0;

 protected:
  /**
   * @brief Type of the map for subscribers.
   */
  using SubscriberMap =
      std::multimap<uint32_t, std::shared_ptr<ConnectorInterface>>;

  /**
   * @brief List of subscribers to send message.
   */
  SubscriberMap subscribers_;

  /**
   * @brief Options for the behavious of server.
   */
  Options options_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // PUBLISHER_BASE_H_
