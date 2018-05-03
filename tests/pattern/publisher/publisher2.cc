/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <2lgc/net/tcp_server.h>
#include <2lgc/net/tcp_server_linux.h>
#include <2lgc/net/tcp_server_linux_ipv4.h>
#include <2lgc/pattern/publisher/connector_client_tcp.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_server_tcp.h>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.h>
#include <2lgc/pattern/publisher/subscriber_direct.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>
#include <2lgc/poco/net.pb.h>
#include <actions_tcp.pb.h>
#include <google/protobuf/stubs/common.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include <2lgc/net/tcp_server.cc>
#include <2lgc/net/tcp_server_linux.cc>
#include <2lgc/net/tcp_server_linux_ipv4.cc>
#include <2lgc/pattern/publisher/connector_client_tcp.cc>
#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_server_tcp.cc>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.cc>
#include <2lgc/pattern/publisher/publisher.cc>
#include <2lgc/pattern/publisher/subscriber_direct.cc>
#include <2lgc/pattern/publisher/subscriber_server_tcp.cc>

template class llgc::pattern::publisher::ConnectorInterface<msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorServerTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorServerTcpIpv4<
    msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorClientTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::SubscriberDirect<msg::ActionsTcp>;
template class llgc::pattern::publisher::SubscriberServerTcp<msg::ActionsTcp>;
template class llgc::net::TcpServer<msg::ActionsTcp>;
template class llgc::net::TcpServerLinux<msg::ActionsTcp>;
template class llgc::net::TcpServerLinuxIpv4<msg::ActionsTcp>;

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
  void Listen(const msg::ActionsTcp& message) override
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
  }

  /**
   * @brief value for test.
   */
  size_t value;
};

std::map<msg::ActionTcp::DataCase,
         std::function<void(llgc::net::TcpServer<msg::ActionsTcp>*, int,
                            const msg::ActionTcp&)>>
    action_server;

void WaitServer(llgc::net::TcpServer<msg::ActionsTcp>* server, int socket)
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = socket;
  fd.events = POLLIN;
  int retval;  // NS

  std::cout << "Server Client " << socket << " Start" << std::endl;

  do
  {
    retval = poll(&fd, 1, 1000);

    // Problem: stop the thread.
    if (retval == -1)
    {
      std::cout << "Server Client " << socket << " Stop" << std::endl;
      server->Stop();
    }
    else if (retval != 0)
    {
      char client_message[1500];

      ssize_t read_size =
          recv(socket, client_message, sizeof(client_message), 0);

      if (read_size == -1 || read_size == 0)
      {
        std::cout << "Server Client " << socket << " Stop" << std::endl;
        server->Stop();
        break;
      }
      std::cout << "Server Client " << socket << " Talk" << std::endl;
      msg::ActionsTcp message;

      std::string client_string(client_message, static_cast<size_t>(read_size));
      assert(message.ParseFromString(client_string));

      for (int i = 0; i < message.action_size(); i++)
      {
        const msg::ActionTcp& action_tcp = message.action(i);

        auto it = action_server.find(action_tcp.data_case());

        if (it != action_server.end())
        {
          (*it).second(server, socket, action_tcp);
        }
      }

      server->Forward(client_string);
    }
  } while (!server->IsStopping());

  close(socket);

  std::cout << "Server Client " << socket << " End" << std::endl;
}

void AddSubscriberFct(llgc::net::TcpServer<msg::ActionsTcp>* server, int socket,
                      const msg::ActionTcp& action_tcp)
{
  assert(action_tcp.has_add_subscriber());
  std::shared_ptr<
      llgc::pattern::publisher::SubscriberServerTcp<msg::ActionsTcp>>
      subscriber = std::make_shared<
          llgc::pattern::publisher::SubscriberServerTcp<msg::ActionsTcp>>(
          socket);
  std::shared_ptr<llgc::pattern::publisher::ConnectorClientTcp<msg::ActionsTcp>>
      connector = std::make_shared<
          llgc::pattern::publisher::ConnectorClientTcp<msg::ActionsTcp>>(
          subscriber, socket);
  assert(server->AddSubscriber(action_tcp.add_subscriber().id_message(),
                               connector));
}

void RemoveSubscriberFct(llgc::net::TcpServer<msg::ActionsTcp>* /*server*/,
                         int /*socket*/, const msg::ActionTcp& /*action_tcp*/)
{
}

int main(int /* argc */, char* /* argv */ [])  // NS
{
  return 0;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  //  llgc::net::Linux::DisableSigPipe();

  action_server[msg::ActionTcp::DataCase::kAddSubscriber] = &AddSubscriberFct;
  action_server[msg::ActionTcp::DataCase::kRemoveSubscriber] =
      &RemoveSubscriberFct;

  std::shared_ptr<llgc::net::TcpServerLinuxIpv4<msg::ActionsTcp>> server =
      std::make_shared<llgc::net::TcpServerLinuxIpv4<msg::ActionsTcp>>(8888);
  assert(server->Listen());
  assert(server->Wait(&WaitServer));
  // Wait to be sure that accept4 is waiting.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
  while (subscriber->value != 1)
  {
  }

  server->Stop();
  server->JoinWait();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
