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
  grpc::Status Talk(grpc::ServerContext* context, const llgc::protobuf::test::Rpc* request, llgc::protobuf::test::Rpc* response) override
  {
    /*
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    */
    // No response
    return grpc::Status::OK;
  }
};


int main(int /* argc */, char* /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  auto service = std::make_unique<GreeterServiceImpl>();
  auto server = std::make_unique<llgc::pattern::publisher::PublisherGrpc<llgc::protobuf::test::Rpc>>(8890, std::move(service));

  assert(server->Listen());
  assert(server->Wait());

  server->Stop();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
