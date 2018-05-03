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

#ifndef PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_
#define PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_

#include <2lgc/compatibility/visual_studio.h>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class ConnectorInterface
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in,out] subscriber Subscriber.
   */
  explicit ConnectorInterface(
      std::shared_ptr<SubscriberInterface<T>> subscriber);

  /**
   * @brief Default destructor.
   */
  virtual ~ConnectorInterface() = default;

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface(ConnectorInterface&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface(ConnectorInterface const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface& operator=(ConnectorInterface&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  ConnectorInterface& operator=(ConnectorInterface const& other) & = delete;

  /**
   * @brief Compare in connector is the same than the object.
   *
   * @param[in] connector The connector to compare with.
   *
   * @return true if the same.
   */
  virtual bool Equals(const ConnectorInterface<T>& connector) const CHK = 0;

  /**
   * @brief Send message to the publisher.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   *
   * @return true if no problem.
   */
  virtual bool Send(const std::string& message) CHK = 0;

  /**
   * @brief Listen message from the publisher.
   *
   * @param[in] message Data of the message in ProtoBuf, SerializeToString.
   * @param[in] hold If true, message is not send to subscriber be will be
   * pending until ListenPending is called.
   */
  void Listen(const T& message, const bool hold);

  /**
   * @brief Listen pending messages from the publisher.
   */
  void ListenPending();

  /**
   * @brief Get the subscriber that manager this interface.
   *
   * @return return the subscriber.
   */
  const SubscriberInterface<T>* GetSubscriber() const
  {
    return subscriber_.get();
  }

  /**
   * @brief Add a new subscriber.
   *
   * @param[in] id_message The add of the message.
   *
   * @return true if no problem.
   */
  virtual bool AddSubscriber(uint32_t id_message) CHK = 0;

  /**
   * @brief Remove a subscriber.
   *
   * @param[in] id_message id of the message.
   *
   * @return true if no problem.
   */
  virtual bool RemoveSubscriber(uint32_t id_message) CHK = 0;

 protected:
  /**
   * @brief Messages on hold.
   */
  std::queue<T> messages_;

  /**
   * @brief id of the next message.
   */
  uint32_t next_id_;

  /**
   * @brief The subscriber.
   */
  std::shared_ptr<SubscriberInterface<T>> subscriber_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
