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

#include "publisher_base.h"

// C++ system
#include <memory>

// macros
#include <handle_error.h>

template <typename M>
pattern::publisher::PublisherBase<M>::PublisherBase()
    : subscribers_(), options_() {}

template <typename M>
void pattern::publisher::PublisherBase<M>::Forward(
    const std::shared_ptr<const std::string> &message) {
  M actions;

  BUGUSER(actions.ParseFromString(*message.get()), ,
          "Failed to decode message.\n");

  for (int i = 0; i < actions.action_size(); i++) {
    // Must use auto because we don't know if M is in namespace or not.
    auto &action = actions.action(i);

    std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
        iterpair = subscribers_.equal_range(action.data_case());

    SubscriberMap::const_iterator it = iterpair.first;
    for (; it != iterpair.second; ++it) {
      it->second->Listen(message);
    }
  }
}

template <typename M>
bool pattern::publisher::PublisherBase<M>::RemoveSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface> subscriber) {
  // Check if Subscriber is already subscribed.
  std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
      iterpair = subscribers_.equal_range(id_message);

  SubscriberMap::const_iterator it = iterpair.first;
  for (; it != iterpair.second; ++it) {
    if (it->second.get()->Equals(subscriber.get())) {
      subscribers_.erase(it);
      return true;
    }
  }

  return false;
}
