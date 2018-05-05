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

#ifndef PATTERN_PUBLISHER_PUBLISHER_H_
#define PATTERN_PUBLISHER_PUBLISHER_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/utils/thread/count_lock.h>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>

/**
 * @brief Google Protobuf stuff.
 */
namespace google::protobuf
{
class Message;
}

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

/**
 * @brief Server that will be used to managed subscribers and to keep and send
 *        messages.
 */
template <typename T, typename U>
class Publisher
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,  // NS
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  /**
   * @brief Options of the server.
   */
  struct Options
  {
    /**
     * @brief To allow a subscriber to subscribe twice to the same message.
     *        Default: false.
     */
    bool add_fail_if_already_subscribed;

    /**
     * @brief Default constructor.
     */
    Options() : add_fail_if_already_subscribed(false) {}
  };

  /**
   * @brief Default constructor.
   */
  Publisher();

  /**
   * @brief Default destructor.
   */
  virtual ~Publisher();

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  Publisher(Publisher&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  Publisher(Publisher const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  Publisher& operator=(Publisher&& other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  Publisher& operator=(Publisher const& other) & = delete;

  /**
   * @brief Add a subscriber to the server.
   *
   * @param id_message The message to subscribe.
   * @param subscriber The subscriber. It's not a reference.
   *
   * @return true if success. May failed if add_fail_if_already_subscribed is
   * true and the subscriber is already registered.
   */
  virtual bool AddSubscriber(uint32_t id_message, U subscriber) CHK;

  /**
   * @brief Send the message to all subscribers.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   *
   * @return true if no problem.
   */
  bool Forward(const std::string& message) CHK;

  /**
   * @brief Send all pending messages of all subscribers.
   *
   * @return true if no problem.
   */
  bool ForwardPending() CHK;

  /**
   * @brief Remove a subscriber of the server.
   *
   * @param id_message The message to unsubscriber.
   * @param subscriber The subscriber.
   *
   * @return true if subscriber is unsubscribed successfully.
   *         May failed if suscriber is not subscribe to the specific message.
   */
  virtual bool RemoveSubscriber(uint32_t id_message, U subscriber) CHK;

  /**
   * @brief Return if subscription will failed if subscriber already subscribed.
   *
   * @return A boolean.
   */
  bool GetOptionFailAlreadySubscribed();

  /**
   * @brief Set if subscription must failed if subscriber already subscribed.
   *
   * @param[in] value true if must failed.
   */
  void SetOptionFailAlreadySubscribed(bool value);

  /**
   * @brief Lock forwarding message all the while the return object is not
   * destroyed.
   *
   * @return The object to keep alive.
   */
  llgc::utils::thread::CountLock<size_t> LockForward() CHK;

 protected:
  /**
   * @brief Type of the map for subscribers.
   */
  using SubscriberMap = std::multimap<uint32_t, U>;

  /**
   * @brief List of subscribers to send message.
   */
  SubscriberMap subscribers_;

 private:
  /**
   * @brief Options for the behavious of server.
   */
  Options options_;

  /**
   * @brief Lock the forward. Must be use with class CountLock.
   */
  size_t lock_forward_;

  /**
   * @brief Forward is not thread-safe.
   */
  std::recursive_mutex mutex_forward_;

  /**
   * @brief Convert a weak_ptr to shared_ptr.
   *
   * @param[in] connector The weak_ptr connector.
   *
   * @return The shared_ptr from weak_ptr connector.
   */
  std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>> GetConn(
      std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<T>> connector);

  /**
   * @brief Convert a shared_ptr to shared_ptr. Exists to be compatible with
   * GetConn with weak_ptr.
   *
   * @param[in] connector The shared_ptr connector.
   *
   * @return connector.
   */
  std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>> GetConn(
      std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
          connector);
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
