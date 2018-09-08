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

#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/publisher_direct.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <map>
#include <memory>
#include "direct.pb.h"
#include "publisher_all.h"

#ifdef TEMPLATE_CLASS
#include <2lgc/config.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/subscriber_local.h>

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
#endif

int main(int /* argc */, char* /* argv */ [])  // NS
{
  constexpr size_t delay = 30;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  auto server = std::make_shared<llgc::pattern::publisher::PublisherDirect<
      llgc::protobuf::test::Direct>>();

  auto subscriber = std::make_shared<
      llgc::pattern::publisher::test::Subscriber<llgc::protobuf::test::Direct>>(
      1);

  auto connector = std::make_shared<
      llgc::pattern::publisher::ConnectorDirect<llgc::protobuf::test::Direct>>(
      subscriber, server);
  EXECUTE_AND_ABORT(std::cout, subscriber->SetConnector(connector));

  llgc::pattern::publisher::test::Publisher::All<
      llgc::protobuf::test::Direct,
      llgc::pattern::publisher::test::Subscriber<llgc::protobuf::test::Direct>,
      llgc::pattern::publisher::PublisherDirect<llgc::protobuf::test::Direct>>(
      subscriber.get(), server.get(), delay);

  // connector must be free. Either server->Stop never stop.
  connector.reset();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
