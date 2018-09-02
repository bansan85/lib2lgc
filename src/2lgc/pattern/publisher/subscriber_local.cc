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

#include <2lgc/pattern/publisher/subscriber_local.h>

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

template <typename T>
class ConnectorInterface;
}  // namespace llgc::pattern::publisher

/** \class llgc::pattern::publisher::SubscriberLocal
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberLocal::SubscriberLocal(uint32_t id)
 * \brief Default constructor
 * \param[in] id The id of the constructor.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberLocal::~SubscriberLocal()
 * \brief Default virtual destructor.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberLocal::SubscriberLocal(SubscriberLocal&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberLocal::SubscriberLocal(SubscriberLocal const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn SubscriberLocal & llgc::pattern::publisher::SubscriberLocal::operator=(SubscriberLocal&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn SubscriberLocal & llgc::pattern::publisher::SubscriberLocal::operator=(SubscriberLocal const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 */

template <typename T>
bool llgc::pattern::publisher::SubscriberLocal<T>::Equals(
    const SubscriberInterface<T> &connector) const
{
  const auto *subscriber_cast =
      dynamic_cast<const SubscriberLocal<T> *>(&connector);

  // Not the same type.
  if (subscriber_cast == nullptr)
  {
    return false;
  }

  return subscriber_cast->id_ == id_;
}

/** \brief Set the connector. Can only be once.
 * \param conn The new connector.
 * \return true if connector was not previously set.
 */
template <typename T>
bool llgc::pattern::publisher::SubscriberLocal<T>::SetConnector(
    std::shared_ptr<ConnectorInterface<T>> conn)
{
  if (!connector_.expired())
  {
    return false;
  }

  connector_ = conn;
  return true;
}

/** \brief Send message to the connector.
 * \param message The protobuf message.
 * \return true if no problem.
 */
template <typename T>
bool llgc::pattern::publisher::SubscriberLocal<T>::Send(const T &message)
{
  if (auto connector = connector_.lock())
  {
    return connector->Send(message);
  }
  return false;
}

/** \brief Add a new subscriber.
 * \param[in] id_message The id of the message.
 * \return true if no problem.
 */
template <typename T>
bool llgc::pattern::publisher::SubscriberLocal<T>::AddSubscriber(
    uint32_t id_message)
{
  if (auto connector = connector_.lock())
  {
    return connector->AddSubscriber(id_message);
  }
  return false;
}

/** \brief Remove a subscriber.
 * \param[in] id_message id of the message.
 * \return true if no problem.
 */
template <typename T>
bool llgc::pattern::publisher::SubscriberLocal<T>::RemoveSubscriber(
    uint32_t id_message)
{
  if (auto connector = connector_.lock())
  {
    return connector->RemoveSubscriber(id_message);
  }
  return false;
}

/** \var llgc::pattern::publisher::SubscriberLocal::id_
 * \brief The id of the connector.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
