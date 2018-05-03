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

#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/subscriber_interface.h>  // IWYU pragma: keep

template <typename T>
llgc::pattern::publisher::ConnectorInterface<T>::ConnectorInterface(
    std::shared_ptr<SubscriberInterface<T>> subscriber)
    : messages_(), next_id_(0), subscriber_(subscriber)
{
}

template <typename T>
void llgc::pattern::publisher::ConnectorInterface<T>::Listen(const T& message,
                                                             const bool hold)
{
  if (hold)
  {
    messages_.push(message);
  }
  else
  {
    subscriber_->Listen(message);
  }
}

template <typename T>
void llgc::pattern::publisher::ConnectorInterface<T>::ListenPending()
{
  while (!messages_.empty())
  {
    const T& it = messages_.front();
    subscriber_->Listen(it);
    messages_.pop();
  }
}
