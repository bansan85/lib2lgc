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
#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_direct.h>  // IWYU pragma: keep
#include <iostream>

template <typename T>
llgc::pattern::publisher::ConnectorDirect<T>::ConnectorDirect(
    std::shared_ptr<SubscriberInterface<T>> subscriber,
    std::shared_ptr<PublisherDirect<T>> server)
    : ConnectorInterface<T>(subscriber), server_(server)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorDirect<T>::~ConnectorDirect() = default;

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::Equals(
    const ConnectorInterface<T> &connector) const
{
  const auto *connector_direct =
      dynamic_cast<const ConnectorDirect<T> *>(&connector);

  if (connector_direct == nullptr)
  {
    return false;
  }

  return this->subscriber_->Equals(*connector.GetSubscriber());
}

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::AddSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout,
          server_->AddSubscriber(id_message, this->shared_from_this()), false);
  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::Send(
    const std::string &message)
{
  BUGCONT(std::cout, server_->Forward(message), false);
  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::RemoveSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout,
          server_->RemoveSubscriber(id_message, this->shared_from_this()),
          false);
  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
