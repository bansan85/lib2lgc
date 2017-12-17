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

#ifndef CONNECTOR_INTERFACE_H_
#define CONNECTOR_INTERFACE_H_

#include "config.h"

// C++ system
#include <memory>
#include <queue>

#include "subscriber_interface.h"

namespace pattern {
namespace publisher {

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
class ConnectorInterface {
 public:
  explicit ConnectorInterface(
      const std::shared_ptr<SubscriberInterface> &subscriber)
      : messages_(), next_id_(0), subscriber_(subscriber) {}

  virtual bool Equals(const ConnectorInterface *connector) const CHK = 0;

  virtual bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) CHK = 0;

  /**
   * @brief Send message.
   *
   * @param id_message The id of the message.
   * @param data Data of the message in ProtoBuf, SerializeToString.
   */
  virtual void Send(const std::shared_ptr<const std::string> &message) = 0;

  virtual void Listen(const std::shared_ptr<const std::string> &message) = 0;

  virtual bool RemoveSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) CHK = 0;

  const SubscriberInterface *GetSubscriber() const { return subscriber_.get(); }

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorInterface() {}

 protected:
  // Pair with the id of the event and the arguments.
  std::queue<std::pair<uint32_t, std::shared_ptr<const std::string>>> messages_;
  // id of the next message.
  uint32_t next_id_;
  std::shared_ptr<SubscriberInterface> subscriber_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // CONNECTOR_INTERFACE_H_
