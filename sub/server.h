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
#include <functional>
#include <list>
#include <map>

namespace pattern {
namespace publisher {

class Server {
 public:
  using SubscriberAction = std::function<void(void *)>;

  struct Options {
    bool add_fail_if_already_subscribed;

    Options() : add_fail_if_already_subscribed(false) {}
  };

  Server();

  bool AddSubscriber(uint32_t id_message, void *subscriber,
                     SubscriberAction action) CHK;
  bool RemoveSubscriber(uint32_t id_message, void *subscriber) CHK;
  void SendMessage(uint32_t id_message, void *message);

 private:
  // id of the message.
  using SubscriberMap =
      std::multimap<uint32_t,
                    // Pointer to the subscriber and function to execute.
                    std::pair<void *, SubscriberAction>>;

  // List of subscriber with id and function to execute.
  SubscriberMap subscribers_;

  // Option for the behavious of server.
  Options options_;

  // Each SendMessage increments the current value by 1.
  std::map<uint32_t, uint32_t> version_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // SERVER_H_
