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

#include "server.h"

pattern::publisher::Server::Server() : subscribers_(), options_(), version_() {}

// Do not fail if subscriber is already subscribed in the same id_message.
bool pattern::publisher::Server::AddSubscriber(uint32_t id_message,
                                               void *subscriber,
                                               SubscriberAction action) {
  if (options_.add_fail_if_already_subscribed) {
    std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
        iterpair = subscribers_.equal_range(id_message);

    SubscriberMap::const_iterator it = iterpair.first;
    for (; it != iterpair.second; ++it) {
      if (it->second.first == subscriber) {
        return false;
      }
    }
  }

  version_[id_message] = 0;
  subscribers_.insert({id_message, {subscriber, action}});

  return true;
}

bool pattern::publisher::Server::RemoveSubscriber(uint32_t id_message,
                                                  void *subscriber) {
  // Check if Subscriber is already subscribed.
  std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
      iterpair = subscribers_.equal_range(id_message);

  SubscriberMap::const_iterator it = iterpair.first;
  for (; it != iterpair.second; ++it) {
    if (it->second.first == subscriber) {
      subscribers_.erase(it);
      return true;
    }
  }

  return false;
}

void pattern::publisher::Server::SendMessage(uint32_t id_message,
                                             void *message) {
  version_[id_message]++;

  std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
      iterpair = subscribers_.equal_range(id_message);

  SubscriberMap::const_iterator it = iterpair.first;
  for (; it != iterpair.second; ++it) {
    it->second.second(message);
  }
}
