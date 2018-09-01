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

// Test direct publisher

#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_direct.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/subscriber_local.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ext/alloc_traits.h>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include "direct.pb.h"
#include "publisher_all.h"

#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/subscriber_local.cc>
#include "publisher_all.cc"

template class llgc::pattern::publisher::PublisherInterface<
    llgc::protobuf::test::Direct,
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<
        llgc::protobuf::test::Direct>>>;
template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::test::Direct>;
template class llgc::pattern::publisher::ConnectorDirect<
    llgc::protobuf::test::Direct>;
template class llgc::pattern::publisher::SubscriberLocal<
    llgc::protobuf::test::Direct>;

/**
 * @brief Simple implementation of a direct subscriber.
 */
class Subscriber final : public llgc::pattern::publisher::SubscriberLocal<
                             llgc::protobuf::test::Direct>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id Id of the subscriber.
   */
  explicit Subscriber(uint32_t id)
      : SubscriberLocal(id),
        value(0),
        message_vector(llgc::protobuf::test::Direct_Msg::DataCase::kTest + 1)
  {
    message_vector[0] = nullptr;
    message_vector[llgc::protobuf::test::Direct_Msg::DataCase::kTest] =
        &Subscriber::TestFct;
  }

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] messages Message from the publisher in protobuf format.
   */
  bool Listen(const llgc::protobuf::test::Direct& messages) override
  {
    for (int i = 0; i < messages.msg_size(); i++)
    {
      const llgc::protobuf::test::Direct_Msg& message = messages.msg(i);

      assert(message_vector[message.data_case()] != nullptr);
      message_vector[message.data_case()](*this, message);
    }
    return true;
  }

  /**
   * @brief Function that will be run with test event.
   *
   * @param[in] message Action that contains the test event.
   */
  void TestFct(const llgc::protobuf::test::Direct_Msg& message)
  {
    (void)message;
    value++;
  }

  /**
   * @brief value for test.
   */
  size_t value;

  /**
   * @brief Function to execute.
   */
  std::vector<
      std::function<void(Subscriber&, const llgc::protobuf::test::Direct_Msg&)>>
      message_vector;
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  constexpr size_t delay = 30;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  auto server = std::make_shared<llgc::pattern::publisher::PublisherDirect<
      llgc::protobuf::test::Direct>>();

  auto subscriber = std::make_shared<Subscriber>(1);

  auto connector = std::make_shared<
      llgc::pattern::publisher::ConnectorDirect<llgc::protobuf::test::Direct>>(
      subscriber, server);
  assert(subscriber->SetConnector(connector));

  llgc::pattern::publisher::test::Publisher::All<
      llgc::protobuf::test::Direct, Subscriber,
      llgc::pattern::publisher::PublisherDirect<llgc::protobuf::test::Direct>>(
      subscriber.get(), server.get(), delay);

  // connector must be free. Either server->Stop never stop.
  connector.reset();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
