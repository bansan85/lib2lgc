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

#ifndef SUBSCRIBER_DIRECT_H_
#define SUBSCRIBER_DIRECT_H_

#include "config.h"

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
class SubscriberDirect : public SubscriberInterface {
 public:
  explicit SubscriberDirect(uint32_t id) : id_(id) {}

  bool Equals(const SubscriberInterface *connector) const override
      __attribute__((pure));

  /**
   * @brief Default virtual destructor.
   */
  virtual ~SubscriberDirect() {}

 private:
  const uint32_t id_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // SUBSCRIBER_DIRECT_H_
