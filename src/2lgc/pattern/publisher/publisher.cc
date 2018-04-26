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

#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/connector_interface.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/publisher.h>
#include <2lgc/poco/gdb.pb.h>  // IWYU pragma: keep
#include <2lgc/utils/thread/count_lock.h>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

template <typename M>
llgc::pattern::publisher::Publisher<M>::Publisher()
    : options_(), lock_forward_(0)
{
}

template <typename M>
llgc::pattern::publisher::Publisher<M>::~Publisher() = default;

template <typename M>
void llgc::pattern::publisher::Publisher<M>::Forward(
    std::shared_ptr<const std::string> message)
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
  M actions;

  BUGUSER(actions.ParseFromString(*message.get()), ,
          "Failed to decode message.\n");

  for (int i = 0; i < actions.action_size(); i++)
  {
    // Must use auto because we don't know if M is in namespace or not.
    auto action = actions.action(i);

    // Filter to keep only the data case.
    auto iterpair = subscribers_.equal_range(action.data_case());

    auto &it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      it->second->Listen(message, lock_forward_ != 0);
    }
  }
}

template <typename M>
void llgc::pattern::publisher::Publisher<M>::ForwardPending()
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);

  if (lock_forward_ != 0)
  {
    return;
  }

  for (auto &it : subscribers_)
  {
    it.second->ListenPending();
  }
}

template <typename M>
bool llgc::pattern::publisher::Publisher<M>::GetOptionFailAlreadySubscribed()
{
  return options_.add_fail_if_already_subscribed;
}

template <typename M>
void llgc::pattern::publisher::Publisher<M>::SetOptionFailAlreadySubscribed(
    bool value)
{
  options_.add_fail_if_already_subscribed = value;
}

template <typename M>
llgc::utils::thread::CountLock<size_t>
llgc::pattern::publisher::Publisher<M>::LockForward()
{
  return llgc::utils::thread::CountLock<size_t>(
      &lock_forward_, &mutex_forward_,
      std::bind(&llgc::pattern::publisher::Publisher<M>::ForwardPending, this));
}

// Do not fail if subscriber is already subscribed in the same id_message.
template <typename M>
bool llgc::pattern::publisher::Publisher<M>::AddSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface> subscriber)
{
  if (GetOptionFailAlreadySubscribed())
  {
    std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
        iterpair = subscribers_.equal_range(id_message);

    auto it = iterpair.first;
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

template <typename M>
bool llgc::pattern::publisher::Publisher<M>::RemoveSubscriber(
    uint32_t id_message, const std::shared_ptr<ConnectorInterface>& subscriber)
{
  // Check if Subscriber is already subscribed.
  std::pair<SubscriberMap::const_iterator, SubscriberMap::const_iterator>
      iterpair = subscribers_.equal_range(id_message);

  auto it = iterpair.first;
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

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
