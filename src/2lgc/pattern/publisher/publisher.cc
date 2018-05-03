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
#include <2lgc/pattern/publisher/connector_interface.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/publisher.h>
#include <2lgc/poco/gdb.pb.h>  // IWYU pragma: keep
#include <2lgc/utils/thread/count_lock.h>
#include <functional>
#include <memory>
#include <utility>

template <typename T, typename U>
std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
llgc::pattern::publisher::Publisher<T, U>::GetConn(
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<T>> connector)
{
  return connector.lock();
}

template <typename T, typename U>
std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
llgc::pattern::publisher::Publisher<T, U>::GetConn(
    std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>> connector)
{
  return connector;
}

template <typename T, typename U>
llgc::pattern::publisher::Publisher<T, U>::Publisher()
    : options_(), lock_forward_(0)
{
}

template <typename T, typename U>
llgc::pattern::publisher::Publisher<T, U>::~Publisher() = default;

template <typename T, typename U>
void llgc::pattern::publisher::Publisher<T, U>::Forward(
    const std::string &message)
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
  T actions;

  BUGUSER(actions.ParseFromString(message), , "Failed to decode message.\n");

  // We start to store destination to avoid sending the same message multiple
  // time if the subscriber is subscribe to multiple id and the message
  // contains multiple action with the same id.
  std::map<U, T, std::owner_less<U>> destination;

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

  for (std::pair<U, T> it : destination)
  {
    std::shared_ptr<ConnectorInterface<T>> connector_subscriber =
        GetConn(it.first);
    if (connector_subscriber == nullptr)
    {
      continue;
    }
    connector_subscriber->Listen(it.second, lock_forward_ != 0);
  }
}

template <typename T, typename U>
void llgc::pattern::publisher::Publisher<T, U>::ForwardPending()
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);

  if (lock_forward_ != 0)
  {
    return;
  }

  for (std::pair<uint32_t, U> it : subscribers_)
  {
    std::shared_ptr<ConnectorInterface<T>> connector_subscriber =
        GetConn(it.second);
    if (connector_subscriber == nullptr)
    {
      continue;
    }
    connector_subscriber->ListenPending();
  }
}

template <typename T, typename U>
bool llgc::pattern::publisher::Publisher<T, U>::GetOptionFailAlreadySubscribed()
{
  return options_.add_fail_if_already_subscribed;
}

template <typename T, typename U>
void llgc::pattern::publisher::Publisher<T, U>::SetOptionFailAlreadySubscribed(
    bool value)
{
  options_.add_fail_if_already_subscribed = value;
}

template <typename T, typename U>
llgc::utils::thread::CountLock<size_t>
llgc::pattern::publisher::Publisher<T, U>::LockForward()
{
  return llgc::utils::thread::CountLock<size_t>(
      &lock_forward_, &mutex_forward_,
      std::bind(&Publisher<T, U>::ForwardPending, this));
}

// Do not fail if subscriber is already subscribed in the same id_message.
template <typename T, typename U>
bool llgc::pattern::publisher::Publisher<T, U>::AddSubscriber(
    uint32_t id_message, U subscriber)
{
  if (GetOptionFailAlreadySubscribed())
  {
    auto iterpair = subscribers_.equal_range(id_message);

    auto &it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      std::shared_ptr<ConnectorInterface<T>> connector_subscriber =
          GetConn(subscriber);
      std::shared_ptr<ConnectorInterface<T>> connector_it = GetConn(it->second);
      if (connector_subscriber == nullptr || connector_it == nullptr)
      {
        return false;
      }
      if (connector_it->Equals(*connector_subscriber))
      {
        return false;
      }
    }
  }

  subscribers_.insert(std::pair<uint32_t, U>(id_message, subscriber));

  return true;
}

template <typename T, typename U>
bool llgc::pattern::publisher::Publisher<T, U>::RemoveSubscriber(
    uint32_t id_message, U subscriber)
{
  // Remove the subscriber from all id_message.
  if (id_message == 0)
  {
    for (auto it = subscribers_.cbegin(); it != subscribers_.cend();)
    {
      std::shared_ptr<ConnectorInterface<T>> connector_i = GetConn(it->second);
      std::shared_ptr<ConnectorInterface<T>> connector_subscriber =
          GetConn(subscriber);
      if (connector_i == nullptr || connector_subscriber == nullptr)
      {
        continue;
      }
      if (connector_i->Equals(*connector_subscriber))
      {
        it = subscribers_.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }
  else
  {
    // Check if Subscriber is already subscribed.
    auto iterpair = subscribers_.equal_range(id_message);

    auto it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      std::shared_ptr<ConnectorInterface<T>> connector_i = GetConn(it->second);
      std::shared_ptr<ConnectorInterface<T>> connector_subscriber =
          GetConn(subscriber);
      if (connector_i == nullptr || connector_subscriber == nullptr)
      {
        continue;
      }
      if (connector_i->Equals(*connector_subscriber))
      {
        subscribers_.erase(it);
        return true;
      }
    }
  }

  return false;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
