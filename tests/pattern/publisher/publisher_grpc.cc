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

// Test gRPC publisher

#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_grpc.h>
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <2lgc/pattern/publisher/connector_subscriber_grpc.h>
#include <2lgc/pattern/publisher/publisher_grpc.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <2lgc/pattern/publisher/subscriber_server_grpc.h>
#include <google/protobuf/stubs/common.h>
#include <grpcpp/impl/codegen/status.h>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <map>
#include <memory>
#include <thread>
#include "publisher_all.h"
#include "rpc.grpc.pb.h"
#include "rpc.pb.h"

#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_publisher_grpc.cc>
#include <2lgc/pattern/publisher/connector_subscriber.cc>
#include <2lgc/pattern/publisher/connector_subscriber_grpc.cc>
#include <2lgc/pattern/publisher/publisher_grpc.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/publisher_ip.cc>
#include <2lgc/pattern/publisher/subscriber_server_grpc.cc>
#include "publisher_all.cc"

template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::ConnectorPublisherGrpc<
    llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter>;
template class llgc::pattern::publisher::ConnectorSubscriber<
    llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::ConnectorSubscriberGrpc<
    llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherInterface<
    llgc::protobuf::test::Rpc,
    std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<
        llgc::protobuf::test::Rpc>>>;
template class llgc::pattern::publisher::PublisherIp<llgc::protobuf::test::Rpc>;
template class llgc::pattern::publisher::PublisherGrpc<
    llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter::Service>;
template class llgc::pattern::publisher::SubscriberServerGrpc<
    llgc::protobuf::test::Rpc>;

int main(int /* argc */, char* /* argv */ [])  // NS
{
  constexpr size_t delay = 30;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  auto server = std::make_unique<llgc::pattern::publisher::PublisherGrpc<
      llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter::Service>>(8890);
  assert(server->Listen());
  assert(server->Wait());
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  auto subscriber = std::make_shared<
      llgc::pattern::publisher::test::Subscriber<llgc::protobuf::test::Rpc>>(1);
  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherGrpc<
          llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter>>(
          subscriber, "127.0.0.1", 8890);
  assert(subscriber->SetConnector(connector));

  llgc::pattern::publisher::test::Publisher::All<
      llgc::protobuf::test::Rpc,
      llgc::pattern::publisher::test::Subscriber<llgc::protobuf::test::Rpc>,
      llgc::pattern::publisher::PublisherGrpc<
          llgc::protobuf::test::Rpc, llgc::protobuf::test::Greeter::Service>>(
      subscriber.get(), server.get(), delay);

  // connector must be free. Either server->Stop never stop.
  connector.reset();
  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
