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

#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <2lgc/poco/net.pb.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
llgc::pattern::publisher::ConnectorPublisherTcp<T>::ConnectorPublisherTcp(
    std::shared_ptr<SubscriberInterface<T>> subscriber, std::string ip,
    uint16_t port)
    : ConnectorInterface<T>(subscriber),
      ip_(std::move(ip)),
      port_(port),
      socket_(-1),
      disposing_(false)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorPublisherTcp<T>::~ConnectorPublisherTcp()
{
  // Properly stop recv and close file descriptor.
  disposing_ = true;
  receiver_.join();
}

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::Equals(
    const ConnectorInterface<T> &connector) const
{
  const auto *connector_direct =
      dynamic_cast<const ConnectorPublisherTcp<T> *>(&connector);

  if (connector_direct == nullptr)
  {
    return false;
  }

  return this->subscriber_->Equals(*connector.GetSubscriber());
}

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::AddSubscriber(
    uint32_t id_message)
{
  BUGCONT(Connect(), false);

  T actions;
  decltype(std::declval<T>().add_action()) action = actions.add_action();
  std::unique_ptr<msg::net::AddSubscriber> action_add =
      std::make_unique<msg::net::AddSubscriber>();
  action_add->set_id_message(id_message);
  action->set_allocated_add_subscriber(action_add.release());
  std::string action_in_string;
  BUGLIB(actions.SerializeToString(&action_in_string), false, "protobuf");
  BUGCONT(Send(action_in_string), false);

  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::Send(
    const std::string &message)
{
  std::cout << "ConnectorPublisherTcp<T>::Send" << std::endl;
  std::cout << "Client " << socket_ << " -> serveur " << message.length()
            << std::endl;
  BUGCONT(llgc::net::Linux::Send(socket_, message), false);
  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::RemoveSubscriber(
    uint32_t id_message)
{
  BUGCONT(Connect(), false);

  T actions;
  decltype(std::declval<T>().add_action()) action = actions.add_action();
  std::unique_ptr<msg::net::RemoveSubscriber> action_add =
      std::make_unique<msg::net::RemoveSubscriber>();
  action_add->set_id_message(id_message);
  action->set_allocated_remove_subscriber(action_add.release());
  std::string action_in_string;
  BUGLIB(actions.SerializeToString(&action_in_string), false, "protobuf");
  BUGCONT(Send(action_in_string), false);

  return true;
}

template <typename T>
void llgc::pattern::publisher::ConnectorPublisherTcp<T>::Receiver()
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(&socket_);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = socket_;
  fd.events = POLLIN;
  int retval;  // NS

  std::cout << "Client " << socket_ << " Start" << std::endl;

  do
  {
    retval = poll(&fd, 1, 50);

    // Problem: stop the thread.
    BUGCRIT(retval != -1, , "Client %, poll failed. Errno %.\n", socket_,
            errno);
    if (retval != 0)
    {
      char client_message[1500];

      ssize_t read_size =
          recv(socket_, client_message, sizeof(client_message), 0);

      BUGCRIT(read_size != -1, ,
              "Client % recv failed. Close connection. Errno %.\n", socket_,
              errno);

      if (read_size == 0)
      {
        continue;
      }

      std::cout << "Client " << socket_ << " recv" << std::endl;
      T message;
      std::string client_string(client_message, static_cast<size_t>(read_size));
      BUGLIB(message.ParseFromString(client_string), , "protobuf");

      BUGCONT(this->subscriber_->Listen(message), );
    }
  } while (!disposing_);

  std::cout << "Client " << socket_ << " End" << std::endl;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
