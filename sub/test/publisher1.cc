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

// C++ system
#include <cassert>

// lib2lgcPublisher
#include <connector_direct.h>
#include <publisher_remote.h>
#include <subscriber_direct.h>
#include <connector_direct.cc>
#include <publisher_base.cc>
#include <publisher_remote.cc>

// Current project
#include "actions.pb.h"

template class pattern::publisher::PublisherBase<msg::Actions>;
template class pattern::publisher::PublisherRemote<msg::Actions>;
template class pattern::publisher::ConnectorDirect<msg::Actions>;

class SubscriberBase final : public pattern::publisher::SubscriberDirect
{
 public:
  explicit SubscriberBase(uint32_t id) : SubscriberDirect(id), value(0) {}

  void Listen(const std::shared_ptr<const std::string> &message) override
  {
    msg::Actions actions;
    assert(actions.ParseFromString(*message.get()));
    value = 3;
  }

  int value;
};

int main(int /* argc */, char * /* argv */ [])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<pattern::publisher::PublisherRemote<msg::Actions>> server =
      std::make_shared<pattern::publisher::PublisherRemote<msg::Actions>>();

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
