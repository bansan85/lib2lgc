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

#ifndef CONNECTOR_INTERFACE_H_
#define CONNECTOR_INTERFACE_H_

// C++ system
#include <memory>
#include <queue>

#include "subscriber_interface.h"

namespace pattern
{
namespace publisher
{
/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
class ConnectorInterface
{
 public:
  explicit ConnectorInterface(
      const std::shared_ptr<SubscriberInterface> &subscriber)
      : messages_(), next_id_(0), subscriber_(subscriber)
  {
  }

  virtual bool Equals(const ConnectorInterface *connector) const CHK = 0;

  virtual bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) CHK = 0;

  /**
   * @brief Send message.
   *
   * @param id_message The id of the message.
   * @param data Data of the message in ProtoBuf, SerializeToString.
   */
  virtual void Send(const std::shared_ptr<const std::string> &message) = 0;

  virtual void Listen(const std::shared_ptr<const std::string> &message) = 0;

  virtual bool RemoveSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) CHK = 0;

  const SubscriberInterface *GetSubscriber() const { return subscriber_.get(); }

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorInterface() {}

 protected:
  // Pair with the id of the event and the arguments.
  std::queue<std::pair<uint32_t, std::shared_ptr<const std::string>>> messages_;
  // id of the next message.
  uint32_t next_id_;
  std::shared_ptr<SubscriberInterface> subscriber_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // CONNECTOR_INTERFACE_H_
