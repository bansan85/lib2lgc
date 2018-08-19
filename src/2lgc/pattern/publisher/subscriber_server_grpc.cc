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
#include <2lgc/pattern/publisher/subscriber_server_grpc.h>
#include <iostream>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

template <typename T>
bool llgc::pattern::publisher::SubscriberServerGrpc<T>::Listen(
    const T &messages)
{
  BUGLIB(std::cout, stream_->Write(messages), false, "grpc");

  return true;
}

template <typename T>
bool llgc::pattern::publisher::SubscriberServerGrpc<T>::Equals(
    const SubscriberInterface<T> &connector) const
{
  const auto *subscriber_cast =
      dynamic_cast<const SubscriberServerGrpc<T> *>(&connector);

  // Not the same type.
  if (subscriber_cast == nullptr)
  {
    return false;
  }

  return subscriber_cast->stream_ == stream_;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
