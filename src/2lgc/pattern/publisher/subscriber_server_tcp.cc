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

#include <2lgc/pattern/publisher/subscriber_server_tcp.h>
#include <sys/socket.h>
#include <string>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

template <typename T>
void llgc::pattern::publisher::SubscriberServerTcp<T>::Listen(const T &message)
{
  std::string message_socket;

  message.SerializeToString(&message_socket);
  send(socket_, message_socket.c_str(), message_socket.length(), 0);
}

template <typename T>
bool llgc::pattern::publisher::SubscriberServerTcp<T>::Equals(
    const SubscriberInterface<T> &connector) const
{
  const auto *subscriber_direct_cast =
      dynamic_cast<const SubscriberServerTcp<T> *>(&connector);

  // Not the same type.
  if (subscriber_direct_cast == nullptr)
  {
    return false;
  }

  return subscriber_direct_cast->socket_ == socket_;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
