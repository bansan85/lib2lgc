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

/**
 * @file server_remote.cc
 * @brief remove server.
 */
#include "server_remote.h"
#include <handle_error.h>
#include <memory>

/**
 * @brief The constructor.
 *
 * @tparam M typename.
 */
template <typename M>
pattern::publisher::ServerRemote<M>::ServerRemote() : ServerBase<M>()
{
}

/**
 * @brief The port.
 *
 * @tparam M typename.
 * @param[in] port The port.
 *
 * @return true if no problem.
 */
template <typename M>
bool pattern::publisher::ServerRemote<M>::StartIp(uint16_t port)
{
}

/**
 * @brief Add a subscriber.
 *
 * @tparam M typename.
 * @param[in,out] id_message id of the message.
 * @param[in,out] subscriber Subscriber.
 *
 * @return Do not fail if subscriber is already subscribed in the same
 * id_message.
 */
template <typename M>
bool pattern::publisher::ServerRemote<M>::AddSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface> subscriber)
{
  if (options_.add_fail_if_already_subscribed)
  {
    std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
        iterpair = subscribers_.equal_range(id_message);

    SubscriberMap::const_iterator it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      if (it->second->Equals(subscriber.get()))
      {
        return false;
      }
    }
  }

  subscribers_.insert(std::pair<uint32_t, std::shared_ptr<ConnectorInterface>>(
      id_message, subscriber));

  return true;
}

/**
 * @brief Remove a subscriber.
 *
 * @tparam M typename.
 * @param[in,out] id_message The id of the message.
 * @param[in,out] subscriber The subscriber.
 *
 * @return true if no problem.
 */
template <typename M>
bool pattern::publisher::ServerRemote<M>::RemoveSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface> subscriber)
{
  // Check if Subscriber is already subscribed.
  std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
      iterpair = subscribers_.equal_range(id_message);

  SubscriberMap::const_iterator it = iterpair.first;
  for (; it != iterpair.second; ++it)
  {
    if (it->second.get()->Equals(subscriber.get()))
    {
      subscribers_.erase(it);
      return true;
    }
  }

  return false;
}
