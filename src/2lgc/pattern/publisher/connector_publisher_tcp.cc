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
#include <2lgc/poco/pattern_publisher.pb.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <iostream>
#include <memory>
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
  if (receiver_.joinable())
  {
    receiver_.join();
  }
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
  BUGCONT(std::cout, Connect(), false);

  T messages;
  auto message = messages.add_msg();
  auto add =
      std::make_unique<llgc::protobuf::pattern::publisher::AddSubscriber>();
  add->set_id_message(id_message);
  message->set_allocated_add_subscriber(add.release());
  BUGCONT(std::cout, Send(messages), false);

  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::Send(
    const T &message)
{
  BUGCONT(std::cout, Connect(), false);

  std::string message_in_string;
  BUGLIB(std::cout, message.SerializeToString(&message_in_string), false, "protobuf");
  BUGCONT(std::cout, llgc::net::Linux::Send(socket_, message_in_string), false);
  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::RemoveSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout, Connect(), false);

  T messages;
  auto message = messages.add_msg();
  auto message_add =
      std::make_unique<llgc::protobuf::pattern::publisher::RemoveSubscriber>();
  message_add->set_id_message(id_message);
  message->set_allocated_remove_subscriber(message_add.release());
  BUGCONT(std::cout, Send(messages), false);

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

  do
  {
    retval = poll(&fd, 1, 50);

    // Problem: stop the thread.
    BUGCRIT(std::cout, retval != -1, ,
            "Client " << socket_ << ", poll failed. Errno " << errno << ".\n");
    if (retval != 0)
    {
      char client_message[1500];

      ssize_t read_size =
          recv(socket_, client_message, sizeof(client_message), 0);

      BUGCRIT(std::cout, read_size != -1, ,
              "Client " << socket_ << " recv failed. Close connection. Errno "
                        << errno << ".\n");

      if (read_size == 0)
      {
        continue;
      }

      T message;
      std::string client_string(client_message, static_cast<size_t>(read_size));
      BUGLIB(std::cout, message.ParseFromString(client_string), , "protobuf");

      BUGCONT(std::cout, this->subscriber_->Listen(message), );
    }
  } while (!disposing_);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
