/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <2lgc/pattern/publisher/connector_client_tcp.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

template <typename T>
llgc::pattern::publisher::ConnectorClientTcp<T>::ConnectorClientTcp(
    std::shared_ptr<SubscriberInterface<T>> subscriber, int socket_fd)
    : ConnectorInterface<T>(subscriber), socket_(socket_fd)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorClientTcp<T>::~ConnectorClientTcp() =
    default;

template <typename T>
bool llgc::pattern::publisher::ConnectorClientTcp<T>::Equals(
    const ConnectorInterface<T> &connector) const
{
  const auto *connector_direct =
      dynamic_cast<const ConnectorClientTcp<T> *>(&connector);

  if (connector_direct == nullptr)
  {
    return false;
  }

  return this->subscriber_->Equals(*connector.GetSubscriber());
}

template <typename T>
bool llgc::pattern::publisher::ConnectorClientTcp<T>::AddSubscriber(
    uint32_t /*id_message*/)
{
  return false;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorClientTcp<T>::Send(
    const std::string &message)
{
  std::cout << "ConnectorClientTcp<T>::Send" << std::endl;
  return send(socket_, message.c_str(), message.length(), 0) ==
         static_cast<ssize_t>(message.length());
}

template <typename T>
bool llgc::pattern::publisher::ConnectorClientTcp<T>::RemoveSubscriber(
    uint32_t /*id_message*/)
{
  return false;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
