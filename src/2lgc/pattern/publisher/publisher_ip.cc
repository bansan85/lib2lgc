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

#ifndef PATTERN_PUBLISHER_PUBLISHER_IP_CC_
#define PATTERN_PUBLISHER_PUBLISHER_IP_CC_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_ip.h>

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;
}

/** \class llgc::pattern::publisher::PublisherIp
 * \brief Interface to create a TCP server.
 * \tparam T Message from protobuf.
 * \dotfile pattern/publisher/publisher_tcp.dot
 */

/** \brief Constructor with port for the TCP server.
 * \param[in] port The port to listen from.
 */
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::PublisherIp<T>::PublisherIp(
    uint16_t port)
    : llgc::pattern::publisher::PublisherInterface<
          T,
          std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>>(),
      port_(port)
{
}

/// \brief Destructor. Make sure that thread is finished.
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::PublisherIp<T>::~PublisherIp()
{
  // Can't destroy a thread if it's still running.
  PublisherIp<T>::JoinWait();
}

/** \fn bool llgc::pattern::publisher::PublisherIp::Listen()
 * \brief Start the server and the listening the port.
 * \return true if no problem.
 *
 *
 * \fn bool llgc::pattern::publisher::PublisherIp::Wait()
 * \brief Wait for client.
 * \return true if no problem.
 *
 *
 * \fn void llgc::pattern::publisher::PublisherIp::Stop()
 * \brief Stop the thread.
 */

/// \brief Join the waiting thread.
template <typename T>
INLINE_TEMPLATE void llgc::pattern::publisher::PublisherIp<T>::JoinWait()
{
  if (thread_wait_.joinable())
  {
    thread_wait_.join();
  }
}

/** \fn uint16_t llgc::pattern::publisher::PublisherIp::GetPort() const
 * \brief Return the port.
 * \return The port.
 *
 *
 * \fn llgc::pattern::publisher::PublisherIp::PublisherIp(PublisherIp&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::pattern::publisher::PublisherIp::PublisherIp(PublisherIp const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn PublisherIp& llgc::pattern::publisher::PublisherIp::operator=(PublisherIp&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn PublisherIp& llgc::pattern::publisher::PublisherIp::operator=(PublisherIp const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \var llgc::pattern::publisher::PublisherIp::thread_wait_
 * \brief Thread that run the Wait function and add socket to thread_sockets_.
 *
 *
 * \var llgc::pattern::publisher::PublisherIp::port_
 * \brief Port to listen from.
 */

#endif  // PATTERN_PUBLISHER_PUBLISHER_IP_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
