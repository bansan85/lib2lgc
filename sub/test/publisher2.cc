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

// C++ system
#include <cassert>

// lib2lgcPublisher
#include <connector_direct.h>
#include <server_remote.h>
#include <subscriber_direct.h>
#include <connector_direct.cc>
#include <server_base.cc>
#include <server_remote.cc>

// Current project
#include "cpp/actions.pb.h"

template class pattern::publisher::ServerBase<msg::Actions>;
template class pattern::publisher::ServerRemote<msg::Actions>;
template class pattern::publisher::ConnectorDirect<msg::Actions>;

class SubscriberBase final : public pattern::publisher::SubscriberDirect {
 public:
  explicit SubscriberBase(uint32_t id) : SubscriberDirect(id), value(0) {}

  void Listen(const std::shared_ptr<const std::string> &message) override {
    msg::Actions actions;
    assert(actions.ParseFromString(*message.get()));
    value = 3;
  }

  int value;
};

int main(int /* argc */, char * /* argv */ []) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<pattern::publisher::ServerRemote<msg::Actions>> server =
      std::make_shared<pattern::publisher::ServerRemote<msg::Actions>>();

  // Create three subscribers.
  std::shared_ptr<SubscriberBase> subscriber =
      std::make_shared<SubscriberBase>(1);
  std::shared_ptr<pattern::publisher::ConnectorDirect<msg::Actions>> connector =
      std::make_shared<pattern::publisher::ConnectorDirect<msg::Actions>>(
          subscriber, server);

  // Add them to the server.
  assert(connector->AddSubscriber(1, connector));

  // Base test case.
  assert(subscriber->value == 0);

  // Check first message.
  msg::Actions actions = msg::Actions();
  msg::Action *action = actions.add_action();
  action->set_allocated_test(new msg::Action_Test());
  std::shared_ptr<std::string> action_in_string =
      std::make_shared<std::string>();
  actions.SerializeToString(action_in_string.get());
  connector->Send(action_in_string);

  assert(subscriber->value == 3);

  // Reset test case.
  subscriber->value = 0;

  // Remove the first subscriber.
  assert(connector->RemoveSubscriber(1, connector));
  connector->Send(action_in_string);
  assert(subscriber->value == 0);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
