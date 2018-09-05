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

#ifndef PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_TCP_CC_
#define PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_TCP_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <2lgc/pattern/publisher/connector_subscriber_tcp.h>
#include <iostream>
#include <string>

/** \class llgc::pattern::publisher::ConnectorSubscriberTcp
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber Subscriber to communicate with client.
 * \param[in] socket_fd Socket to communicate with client.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorSubscriberTcp<T>::ConnectorSubscriberTcp(
    std::shared_ptr<SubscriberInterface<T>> subscriber, int socket_fd)
    : ConnectorSubscriber<T>(subscriber), socket_(socket_fd)
{
}

/** \fn llgc::pattern::publisher::ConnectorSubscriberTcp::~ConnectorSubscriberTcp()
 * \brief Default virtual destructor.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorSubscriberTcp::ConnectorSubscriberTcp(ConnectorSubscriberTcp &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorSubscriberTcp::ConnectorSubscriberTcp(ConnectorSubscriberTcp const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorSubscriberTcp & llgc::pattern::publisher::ConnectorSubscriberTcp::operator=(ConnectorSubscriberTcp &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorSubscriberTcp & llgc::pattern::publisher::ConnectorSubscriberTcp::operator=(ConnectorSubscriberTcp const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

template <typename T>
INLINE_TEMPLATE
bool llgc::pattern::publisher::ConnectorSubscriberTcp<T>::Send(const T &message)
{
  std::string message_in_string;
  BUGLIB(std::cout, message.SerializeToString(&message_in_string), false,
         "protobuf");
  BUGCONT(std::cout, llgc::net::Linux::Send(socket_, message_in_string), false);
  return true;
}

/** \var llgc::pattern::publisher::ConnectorSubscriberTcp::socket_
 * \brief Socket to the server.
 */

#endif  // PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_TCP_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
