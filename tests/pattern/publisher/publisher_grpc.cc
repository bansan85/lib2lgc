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

// Test ipv6 publisher

#include <google/protobuf/stubs/common.h>
#include <2lgc/pattern/publisher/publisher_grpc.h>
#include "rpc.pb.h"
#include "rpc.grpc.pb.h"
#include <cassert>
#include <2lgc/pattern/publisher/subscriber.h>
#include <2lgc/pattern/publisher/connector_publisher_grpc.h>

#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/subscriber.cc>
#include <2lgc/pattern/publisher/publisher_ip.cc>
#include <2lgc/pattern/publisher/publisher_grpc.cc>
#include <2lgc/pattern/publisher/connector_publisher_grpc.cc>
#include <2lgc/pattern/publisher/connector_subscriber_grpc.cc>
#include <2lgc/pattern/publisher/subscriber_server_grpc.cc>

template class llgc::pattern::publisher::ConnectorInterface<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherInterface<llgc::protobuf::test::Rpc, std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<llgc::protobuf::test::Rpc>>>;
template class llgc::pattern::publisher::Subscriber<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherIp<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherGrpc<llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter::Service>;
template class llgc::pattern::publisher::ConnectorPublisherGrpc<llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter>;
template class llgc::pattern::publisher::ConnectorSubscriberGrpc<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::SubscriberServerGrpc<llgc::protobuf::test::Rpc>;

class SubscriberBase final : public llgc::pattern::publisher::Subscriber<
                                 llgc::protobuf::test::Rpc>
{
 public:
  explicit SubscriberBase(uint32_t id) : Subscriber(id), value(0) {}
  SubscriberBase(SubscriberBase&& other) = delete;
  SubscriberBase(SubscriberBase const& other) = delete;
  SubscriberBase& operator=(SubscriberBase&& other) & = delete;
  SubscriberBase& operator=(SubscriberBase const& other) & = delete;

  bool Listen(const llgc::protobuf::test::Rpc& messages) override
  {
    for (int i = 0; i < messages.msg_size(); i++)
    {
      const auto& message = messages.msg(i);

      switch (message.data_case())
      {
        case llgc::protobuf::test::Rpc_Msg::DataCase::kTest:
        {
          value++;
          break;
        }
        case llgc::protobuf::test::Rpc_Msg::DataCase::DATA_NOT_SET:
        case llgc::protobuf::test::Rpc_Msg::DataCase::kAddSubscriber:
        case llgc::protobuf::test::Rpc_Msg::DataCase::kRemoveSubscriber:
        default:
          assert(false);
      }
    }
    return true;
  }
  size_t value;
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  auto server = std::make_unique<llgc::pattern::publisher::PublisherGrpc<llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter::Service>>(8890);
  assert(server->Listen());
  assert(server->Wait());

  auto subscriber = std::make_shared<SubscriberBase>(1);
  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherGrpc<
          llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter>>(subscriber, "127.0.0.1", 8890);

  assert(connector->AddSubscriber(llgc::protobuf::test::Rpc_Msg::DataCase::kTest));

  assert(subscriber->value == 0);

  // Check first message.
  llgc::protobuf::test::Rpc messages;
  auto message = messages.add_msg();
  auto message_test = std::make_unique<llgc::protobuf::test::Rpc_Msg_Test>();
  message->set_allocated_test(message_test.release());
  assert(connector->Send(messages));
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  do
  {
    end = std::chrono::system_clock::now();
    assert(static_cast<size_t>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) < 1000);
  } while (subscriber->value != 1);

  connector.reset();
  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
