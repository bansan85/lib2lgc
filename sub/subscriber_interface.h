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

#ifndef SUBSCRIBER_INTERFACE_H_
#define SUBSCRIBER_INTERFACE_H_

#include "config.h"

// C++ system
#include <cstdint>
#include <memory>
#include <string>

namespace pattern {
namespace publisher {

class ConnectorInterface;

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
class SubscriberInterface {
 public:
  /**
   * @brief Send message.
   *
   * @param data Data of the message in ProtoBuf, SerializeToString.
   */
  virtual void Listen(const std::shared_ptr<const std::string> &message) = 0;

  virtual bool Equals(const SubscriberInterface *connector) const = 0;

  /**
   * @brief Default virtual destructor.
   */
  virtual ~SubscriberInterface() {}
};

}  // namespace publisher
}  // namespace pattern

#endif  // SUBSCRIBER_INTERFACE_H_
