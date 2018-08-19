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

#include <2lgc/pattern/publisher/publisher_ip.h>
#include <2lgc/pattern/publisher/publisher_tcp.h>

template <typename T>
llgc::pattern::publisher::PublisherTcp<T>::PublisherTcp(uint16_t port)
    : llgc::pattern::publisher::PublisherIp<T>(port), disposing_(false)
{
}

template <typename T>
llgc::pattern::publisher::PublisherTcp<T>::~PublisherTcp()
{
  PublisherTcp<T>::JoinWait();
}

template <typename T>
void llgc::pattern::publisher::PublisherTcp<T>::JoinWait()
{
  PublisherIp<T>::JoinWait();
  for (auto& thread_i : thread_sockets_)
  {
    if (thread_i.second.joinable())
    {
      thread_i.second.join();
    }
  }
}

template <typename T>
void llgc::pattern::publisher::PublisherTcp<T>::Stop()
{
  disposing_ = true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
