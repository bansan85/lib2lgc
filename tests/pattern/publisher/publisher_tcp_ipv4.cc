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

// Test ipv4 publisher

#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h>
#include <2lgc/pattern/publisher/connector_subscriber_tcp.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>  // IWYU pragma: keep
#include <google/protobuf/stubs/common.h>
#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include "publisher_all.h"
#include "tcp.pb.h"

#ifdef TEMPLATE_CLASS
#include <2lgc/config.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_open_ssl.h>
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_tcp.h>
#include <2lgc/pattern/publisher/subscriber_local.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>

#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_publisher_tcp.cc>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.cc>
#include <2lgc/pattern/publisher/connector_subscriber.cc>
#include <2lgc/pattern/publisher/connector_subscriber_tcp.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/publisher_ip.cc>
#include <2lgc/pattern/publisher/publisher_tcp.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.cc>
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_open_ssl.cc>
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_tcp.cc>
#include <2lgc/pattern/publisher/subscriber_local.cc>
#include <2lgc/pattern/publisher/subscriber_server_tcp.cc>
#include "publisher_all.cc"

template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorPublisherTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorSubscriber<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorSubscriberTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::SubscriberServerTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherInterface<
    llgc::protobuf::test::Tcp,
    std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<
        llgc::protobuf::test::Tcp>>>;
template class llgc::pattern::publisher::PublisherIp<llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::StrategyPublisherTcpLinuxTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherTcpLinux<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherTcpLinuxIpv4<
    llgc::protobuf::test::Tcp>;

template class llgc::pattern::publisher::SubscriberLocal<
    llgc::protobuf::test::Tcp>;
#endif

int main(int /* argc */, char * /* argv */ [])  // NS
{
  constexpr size_t delay = 30;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  EXECUTE_AND_ABORT(std::cout, llgc::net::Linux::DisableSigPipe());

  auto server =
      std::make_shared<llgc::pattern::publisher::PublisherTcpLinuxIpv4<
          llgc::protobuf::test::Tcp>>(8888);
  EXECUTE_AND_ABORT(std::cout, server->Listen());
  EXECUTE_AND_ABORT(std::cout, server->Wait());

  auto subscriber = std::make_shared<
      llgc::pattern::publisher::test::Subscriber<llgc::protobuf::test::Tcp>>(1);

  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
          llgc::protobuf::test::Tcp>>(subscriber, "127.0.0.1", 8888);
  EXECUTE_AND_ABORT(std::cout, subscriber->SetConnector(connector));

  llgc::pattern::publisher::test::Publisher::All<
      llgc::protobuf::test::Tcp,
      llgc::pattern::publisher::test::Subscriber<llgc::protobuf::test::Tcp>,
      llgc::pattern::publisher::PublisherTcpLinuxIpv4<
          llgc::protobuf::test::Tcp>>(subscriber.get(), server.get(), delay);

  connector.reset();
  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
