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
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>
#include <iostream>
#include <string>

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

/** \class llgc::pattern::publisher::SubscriberServerTcp
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberServerTcp::SubscriberServerTcp(int socket)
 * \brief Default constructor.
 * \param[in] socket Socket to communicate with server.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberServerTcp::~SubscriberServerTcp()
 * \brief Default virtual destructor.
 */

template <typename T>
bool llgc::pattern::publisher::SubscriberServerTcp<T>::Listen(const T &messages)
{
  std::string message_socket;

  BUGLIB(std::cout, messages.SerializeToString(&message_socket), false,
         "protobuf");
  BUGCONT(std::cout, llgc::net::Linux::Send(socket_, message_socket), false);

  return true;
}

template <typename T>
bool llgc::pattern::publisher::SubscriberServerTcp<T>::Equals(
    const SubscriberInterface<T> &connector) const
{
  const auto *subscriber_cast =
      dynamic_cast<const SubscriberServerTcp<T> *>(&connector);

  // Not the same type.
  if (subscriber_cast == nullptr)
  {
    return false;
  }

  return subscriber_cast->socket_ == socket_;
}

/** \fn llgc::pattern::publisher::SubscriberServerTcp::SubscriberServerTcp(SubscriberServerTcp&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberServerTcp::SubscriberServerTcp(SubscriberServerTcp const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn SubscriberServerTcp& llgc::pattern::publisher::SubscriberServerTcp::operator=(SubscriberServerTcp&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn SubscriberServerTcp& llgc::pattern::publisher::SubscriberServerTcp::operator=(SubscriberServerTcp const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \var llgc::pattern::publisher::SubscriberServerTcp::socket_
 * \brief The id of the connector.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
