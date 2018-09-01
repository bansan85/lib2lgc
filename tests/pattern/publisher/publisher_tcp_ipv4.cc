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

#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h>
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <2lgc/pattern/publisher/connector_subscriber_tcp.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h>
#include <2lgc/pattern/publisher/subscriber_local.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include "publisher_all.h"
#include "tcp.pb.h"

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
template class llgc::pattern::publisher::SubscriberLocal<
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
template class llgc::pattern::publisher::PublisherTcpLinux<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherTcpLinuxIpv4<
    llgc::protobuf::test::Tcp>;

/**
 * @brief Simple implementation of a direct subscriber.
 */
class Subscriber final : public llgc::pattern::publisher::SubscriberLocal<
                             llgc::protobuf::test::Tcp>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id Id of the subscriber.
   */
  explicit Subscriber(uint32_t id) : SubscriberLocal(id), value(0) {}

  /**
   * @brief Default destructor.
   */
  ~Subscriber() override = default;

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  Subscriber(Subscriber&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  Subscriber(Subscriber const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  Subscriber& operator=(Subscriber&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  Subscriber& operator=(Subscriber const& other) & = delete;

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] messages Message from the publisher in protobuf format.
   */
  bool Listen(const llgc::protobuf::test::Tcp& messages) override
  {
    for (int i = 0; i < messages.msg_size(); i++)
    {
      const auto& message = messages.msg(i);

      switch (message.data_case())
      {
        case llgc::protobuf::test::Tcp_Msg::DataCase::kTest:
        {
          value++;
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

  /**
   * @brief value for test.
   */
  size_t value;
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  constexpr size_t delay = 30;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  assert(llgc::net::Linux::DisableSigPipe());

  auto server =
      std::make_shared<llgc::pattern::publisher::PublisherTcpLinuxIpv4<
          llgc::protobuf::test::Tcp>>(8888);
  assert(server->Listen());
  assert(server->Wait());

  auto subscriber = std::make_shared<Subscriber>(1);

  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
          llgc::protobuf::test::Tcp>>(subscriber, "127.0.0.1", 8888);
  assert(subscriber->SetConnector(connector));

  llgc::pattern::publisher::test::Publisher::All<
      llgc::protobuf::test::Tcp, Subscriber,
      llgc::pattern::publisher::PublisherTcpLinuxIpv4<
          llgc::protobuf::test::Tcp>>(subscriber.get(), server.get(), delay);

  connector.reset();
  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
