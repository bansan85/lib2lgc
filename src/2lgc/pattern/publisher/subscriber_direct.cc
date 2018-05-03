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

#include <2lgc/pattern/publisher/subscriber_direct.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

template <typename T>
bool llgc::pattern::publisher::SubscriberDirect<T>::Equals(
    const SubscriberInterface<T> &connector) const
{
  const auto *subscriber_direct_cast =
      dynamic_cast<const SubscriberDirect<T> *>(&connector);

  // Not the same type.
  if (subscriber_direct_cast == nullptr)
  {
    return false;
  }

  return subscriber_direct_cast->id_ == id_;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
