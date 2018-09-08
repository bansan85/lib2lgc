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

#ifndef PATTERN_PUBLISHER_CONNECTOR_INTERFACE_CC_
#define PATTERN_PUBLISHER_CONNECTOR_INTERFACE_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/connector_interface.h>
// IWYU wants to remove it but subscriber_->Equals will fails.
#include <2lgc/pattern/publisher/subscriber_interface.h>  // IWYU pragma: keep
#include <iostream>
#include <typeinfo>

/** \class llgc::pattern::publisher::ConnectorInterface
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *        There's could be three kind of connector: direct connection, TCP/IP
 *        and gRPC.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber Subscriber.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorInterface<T>::ConnectorInterface(
    std::shared_ptr<SubscriberInterface<T>> subscriber)
    : subscriber_(std::move(subscriber)), messages_(), next_id_(0)
{
}

/** \fn llgc::pattern::publisher::ConnectorInterface::~ConnectorInterface()
 * \brief Default destructor.
 */

/** \brief Compare in connector is the same than the object.
 * \param[in] connector The connector to compare with.
 * \return true if the same.
 */
template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::ConnectorInterface<T>::Equals(
    const ConnectorInterface<T> &connector) const
{
  if (typeid(connector) != typeid(*this))
  {
    return false;
  }

  return subscriber_->Equals(*connector.GetSubscriber());
}

/** \fn bool llgc::pattern::publisher::ConnectorInterface::Send(const T& message)
 * \brief Send message to the publisher.
 * \param message The protobuf message.
 * \return true if no problem.
 */

/** \brief message from the publisher.
 * \param[in] message The protobuf message.
 * \param[in] hold If true, message is not send to subscriber be will be pending
 *            until ListenPending is called.
 * \return true if no problem.
 */
template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::ConnectorInterface<T>::Listen(
    const T &message, bool hold)
{
  if (hold)
  {
    messages_.push(message);
  }
  else
  {
    BUGCONT(std::cout, subscriber_->Listen(message), false);
  }

  return true;
}

/** \brief Listen pending messages from the publisher.
 * \return true if no problem.
 */
template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorInterface<T>::ListenPending()
{
  while (!messages_.empty())
  {
    const T &it = messages_.front();
    BUGCONT(std::cout, subscriber_->Listen(it), false);
    messages_.pop();
  }
  return true;
}

/** \fn bool llgc::pattern::publisher::ConnectorInterface::AddSubscriber(uint32_t id_message)
 * \brief Add a new subscriber.
 * \param[in] id_message The id of the message.
 * \return true if no problem.
 * \dotfile pattern/publisher/publisher_add_subscriber.dot
 *
 *
 * \fn bool llgc::pattern::publisher::ConnectorInterface::RemoveSubscriber(uint32_t id_message)
 * \brief Remove a subscriber.
 * \param[in] id_message id of the message.
 * \return true if no problem.
 *
 *
 * \fn const SubscriberInterface<T>* llgc::pattern::publisher::ConnectorInterface::GetSubscriber() const
 * \brief Get the subscriber that manager this interface.
 * \return Return the subscriber.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorInterface::ConnectorInterface(ConnectorInterface&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorInterface::ConnectorInterface(ConnectorInterface const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn ConnectorInterface& llgc::pattern::publisher::ConnectorInterface::operator=(ConnectorInterface&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return * Nothing.
 *
 *
 * \fn ConnectorInterface& llgc::pattern::publisher::ConnectorInterface::operator=(ConnectorInterface const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \var llgc::pattern::publisher::ConnectorInterface::subscriber_
 * \brief The subscriber.
 *
 *
 * \var llgc::pattern::publisher::ConnectorInterface::messages_
 * \brief Messages on hold.
 *
 *
 * \var llgc::pattern::publisher::ConnectorInterface::next_id_
 * \brief id of the next message.
 */

#endif  // PATTERN_PUBLISHER_CONNECTOR_INTERFACE_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
