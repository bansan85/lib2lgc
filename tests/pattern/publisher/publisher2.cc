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

#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_client_tcp.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_server_tcp.h>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h>
#include <2lgc/pattern/publisher/subscriber_direct.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>
#include <actions_tcp.pb.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include <2lgc/pattern/publisher/connector_client_tcp.cc>
#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_server_tcp.cc>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.cc>
#include <2lgc/pattern/publisher/publisher.cc>
#include <2lgc/pattern/publisher/publisher_tcp.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.cc>
#include <2lgc/pattern/publisher/subscriber_direct.cc>
#include <2lgc/pattern/publisher/subscriber_server_tcp.cc>

template class llgc::pattern::publisher::ConnectorInterface<msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorServerTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorServerTcpIpv4<
    msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorClientTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::SubscriberDirect<msg::ActionsTcp>;
template class llgc::pattern::publisher::SubscriberServerTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::PublisherTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::PublisherTcpLinux<msg::ActionsTcp>;
template class llgc::pattern::publisher::PublisherTcpLinuxIpv4<msg::ActionsTcp>;

/**
 * @brief Simple implementation of a direct subscriber.
 */
class SubscriberBase final
    : public llgc::pattern::publisher::SubscriberDirect<msg::ActionsTcp>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id id of the subscriber.
   */
  explicit SubscriberBase(uint32_t id) : SubscriberDirect(id), value(0) {}
  /**
   * @brief Default constructor for debug.
   */
  ~SubscriberBase() override { std::cout << "DEAD IN HELL" << std::endl; }

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberBase(SubscriberBase&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberBase(SubscriberBase const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberBase& operator=(SubscriberBase&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberBase& operator=(SubscriberBase const& other) & = delete;
  /**
   * @brief Receive message from publisher.
   *
   * @param[in] message message from the publisher in protobuf format.
   */

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] message message from the publisher in protobuf format.
   */
  bool Listen(const msg::ActionsTcp& message) override
  {
    std::cout << "LISTEN" << std::endl;
    for (int i = 0; i < message.action_size(); i++)
    {
      decltype(message.action(i)) action = message.action(i);

      switch (action.data_case())
      {
        case msg::ActionTcp::DataCase::kTest:
        {
          value++;
          std::cout << "value++" << std::endl;
          break;
        }
        case msg::ActionTcp::DataCase::DATA_NOT_SET:
        case msg::ActionTcp::DataCase::kAddSubscriber:
        case msg::ActionTcp::DataCase::kRemoveSubscriber:
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
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  llgc::net::Linux::DisableSigPipe();

  std::shared_ptr<
      llgc::pattern::publisher::PublisherTcpLinuxIpv4<msg::ActionsTcp>>
      server = std::make_shared<
          llgc::pattern::publisher::PublisherTcpLinuxIpv4<msg::ActionsTcp>>(
          8888);
  assert(server->Listen());
  assert(server->Wait());

  std::shared_ptr<SubscriberBase> subscriber =
      std::make_shared<SubscriberBase>(1);

  std::shared_ptr<
      llgc::pattern::publisher::ConnectorServerTcpIpv4<msg::ActionsTcp>>
      connector = std::make_shared<
          llgc::pattern::publisher::ConnectorServerTcpIpv4<msg::ActionsTcp>>(
          subscriber, "127.0.0.1", 8888);

  // Add them to the server.
  assert(connector->AddSubscriber(msg::ActionTcp::DataCase::kTest));

  // Base test case.
  assert(subscriber->value == 0);

  // Check first message.
  msg::ActionsTcp actions = msg::ActionsTcp();
  msg::ActionTcp* action = actions.add_action();
  std::unique_ptr<msg::ActionTcp_Test> action_test =
      std::make_unique<msg::ActionTcp_Test>();
  action->set_allocated_test(action_test.release());
  std::string action_in_string;
  actions.SerializeToString(&action_in_string);
  assert(connector->Send(action_in_string));
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  do
  {
    end = std::chrono::system_clock::now();
    assert(
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count()) < 1000);
  } while (subscriber->value != 1);

  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
