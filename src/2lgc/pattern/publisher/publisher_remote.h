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

#ifndef PATTERN_PUBLISHER_PUBLISHER_REMOTE_H_
#define PATTERN_PUBLISHER_PUBLISHER_REMOTE_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/publisher_base.h>
#include <cstdint>
#include <map>
#include <memory>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
class ConnectorInterface;

/**
 * @brief Server that will be used to managed subscribers and to keep and send
 *        messages.
 */
template <typename M>
class PublisherRemote : public PublisherBase<M>
{
 public:
  /**
   * @brief Default constructor.
   */
  PublisherRemote();

  /**
   * @brief Default destructor.
   */
  ~PublisherRemote() override;

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  PublisherRemote(PublisherRemote&& other) = delete;

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  PublisherRemote(PublisherRemote const& other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in,out] other The original.
   *
   * @return Delete function.
   */
  PublisherRemote& operator=(PublisherRemote&& other) = delete;

  /**
   * @brief Delete copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  PublisherRemote& operator=(PublisherRemote const& other) & = delete;

  /**
   * @brief Add a subscriber to the server.
   *
   * @param id_message The message to subscribe.
   * @param subscriber The subscriber.
   *
   * @return true if success. May failed if add_fail_if_already_subscribed is
   * true and the subscriber is already registered.
   */
  bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) override CHK;

  /**
   * @brief Remove a subscriber of the server.
   *
   * @param id_message The message to unsubscriber.
   * @param subscriber The subscriber.
   *
   * @return true if subscriber is unsubscribed successfully.
   *         May failed if suscriber is not subscribe to the specific message.
   */
  bool RemoveSubscriber(
      uint32_t id_message,
      const std::shared_ptr<ConnectorInterface>& subscriber) override CHK;

 protected:
  // using needed because of template inheritance.
  using PublisherBase<M>::subscribers_;
  /**
   * @brief definition of the multimap.
   */
  using SubscriberMap =
      std::multimap<uint32_t, std::shared_ptr<ConnectorInterface>>;

 private:
  /**
   * @brief Port to communicate with.
   */
  uint16_t port_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_REMOTE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
