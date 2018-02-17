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

#ifndef PUBLISHER_REMOTE_H_
#define PUBLISHER_REMOTE_H_

#include "config.h"

#include "publisher_base.h"

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
class PublisherRemote : public PublisherBase<M> {
 public:
  /**
   * @brief Default constructor.
   */
  PublisherRemote();

  bool StartIp(uint16_t port) CHK;

  /**
   * @brief Add a subscriber to the server.
   *
   * @param id_message The message to subscribe.
   * @param subscriber The subscriber.
   *
   * @return true if success. May failed if add_fail_if_already_subscribed is
   * true and the subscriber is already registered.
   */
  bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) override CHK;

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

 protected:
  // using needed because of template inheritance.
  using PublisherBase<M>::options_;
  using PublisherBase<M>::subscribers_;
  using SubscriberMap =
      std::multimap<uint32_t, std::shared_ptr<ConnectorInterface>>;

 private:
  uint16_t port_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // PUBLISHER_REMOTE_H_
