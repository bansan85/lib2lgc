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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_CC_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_CC_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>

/** \class llgc::pattern::publisher::PublisherTcp
 * \brief Interface to create a TCP server.
 * \tparam T Message from protobuf.
 * \dotfile pattern/publisher/publisher_tcp.dot
 */

/** \brief Constructor with port for the TCP server.
 * \param[in] port The port to listen from.
 */
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::PublisherTcp<T>::PublisherTcp(
    uint16_t port)
    : llgc::pattern::publisher::PublisherIp<T>(port), disposing_(false)
{
}

/// \brief Destructor. Make sure that thread is finished.
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::PublisherTcp<T>::~PublisherTcp()
{
  PublisherTcp<T>::JoinWait();
}

template <typename T>
INLINE_TEMPLATE void llgc::pattern::publisher::PublisherTcp<T>::JoinWait()
{
  PublisherIp<T>::JoinWait();
  for (auto &thread_i : thread_sockets_)
  {
    if (thread_i.second.joinable())
    {
      thread_i.second.join();
    }
  }
}

template <typename T>
INLINE_TEMPLATE void llgc::pattern::publisher::PublisherTcp<T>::Stop()
{
  disposing_ = true;
}

/** \fn llgc::pattern::publisher::PublisherTcp::PublisherTcp(PublisherTcp&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::pattern::publisher::PublisherTcp::PublisherTcp(PublisherTcp const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn PublisherTcp& llgc::pattern::publisher::PublisherTcp::operator=(PublisherTcp&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn PublisherTcp& llgc::pattern::publisher::PublisherTcp::operator=(PublisherTcp const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \var llgc::pattern::publisher::PublisherTcp::disposing_
 * \brief If thread in trying to stop.
 *
 *
 * \var llgc::pattern::publisher::PublisherTcp::thread_sockets_
 * \brief Store thread based on the socket file descriptor.
 *
 *
 * \fn void llgc::pattern::publisher::PublisherTcp::AddSubscriberLocal(int socket, T::Msg message)
 * \brief Internal function to subscribe a socket to an event.
 * \param[in] socket The socket.
 * \param[in] message The message.
 */

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
