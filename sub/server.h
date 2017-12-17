/* Copyright [2017] LE GARREC Vincent
 *
 * This file is part of 2LGC.
 *
 * 2LGC is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * 2LGC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with 2LGC. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "config.h"

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
class Server {
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
  Server();

  /**
   * @brief Add a subscriber to the server.
   *
   * @param id_message The message to subscribe.
   * @param subscriber The subscriber.
   *
   i* @return true if success. May failed if add_fail_if_already_subscribed i2
   * true and the subscriber is already registered.
   */
  bool AddSubscriber(uint32_t id_message,
                     std::shared_ptr<ConnectorInterface> subscriber) CHK;

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
  bool RemoveSubscriber(uint32_t id_message,
                        std::shared_ptr<ConnectorInterface> subscriber) CHK;

 private:
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

#endif  // SERVER_H_
