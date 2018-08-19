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
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_subscriber_grpc.h>
#include <cassert>
#include <iostream>
namespace grpc
{
template <class W, class R>
class ServerReaderWriter;
}

template <typename T>
llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::ConnectorSubscriberGrpc(
    std::shared_ptr<SubscriberInterface<T>> subscriber,
    grpc::ServerReaderWriter<T, T> *stream)
    : ConnectorInterface<T>(subscriber), stream_(stream)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorSubscriberGrpc<
    T>::~ConnectorSubscriberGrpc() = default;

template <typename T>
bool llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::Equals(
    const ConnectorInterface<T> &connector) const
{
  const auto *connector_direct =
      dynamic_cast<const ConnectorSubscriberGrpc<T> *>(&connector);

  if (connector_direct == nullptr)
  {
    return false;
  }

  return this->subscriber_->Equals(*connector.GetSubscriber());
}

template <typename T>
bool llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::AddSubscriber(
    uint32_t /*id_message*/)
{
  // Can't append.
  assert(false);
}

template <typename T>
bool llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::Send(
    const T &message)
{
  BUGLIB(std::cout, stream_->Write(message), false, "grpc");
  return true;
}

template <typename T>
bool llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::RemoveSubscriber(
    uint32_t /*id_message*/)
{
  // Can't append.
  assert(false);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
