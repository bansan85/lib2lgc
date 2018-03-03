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

#include "publisher_base.h"

// C++ system
#include <memory>

// macros
#include <handle_error.h>

template <typename M>
pattern::publisher::PublisherBase<M>::PublisherBase()
    : subscribers_(), options_()
{
}

template <typename M>
void pattern::publisher::PublisherBase<M>::Forward(
    const std::shared_ptr<const std::string> &message)
{
  M actions;

  BUGUSER(actions.ParseFromString(*message.get()), ,
          "Failed to decode message.\n");

  for (int i = 0; i < actions.action_size(); i++)
  {
    // Must use auto because we don't know if M is in namespace or not.
    auto &action = actions.action(i);

    std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
        iterpair = subscribers_.equal_range(action.data_case());

    SubscriberMap::const_iterator it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      it->second->Listen(message);
    }
  }
}

template <typename M>
bool pattern::publisher::PublisherBase<M>::RemoveSubscriber(
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
