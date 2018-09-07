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

#ifndef PATTERN_PUBLISHER_CONNECTOR_DIRECT_CC_
#define PATTERN_PUBLISHER_CONNECTOR_DIRECT_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <iostream>

/** \namespace llgc::pattern::publisher
 * \brief Namespace for the pattern publisher.
 *
 *
 * \class llgc::pattern::publisher::ConnectorDirect
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber The subscriber.
 * \param[in] server The server.
 */
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::ConnectorDirect<T>::ConnectorDirect(
    std::shared_ptr<SubscriberInterface<T>> subscriber,
    std::shared_ptr<PublisherDirect<T>> server)
    : ConnectorInterface<T>(std::move(subscriber)), server_(std::move(server))
{
}

/** \fn llgc::pattern::publisher::ConnectorDirect::~ConnectorDirect()
 * \brief Default virtual destructor.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorDirect::ConnectorDirect(ConnectorDirect &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorDirect::ConnectorDirect(ConnectorDirect const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorDirect & llgc::pattern::publisher::ConnectorDirect::operator=(ConnectorDirect &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorDirect & llgc::pattern::publisher::ConnectorDirect::operator=(ConnectorDirect const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorDirect<T>::AddSubscriber(uint32_t id_message)
{
  BUGCONT(std::cout,
          server_->AddSubscriber(id_message, this->shared_from_this()), false);
  return true;
}

template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::ConnectorDirect<T>::Send(
    const T &message)
{
  BUGCONT(std::cout, server_->Forward(message), false);
  return true;
}

template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorDirect<T>::RemoveSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout,
          server_->RemoveSubscriber(id_message, this->shared_from_this()),
          false);
  return true;
}

/** \var llgc::pattern::publisher::ConnectorDirect::server_
 * \brief The server.
 */

#endif  // PATTERN_PUBLISHER_CONNECTOR_DIRECT_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
