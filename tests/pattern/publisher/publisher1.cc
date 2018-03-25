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

#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/publisher_base.h>
#include <2lgc/pattern/publisher/publisher_remote.h>
#include <2lgc/pattern/publisher/subscriber_direct.h>
#include <actions.pb.h>
#include <bits/stdint-uintn.h>
#include <google/protobuf/stubs/common.h>
#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/publisher_base.cc>
#include <2lgc/pattern/publisher/publisher_remote.cc>
#include <cassert>
#include <memory>
#include <string>

template class llgc::pattern::publisher::PublisherBase<msg::Actions>;
template class llgc::pattern::publisher::PublisherRemote<msg::Actions>;
template class llgc::pattern::publisher::ConnectorDirect<msg::Actions>;

/**
 * @brief Simple implementation of a direct subscriber.
 */
class SubscriberBase final : public llgc::pattern::publisher::SubscriberDirect
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id id of the subscriber.
   */
  explicit SubscriberBase(uint32_t id) : SubscriberDirect(id), value(0) {}

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] message message from the publisher in protobuf format.
   */
  void Listen(const std::shared_ptr<const std::string> &message) override
  {
    msg::Actions actions;
    assert(actions.ParseFromString(*message.get()));
    value = 3;
  }

  /**
   * @brief value for test.
   */
  size_t value;
};

int main(int /* argc */, char * /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<llgc::pattern::publisher::PublisherRemote<msg::Actions>>
      server = std::make_shared<
          llgc::pattern::publisher::PublisherRemote<msg::Actions>>();

  // Create three subscribers.
  std::shared_ptr<SubscriberBase> subscriber =
      std::make_shared<SubscriberBase>(1);
  std::shared_ptr<llgc::pattern::publisher::ConnectorDirect<msg::Actions>>
      connector = std::make_shared<
          llgc::pattern::publisher::ConnectorDirect<msg::Actions>>(subscriber,
                                                                   server);

  // Add them to the server.
  assert(connector->AddSubscriber(1));

  // Base test case.
  assert(subscriber->value == 0);

  // Check first message.
  msg::Actions actions = msg::Actions();
  msg::Action *action = actions.add_action();
  std::unique_ptr<msg::Action_Test> action_test =
      std::make_unique<msg::Action_Test>();
  action->set_allocated_test(action_test.release());
  std::shared_ptr<std::string> action_in_string =
      std::make_shared<std::string>();
  actions.SerializeToString(action_in_string.get());
  connector->Send(action_in_string);

  assert(subscriber->value == 3);

  // Reset test case.
  subscriber->value = 0;

  // Remove the first subscriber.
  assert(connector->RemoveSubscriber(1));
  connector->Send(action_in_string);
  assert(subscriber->value == 0);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
