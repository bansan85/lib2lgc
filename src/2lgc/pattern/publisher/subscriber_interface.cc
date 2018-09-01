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

/** \class llgc::pattern::publisher::SubscriberInterface
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberInterface::SubscriberInterface()
 * \brief Default constructor.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberInterface::~SubscriberInterface()
 * \brief Default virtual destructor.
 *
 *
 * \fn bool llgc::pattern::publisher::SubscriberInterface::Listen(const T &messages)
 * \brief Send message.
 * \param[in] messages Protobuf message to send.
 * \return true if no problem.
 *
 *
 * \fn bool llgc::pattern::publisher::SubscriberInterface::Equals(const SubscriberInterface<T> &connector) const
 * \brief Compare in connector is the same than the object.
 * \param[in] connector The connector to compare with.
 * \return true if the same.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberInterface::SubscriberInterface(SubscriberInterface &&other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::SubscriberInterface::SubscriberInterface(SubscriberInterface const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn SubscriberInterface & llgc::pattern::publisher::SubscriberInterface::operator=(SubscriberInterface &&other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn SubscriberInterface & llgc::pattern::publisher::SubscriberInterface::operator=(SubscriberInterface const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
