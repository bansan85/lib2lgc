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

#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_client_tcp.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <cassert>
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
  // Can't append.
  assert(false);
}

template <typename T>
bool llgc::pattern::publisher::ConnectorClientTcp<T>::Send(
    const std::string &message)
{
  std::cout << "ConnectorClientTcp<T>::Send" << std::endl;
  BUGCONT(llgc::net::Linux::Send(socket_, message), false);
  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorClientTcp<T>::RemoveSubscriber(
    uint32_t /*id_message*/)
{
  assert(false);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
