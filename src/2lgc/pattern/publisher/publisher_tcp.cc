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

#include <2lgc/pattern/publisher/publisher.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;
}

template <typename T>
llgc::pattern::publisher::PublisherTcp<T>::PublisherTcp(uint16_t port)
    : llgc::pattern::publisher::Publisher<
          T,
          std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>>(),
      port_(port),
      disposing_(false)
{
}

template <typename T>
llgc::pattern::publisher::PublisherTcp<T>::~PublisherTcp()
{
  // Can't destroy a thread if it's still running.
  JoinWait();
}

template <typename T>
bool llgc::pattern::publisher::PublisherTcp<T>::IsStopping()
{
  return disposing_;
}

template <typename T>
void llgc::pattern::publisher::PublisherTcp<T>::Stop()
{
  disposing_ = true;
}

template <typename T>
void llgc::pattern::publisher::PublisherTcp<T>::JoinWait()
{
  if (thread_wait_.joinable())
  {
    thread_wait_.join();
  }
  for (auto& thread_i : thread_sockets_)
  {
    if (thread_i.second.joinable())
    {
      thread_i.second.join();
    }
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
