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

#ifndef PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_CC_
#define PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_CC_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <cassert>
#include <cstdint>
#include <memory>

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

/** \class llgc::pattern::publisher::ConnectorSubscriber
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber Subscriber to communicate with client.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorSubscriber<T>::ConnectorSubscriber(
    std::shared_ptr<SubscriberInterface<T>> subscriber)
    : ConnectorInterface<T>(subscriber)
{
}

/** \fn llgc::pattern::publisher::ConnectorSubscriber::~ConnectorSubscriber()
 * \brief Default virtual destructor.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorSubscriber::ConnectorSubscriber(ConnectorSubscriber &&other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorSubscriber::ConnectorSubscriber(ConnectorSubscriber const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorSubscriber & llgc::pattern::publisher::ConnectorSubscriber::operator=(ConnectorSubscriber &&other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorSubscriber & llgc::pattern::publisher::ConnectorSubscriber::operator=(ConnectorSubscriber const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

template <typename T>
INLINE_TEMPLATE
bool llgc::pattern::publisher::ConnectorSubscriber<T>::AddSubscriber(
    uint32_t /*id_message*/)
{
  // Can't append.
  assert(false);
}

template <typename T>
INLINE_TEMPLATE
bool llgc::pattern::publisher::ConnectorSubscriber<T>::RemoveSubscriber(
    uint32_t /*id_message*/)
{
  // Can't append.
  assert(false);
}

#endif  // PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
