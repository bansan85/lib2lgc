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
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/poco/software_gdb.pb.h>  // IWYU pragma: keep
#include <2lgc/utils/count_lock.h>
#include <iostream>
#include <memory>
#include <utility>

/** \class llgc::pattern::publisher::PublisherInterface
 * \brief Server that will be used to managed subscribers and to keep and send
 *        messages.
 * \tparam T Message from protobuf.
 * \tparam U Type of the subscriber.
 *
 *
 * \struct llgc::pattern::publisher::PublisherInterface::Options
 * \brief Options of the server.
 *
 *
 * \var llgc::pattern::publisher::PublisherInterface::Options::add_fail_if_already_subscribed
 * \brief To allow a subscriber to subscribe twice to the same message.
 *        Default: false.
 */

/// \brief Default constructor.
template <typename T, typename U>
llgc::pattern::publisher::PublisherInterface<T, U>::PublisherInterface()
    : options_()
{
}

/** \fn llgc::pattern::publisher::PublisherInterface::~PublisherInterface()
 * \brief Default destructor.
 */

/** \brief Add a subscriber to the server.
 * \param id_message The message to subscribe.
 * \param subscriber The subscriber. It's not a reference.
 * \return true if success. May failed if add_fail_if_already_subscribed is
 *         true and the subscriber is already registered.
 * \dotfile pattern/publisher/publisher_add_subscriber.dot
 */
template <typename T, typename U>
bool llgc::pattern::publisher::PublisherInterface<T, U>::AddSubscriber(
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
      BUGCRIT(std::cout,
              connector_subscriber != nullptr && connector_it != nullptr, false,
              "Failed to add subscriber.\n");
      BUGCRIT(std::cout, !connector_it->Equals(*connector_subscriber), false,
              "Subscriber already exists.\n");
    }
  }

  subscribers_.insert(std::pair<uint32_t, U>(id_message, subscriber));

  return true;
}

/** \brief Send the message to all subscribers.
 * \param messages Message in ProtoBuf format.
 * \return true if no problem.
 */
template <typename T, typename U>
bool llgc::pattern::publisher::PublisherInterface<T, U>::Forward(
    const T &messages)
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);

  // We start to store destination to avoid sending the same message multiple
  // time if the subscriber is subscribe to multiple id.
  std::map<U, T, std::owner_less<U>> destination;

  for (int i = 0; i < messages.msg_size(); i++)
  {
    const auto &message = messages.msg(i);

    // Filter to keep only the data case.
    const auto &iterpair = subscribers_.equal_range(message.data_case());

    auto it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      auto it2 = destination.find(it->second);
      if (it2 == destination.end())
      {
        destination[it->second] = T();
      }
      T &message_i = destination[it->second];
      auto new_message_i = message_i.add_msg();
      new_message_i->CopyFrom(message);
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
    BUGCONT(std::cout,
            connector_subscriber->Listen(it.second, lock_forward_ != 0), false);
  }

  return true;
}

/** \brief Send all pending messages of all subscribers.
 * \return true if no problem.
 */
template <typename T, typename U>
bool llgc::pattern::publisher::PublisherInterface<T, U>::ForwardPending()
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);

  if (lock_forward_ != 0)
  {
    return false;
  }

  for (std::pair<uint32_t, U> it : subscribers_)
  {
    std::shared_ptr<ConnectorInterface<T>> connector_subscriber =
        GetConn(it.second);
    if (connector_subscriber == nullptr)
    {
      continue;
    }
    BUGCONT(std::cout, connector_subscriber->ListenPending(), false);
  }

  return true;
}

/** \brief Remove a subscriber of the server.
 * \param id_message The message to unsubscriber.
 * \param subscriber The subscriber.
 * \return true if subscriber is unsubscribed successfully.
 *         May failed if suscriber is not subscribe to the specific message.
 */
template <typename T, typename U>
bool llgc::pattern::publisher::PublisherInterface<T, U>::RemoveSubscriber(
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
      BUGCRIT(std::cout,
              connector_i != nullptr && connector_subscriber != nullptr, false,
              "Failed to remove subscriber.\n");
      /*
      if (connector_i == nullptr || connector_subscriber == nullptr)
      {
        continue;
      }
      */
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
      BUGCRIT(std::cout,
              connector_i != nullptr && connector_subscriber != nullptr, false,
              "Failed to remove subscriber.\n");
      /*
      if (connector_i == nullptr || connector_subscriber == nullptr)
      {
        continue;
      }
      */
      if (connector_i->Equals(*connector_subscriber))
      {
        subscribers_.erase(it);
        return true;
      }
    }
  }

  return false;
}

/** \brief Return if subscription will failed if subscriber already subscribed.
 * \return The boolean.
 */
template <typename T, typename U>
bool llgc::pattern::publisher::PublisherInterface<
    T, U>::GetOptionFailAlreadySubscribed()
{
  return options_.add_fail_if_already_subscribed;
}

/** \brief Set if subscription must failed if subscriber already subscribed.
 * \param[in] value true if must failed.
 */
template <typename T, typename U>
void llgc::pattern::publisher::PublisherInterface<
    T, U>::SetOptionFailAlreadySubscribed(bool value)
{
  options_.add_fail_if_already_subscribed = value;
}

/** \brief Lock forwarding message all the while the return object is not
 *         destroyed.
 * \return The object to keep alive.
 */
template <typename T, typename U>
llgc::utils::thread::CountLock<size_t>
llgc::pattern::publisher::PublisherInterface<T, U>::LockForward()
{
  return llgc::utils::thread::CountLock<size_t>(
      &lock_forward_, &mutex_forward_,
      [this] { return this->ForwardPending(); });
}

/** \fn llgc::pattern::publisher::PublisherInterface::PublisherInterface(PublisherInterface&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::PublisherInterface::PublisherInterface(PublisherInterface const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn PublisherInterface& llgc::pattern::publisher::PublisherInterface::operator=(PublisherInterface&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn PublisherInterface& llgc::pattern::publisher::PublisherInterface::operator=(PublisherInterface const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \typedef llgc::pattern::publisher::PublisherInterface::SubscriberMap
 * \brief Type of the map for subscribers.
 *
 *
 * \var llgc::pattern::publisher::PublisherInterface::subscribers_
 * \brief List of subscribers to send message.
 *
 *
 * \var llgc::pattern::publisher::PublisherInterface::options_
 * \brief Options for the behavious of server.
 *
 *
 * \var llgc::pattern::publisher::PublisherInterface::lock_forward_
 * \brief Lock the forward. Must be use with class CountLock.
 *
 *
 * \var llgc::pattern::publisher::PublisherInterface::mutex_forward_
 * \brief Forward is not thread-safe.
 */

/** \brief Convert a weak_ptr to shared_ptr.
 * \param[in] connector The weak_ptr connector.
 * \return The shared_ptr from weak_ptr connector.
 */
template <typename T, typename U>
std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
llgc::pattern::publisher::PublisherInterface<T, U>::GetConn(
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<T>> connector)
{
  return connector.lock();
}

/** \brief Convert a shared_ptr to shared_ptr. Exists to be compatible with
 *         GetConn with weak_ptr.
 * \param[in] connector The shared_ptr connector.
 * \return connector.
 */
template <typename T, typename U>
std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
llgc::pattern::publisher::PublisherInterface<T, U>::GetConn(
    std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>> connector)
{
  return connector;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
