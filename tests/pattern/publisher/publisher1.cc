/*
 *    Copyright 2018 LE GARREC Vincent
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_direct.h>
#include <2lgc/pattern/publisher/subscriber_direct.h>
#include <2lgc/utils/thread/count_lock.h>
#include <actions.pb.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ext/alloc_traits.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/subscriber_direct.cc>

template class llgc::pattern::publisher::PublisherInterface<
    msg::Actions,
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<msg::Actions>>>;
template class llgc::pattern::publisher::ConnectorInterface<msg::Actions>;
template class llgc::pattern::publisher::SubscriberDirect<msg::Actions>;
template class llgc::pattern::publisher::ConnectorDirect<msg::Actions>;

/**
 * @brief Simple implementation of a direct subscriber.
 */
class SubscriberBase final
    : public llgc::pattern::publisher::SubscriberDirect<msg::Actions>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id Id of the subscriber.
   */
  explicit SubscriberBase(uint32_t id)
      : SubscriberDirect(id),
        value(0),
        action_vector(msg::Action::DataCase::kTest + 1)
  {
    action_vector[0] = nullptr;
    action_vector[msg::Action::DataCase::kTest] = &SubscriberBase::TestFct;
  }

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] message Message from the publisher in protobuf format.
   */
  bool Listen(const msg::Actions& message) override
  {
    for (int i = 0; i < message.action_size(); i++)
    {
      const msg::Action& action = message.action(i);

      action_vector[action.data_case()](*this, action);
    }
    return true;
  }

  /**
   * @brief Function that will be run with test event.
   *
   * @param[in] action Action that contains the test event.
   */
  void TestFct(const msg::Action& action)
  {
    (void)action;
    value++;
  }

  /**
   * @brief value for test.
   */
  size_t value;

  /**
   * @brief Function to execute.
   */
  std::vector<std::function<void(SubscriberBase&, const msg::Action&)>>
      action_vector;
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<llgc::pattern::publisher::PublisherDirect<msg::Actions>>
      server = std::make_shared<
          llgc::pattern::publisher::PublisherDirect<msg::Actions>>();

  std::shared_ptr<SubscriberBase> subscriber =
      std::make_shared<SubscriberBase>(1);

  std::shared_ptr<llgc::pattern::publisher::ConnectorDirect<msg::Actions>>
      connector = std::make_shared<
          llgc::pattern::publisher::ConnectorDirect<msg::Actions>>(subscriber,
                                                                   server);

  // Add them to the server.
  assert(connector->AddSubscriber(msg::Action::DataCase::kTest));

  // Base test case.
  assert(subscriber->value == 0);

  // Check first message.
  msg::Actions actions = msg::Actions();
  msg::Action* action = actions.add_action();
  std::unique_ptr<msg::Action_Test> action_test =
      std::make_unique<msg::Action_Test>();
  action->set_allocated_test(action_test.release());
  std::string action_in_string;
  actions.SerializeToString(&action_in_string);
  assert(connector->Send(action_in_string));
  assert(subscriber->value == 1);

  // Test lock forward.
  subscriber->value = 0;
  {
    llgc::utils::thread::CountLock<size_t> lock = server->LockForward();
    assert(connector->Send(action_in_string));
    assert(subscriber->value == 0);
  }
  assert(subscriber->value == 1);

  // Remove the first subscriber.
  subscriber->value = 0;
  assert(connector->RemoveSubscriber(1));
  assert(connector->Send(action_in_string));
  assert(subscriber->value == 0);

  // Double insert
  assert(connector->AddSubscriber(1));
  assert(connector->AddSubscriber(1));
  assert(connector->Send(action_in_string));
  assert(subscriber->value == 2);
  assert(connector->RemoveSubscriber(1));
  assert(connector->Send(action_in_string));
  assert(subscriber->value == 3);
  assert(connector->RemoveSubscriber(1));
  assert(connector->Send(action_in_string));
  assert(subscriber->value == 3);
  assert(!server->GetOptionFailAlreadySubscribed());
  server->SetOptionFailAlreadySubscribed(true);
  assert(connector->AddSubscriber(1));
  assert(!connector->AddSubscriber(1));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
