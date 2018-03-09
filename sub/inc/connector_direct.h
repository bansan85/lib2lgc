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

#ifndef CONNECTOR_DIRECT_H_
#define CONNECTOR_DIRECT_H_

#include <bits/stdint-uintn.h>
#include <memory>
#include <string>
#include "compat.h"
#include "connector_interface.h"

namespace pattern
{
namespace publisher
{
class SubscriberInterface;
}
}

namespace pattern
{
namespace publisher
{
template <typename M>
class PublisherBase;
}
}

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
template <typename T>
class ConnectorDirect : public ConnectorInterface
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
      const std::shared_ptr<PublisherBase<T>> &server)
      : ConnectorInterface(subscriber), server_(server)
  {
  }

  /**
   * @brief Compare two connectors.
   *
   * @param[in,out] connector The connector to compare with this.
   *
   * @return true if same connector.
   */
  bool Equals(const ConnectorInterface *connector) const override CHK;

  /**
   * @brief Add a subscriber.
   *
   * @param[in] id_message The id of the message.
   * @param[in] subscriber
   *
   * @return true if no problem.
   */
  bool AddSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) override CHK;

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
   * @brief Remove a subscriber.
   *
   * @param[in] id_message The id of the message.
   * @param[in] subscriber The subscriber.
   *
   * @return true if no problem.
   */
  bool RemoveSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) override CHK;

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorDirect() {}

 private:
  /**
   * @brief The server.
   */
  std::shared_ptr<PublisherBase<T>> server_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // CONNECTOR_DIRECT_H_
