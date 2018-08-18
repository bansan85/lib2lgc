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
#include <2lgc/pattern/publisher/connector_subscriber_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h>
#include <2lgc/pattern/publisher/subscriber.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>
#include <google/protobuf/stubs/common.h>
#include <tcp.pb.h>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <thread>

#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_publisher_tcp.cc>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.cc>
#include <2lgc/pattern/publisher/connector_subscriber_tcp.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/publisher_ip.cc>
#include <2lgc/pattern/publisher/publisher_tcp.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.cc>
#include <2lgc/pattern/publisher/subscriber.cc>
#include <2lgc/pattern/publisher/subscriber_server_tcp.cc>

template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorPublisherTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorSubscriberTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::Subscriber<llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::SubscriberServerTcp<
    llgc::protobuf::test::Tcp>;
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
class SubscriberBase final : public llgc::pattern::publisher::Subscriber<
                                 llgc::protobuf::test::Tcp>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id Id of the subscriber.
   */
  explicit SubscriberBase(uint32_t id) : Subscriber(id), value(0) {}

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

static void WaitUpToOneSecond(std::function<bool()> test)
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  do
  {
    end = std::chrono::system_clock::now();
    assert(
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count()) < 1000);
  } while (!test());
}

int main(int /* argc */, char* /* argv */ [])  // NS
{
  constexpr size_t delay = 30;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  llgc::net::Linux::DisableSigPipe();

  auto server =
      std::make_shared<llgc::pattern::publisher::PublisherTcpLinuxIpv4<
          llgc::protobuf::test::Tcp>>(8888);
  assert(server->Listen());
  assert(server->Wait());

  auto subscriber = std::make_shared<SubscriberBase>(1);

  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherTcpIpv4<
          llgc::protobuf::test::Tcp>>(subscriber, "127.0.0.1", 8888);

  // Add them to the server.
  assert(
      connector->AddSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  // Base test case.
  assert(subscriber->value == 0);

  // Check first message.
  llgc::protobuf::test::Tcp messages;
  auto message = messages.add_msg();
  auto message_test = std::make_unique<llgc::protobuf::test::Tcp_Msg_Test>();
  message->set_allocated_test(message_test.release());
  assert(connector->Send(messages));
  WaitUpToOneSecond([&subscriber]() { return subscriber->value == 1; });

  // Test lock forward.
  subscriber->value = 0;
  {
    llgc::utils::thread::CountLock<size_t> lock = server->LockForward();
    assert(connector->Send(messages));
    // Wait one second to be sure that the message is not send.
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    assert(subscriber->value == 0);
  }
  WaitUpToOneSecond([&subscriber]() { return subscriber->value == 1; });

  // Remove the first subscriber.
  subscriber->value = 0;
  assert(connector->RemoveSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(connector->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 0);

  // Double insert
  assert(connector->AddSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(connector->AddSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(connector->Send(messages));
  WaitUpToOneSecond([&subscriber]() { return subscriber->value == 2; });
  assert(connector->RemoveSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(connector->Send(messages));
  WaitUpToOneSecond([&subscriber]() { return subscriber->value == 3; });
  assert(connector->RemoveSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(connector->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 3);
  assert(!server->GetOptionFailAlreadySubscribed());
  server->SetOptionFailAlreadySubscribed(true);
  assert(connector->AddSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  // Here, AddSubscriber will not failed because the TCP server can't return a value.
  assert(connector->AddSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(connector->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 4);

  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
