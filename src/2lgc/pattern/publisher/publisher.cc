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
#include <utility>

template <typename T>
llgc::pattern::publisher::Publisher<T>::Publisher()
    : options_(), lock_forward_(0)
{
}

template <typename T>
llgc::pattern::publisher::Publisher<T>::~Publisher() = default;

template <typename T>
void llgc::pattern::publisher::Publisher<T>::Forward(const std::string &message)
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
  T actions;

  BUGUSER(actions.ParseFromString(message), , "Failed to decode message.\n");

  // We start to store destination to avoid sending the same message multiple
  // time if the subscriber is subscribe to multiple id and the message
  // contains multiple action with the same id.
  std::map<std::shared_ptr<ConnectorInterface<T>>, T> destination;

  for (int i = 0; i < actions.action_size(); i++)
  {
    decltype(actions.action(i)) action = actions.action(i);

    // Filter to keep only the data case.
    auto iterpair = subscribers_.equal_range(action.data_case());

    auto &it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      auto it2 = destination.find(it->second);
      if (it2 == destination.end())
      {
        destination[it->second] = T();
      }
      T &message_i = destination[it->second];
      decltype(message_i.add_action()) new_action_i = message_i.add_action();
      new_action_i->CopyFrom(action);
    }
  }

  for (std::pair<std::shared_ptr<ConnectorInterface<T>>, T> it : destination)
  {
    it.first->Listen(it.second, lock_forward_ != 0);
  }
}

template <typename T>
void llgc::pattern::publisher::Publisher<T>::ForwardPending()
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);

  if (lock_forward_ != 0)
  {
    return;
  }

  for (std::pair<uint32_t, std::shared_ptr<ConnectorInterface<T>>> it :
       subscribers_)
  {
    it.second->ListenPending();
  }
}

template <typename T>
bool llgc::pattern::publisher::Publisher<T>::GetOptionFailAlreadySubscribed()
{
  return options_.add_fail_if_already_subscribed;
}

template <typename T>
void llgc::pattern::publisher::Publisher<T>::SetOptionFailAlreadySubscribed(
    bool value)
{
  options_.add_fail_if_already_subscribed = value;
}

template <typename T>
llgc::utils::thread::CountLock<size_t>
llgc::pattern::publisher::Publisher<T>::LockForward()
{
  return llgc::utils::thread::CountLock<size_t>(
      &lock_forward_, &mutex_forward_,
      std::bind(&llgc::pattern::publisher::Publisher<T>::ForwardPending, this));
}

// Do not fail if subscriber is already subscribed in the same id_message.
template <typename T>
bool llgc::pattern::publisher::Publisher<T>::AddSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface<T>> subscriber)
{
  if (GetOptionFailAlreadySubscribed())
  {
    auto iterpair = subscribers_.equal_range(id_message);

    auto &it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      if (it->second->Equals(*subscriber))
      {
        return false;
      }
    }
  }

  subscribers_.insert(
      std::pair<uint32_t, std::shared_ptr<ConnectorInterface<T>>>(id_message,
                                                                  subscriber));

  return true;
}

template <typename T>
bool llgc::pattern::publisher::Publisher<T>::RemoveSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface<T>> subscriber)
{
  // Check if Subscriber is already subscribed.
  auto iterpair = subscribers_.equal_range(id_message);

  auto it = iterpair.first;
  for (; it != iterpair.second; ++it)
  {
    if (it->second->Equals(*subscriber))
    {
      subscribers_.erase(it);
      return true;
    }
  }

  return false;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
