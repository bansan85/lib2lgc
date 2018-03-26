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

#ifndef PATTERN_PUBLISHER_CONNECTOR_DIRECT_H_
#define PATTERN_PUBLISHER_CONNECTOR_DIRECT_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_remote.h>
#include <bits/stdint-uintn.h>
#include <memory>
#include <string>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
class SubscriberInterface;

template <typename M>
class PublisherBase;

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class ConnectorDirect : public ConnectorInterface,
                        public std::enable_shared_from_this<ConnectorDirect<T>>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in,out] subscriber The subscriber.
   * @param[in,out] server The server.
   */
  explicit ConnectorDirect(
      const std::shared_ptr<SubscriberInterface> &subscriber,
      const std::shared_ptr<PublisherBase<T>> &server);

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorDirect();

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorDirect(ConnectorDirect &&other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in,out] other The original.
   *
   * @return Delete function.
   */
  ConnectorDirect &operator=(ConnectorDirect &&other) = delete;

  /**
   * @brief Compare two connectors.
   *
   * @param[in,out] connector The connector to compare with this.
   *
   * @return true if same connector.
   */
  bool Equals(const ConnectorInterface *connector) const override CHK;

  /**
   * @brief Send message.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   */
  void Send(const std::shared_ptr<const std::string> &message) override;
  /**
   * @brief Listen message.
   *
   * @param message Data of the message in ProtoBuf, SerializeToString.
   */
  void Listen(const std::shared_ptr<const std::string> &message) override;

  /**
   * @brief Add a subscriber.
   *
   * @param[in] id_message The id of the message.
   *
   * @return true if no problem.
   */
  bool AddSubscriber(uint32_t id_message) override CHK;

  /**
   * @brief Remove a subscriber.
   *
   * @param[in] id_message The id of the message.
   *
   * @return true if no problem.
   */
  bool RemoveSubscriber(uint32_t id_message) override CHK;

 private:
  /**
   * @brief The server.
   */
  std::shared_ptr<PublisherBase<T>> server_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_DIRECT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
