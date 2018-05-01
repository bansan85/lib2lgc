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

#ifndef PATTERN_PUBLISHER_SUBSCRIBER_SERVER_TCP_H_
#define PATTERN_PUBLISHER_SUBSCRIBER_SERVER_TCP_H_

#include <2lgc/pattern/publisher/subscriber_interface.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class SubscriberServerTcp : public SubscriberInterface<T>
{
 public:
  /**
   * @brief Default constructor
   *
   * @param[in] socket Socket to communicate with server.
   */
  explicit SubscriberServerTcp(int socket) : socket_(socket) {}

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp(SubscriberServerTcp&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp(SubscriberServerTcp const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp& operator=(SubscriberServerTcp&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerTcp& operator=(SubscriberServerTcp const& other) & = delete;
  /**
   * @brief Receive message from publisher.
   *
   * @param[in] message message from the publisher in protobuf format.
   */

  void Listen(const T& message) override;

  /**
   * @brief Compare in connector is the same than the object.
   *
   * @param[in,out] connector The connector to compare with.
   *
   * @return true if the same.
   */
  bool Equals(const SubscriberInterface<T>& connector) const override
      __attribute__((pure));

  /**
   * @brief Default virtual destructor.
   */
  ~SubscriberServerTcp() override {}

 private:
  /**
   * @brief The id of the connector.
   */
  const int socket_;  // NS
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_SERVER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
