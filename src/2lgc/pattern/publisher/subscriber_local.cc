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
}

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

/** \var llgc::pattern::publisher::SubscriberLocal::id_
 * \brief The id of the connector.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
