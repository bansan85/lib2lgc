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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_CC_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_CC_

#include <2lgc/compat.h>
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

/** \class llgc::pattern::publisher::ConnectorPublisherTcp
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber The subscriber.
 * \param[in] ip The IP of the server.
 * \param[in] port The port of the server.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorPublisherTcp<T>::ConnectorPublisherTcp(
    std::shared_ptr<SubscriberInterface<T>> subscriber, std::string ip,
    uint16_t port)
    : ConnectorInterface<T>(subscriber),
      socket_(-1),
      ip_(std::move(ip)),
      port_(port),
      disposing_(false)
{
}

/** \fn llgc::pattern::publisher::ConnectorPublisherTcp::ConnectorPublisherTcp(ConnectorPublisherTcp &&other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorPublisherTcp::ConnectorPublisherTcp(ConnectorPublisherTcp const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorPublisherTcp & llgc::pattern::publisher::ConnectorPublisherTcp::operator=(ConnectorPublisherTcp &&other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorPublisherTcp & llgc::pattern::publisher::ConnectorPublisherTcp::operator=(ConnectorPublisherTcp const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/// \brief Default virtual destructor.
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorPublisherTcp<T>::~ConnectorPublisherTcp()
{
  // Properly stop recv and close file descriptor.
  disposing_ = true;
  if (receiver_.joinable())
  {
    receiver_.join();
  }
}

/** \brief Send message to the publisher.
 * \param[in] message The protobuf message.
 * \return true if no problem.
 * \dotfile pattern/publisher/publisher_tcp_send_message.dot
 */
template <typename T>
INLINE_TEMPLATE
bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::Send(const T &message)
{
  BUGCONT(std::cout, Connect(), false);

  std::string message_in_string;
  BUGLIB(std::cout, message.SerializeToString(&message_in_string), false,
         "protobuf");
  BUGCONT(std::cout, llgc::net::Linux::Send(socket_, message_in_string), false);
  return true;
}

template <typename T>
INLINE_TEMPLATE
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
INLINE_TEMPLATE
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

/** \brief The function used by the thread that receive message from server.
 * Need to be public so thread can use it. Protected is not possible.
 */
template <typename T>
INLINE_TEMPLATE
void llgc::pattern::publisher::ConnectorPublisherTcp<T>::Receiver()
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(&socket_);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct pollfd fd;  // NOLINT(hicpp-member-init)
  fd.fd = socket_;
  fd.events = POLLIN;

  do
  {
    int retval = poll(&fd, 1, 50);

    // Problem: stop the thread.
    BUGCRIT(std::cout, retval != -1, ,
            "Client " + std::to_string(socket_) + ", poll failed. Errno " +
                std::to_string(errno) + ".\n");
    if (retval != 0)
    {
      char client_message[1500];

      ssize_t read_size =
          recv(socket_, client_message, sizeof(client_message), 0);

      BUGCRIT(std::cout, read_size != -1, ,
              "Client " + std::to_string(socket_) +
                  " recv failed. Close connection. Errno " +
                  std::to_string(errno) + ".\n");

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

/** \var llgc::pattern::publisher::ConnectorPublisherTcp::receiver_
 * \brief Thread that listen the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::socket_
 * \brief Socket to the server.
 *
 *
 * \fn bool llgc::pattern::publisher::ConnectorPublisherTcp::Connect()
 * \brief Start connection with server.
 * \return true if no problem.
 *
 *
 * \fn const std::string & llgc::pattern::publisher::ConnectorPublisherTcp::GetIp() const
 * \brief Get the ip of the connector.
 * \return The return value;
 *
 *
 * \fn uint16_t llgc::pattern::publisher::ConnectorPublisherTcp::GetPort() const
 * \brief Get the port of the connector.
 * \return The return value.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::ip_
 * \brief The IP of the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::port_
 * \brief The port of the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::disposing_
 * \brief If thread in trying to stop.
 */

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
