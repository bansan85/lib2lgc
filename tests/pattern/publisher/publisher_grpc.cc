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

#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/publisher_ip.cc>
#include <2lgc/pattern/publisher/publisher_grpc.cc>

template class llgc::pattern::publisher::ConnectorInterface<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherInterface<llgc::protobuf::test::Rpc, std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<llgc::protobuf::test::Rpc>>>;
template class llgc::pattern::publisher::PublisherIp<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherGrpc<llgc::protobuf::test::Rpc>;


class GreeterServiceImpl final : public llgc::protobuf::test::Greeter::Service
{
  grpc::Status Talk(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::llgc::protobuf::test::Rpc, ::llgc::protobuf::test::Rpc>* stream) override
  {
    /*
    llgc::protobuf::test::Rpc message;
    while (stream->Read(&message)) {
      stream->Write(message);
    }
    */

    // No response for now
    return grpc::Status::OK;
  }
};

/*
class SubscriberBase final : public llgc::pattern::publisher::Subscriber<
                                 llgc::protobuf::test::Tcp>
{
 public:
  explicit SubscriberBase(uint32_t id) : Subscriber(id), value(0) {}
  ~SubscriberBase() override { std::cout << "DEAD IN HELL" << std::endl; }
  SubscriberBase(SubscriberBase&& other) = delete;
  SubscriberBase(SubscriberBase const& other) = delete;
  SubscriberBase& operator=(SubscriberBase&& other) & = delete;
  SubscriberBase& operator=(SubscriberBase const& other) & = delete;
  bool Listen(const llgc::protobuf::test::Tcp& messages) override
  {
    std::cout << "LISTEN" << std::endl;
    for (int i = 0; i < messages.msg_size(); i++)
    {
      const auto& message = messages.msg(i);

      switch (message.data_case())
      {
        case llgc::protobuf::test::Tcp_Msg::DataCase::kTest:
        {
          value++;
          std::cout << "value++" << std::endl;
          break;
        }
        case llgc::protobuf::test::Tcp_Msg::DataCase::DATA_NOT_SET:
        case llgc::protobuf::test::Tcp_Msg::DataCase::kAddSubscriber:
        case llgc::protobuf::test::Tcp_Msg::DataCase::kRemoveSubscriber:
        default:
          assert(false);
      }
    }
    return true;
  }
  size_t value;
};
*/

int main(int /* argc */, char* /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  auto service = std::make_unique<GreeterServiceImpl>();
  auto server = std::make_unique<llgc::pattern::publisher::PublisherGrpc<llgc::protobuf::test::Rpc>>(8890, std::move(service));
  /*
  auto subscriber = std::make_shared<SubscriberBase>(1);
  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherGrpc<
          llgc::protobuf::test::Rpc>>(subscriber, "127.0.0.1", 8890);
          */

  assert(server->Listen());
  assert(server->Wait());

  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
