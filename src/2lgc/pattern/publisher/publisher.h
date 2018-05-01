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
template <typename T>
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
  virtual bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface<T>> subscriber) CHK;

  /**
   * @brief Send the message to all subscribers.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   */
  void Forward(const std::string& message);

  /**
   * @brief Send all pending messages of all subscribers.
   */
  void ForwardPending();

  /**
   * @brief Remove a subscriber of the server.
   *
   * @param id_message The message to unsubscriber.
   * @param subscriber The subscriber.
   *
   * @return true if subscriber is unsubscribed successfully.
   *         May failed if suscriber is not subscribe to the specific message.
   */
  virtual bool RemoveSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface<T>> subscriber) CHK;

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
  using SubscriberMap =
      std::multimap<uint32_t, std::shared_ptr<ConnectorInterface<T>>>;

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
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
