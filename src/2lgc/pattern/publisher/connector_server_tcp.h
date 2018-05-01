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

#ifndef PATTERN_PUBLISHER_CONNECTOR_SERVER_TCP_H_
#define PATTERN_PUBLISHER_CONNECTOR_SERVER_TCP_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

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
class ConnectorServerTcp : public ConnectorInterface<T>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] subscriber The subscriber.
   * @param[in] ip The IP of the server.
   * @param[in] port The port of the server.
   */
  ConnectorServerTcp(std::shared_ptr<SubscriberInterface<T>> subscriber,
                     std::string ip, uint16_t port);

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorServerTcp();

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorServerTcp(ConnectorServerTcp &&other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorServerTcp(ConnectorServerTcp const &other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorServerTcp &operator=(ConnectorServerTcp &&other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorServerTcp &operator=(ConnectorServerTcp const &other) & = delete;

  /**
   * @brief Compare two connectors.
   *
   * @param[in] connector The connector to compare with this.
   *
   * @return true if same connector.
   */
  bool Equals(const ConnectorInterface<T> &connector) const override CHK;

  /**
   * @brief Send message to the publisher.
   *
   * @param[in] message Data of the message in ProtoBuf, SerializeToString.
   *
   * @return true if no problem.
   */
  bool Send(const std::string &message) override CHK;

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

  /**
   * @brief The function used by the thread that receive message from server.
   *
   * Need to be public so thread can use it. Protected is not possible.
   */
  void Receiver();

 protected:
  /**
   * @brief The IP of the server.
   */
  std::string ip_;

  /**
   * @brief The port of the server.
   */
  uint16_t port_;

  /**
   * @brief Socket to the server.
   */
  int socket_;  // NS

  /**
   * @brief Start connection with server.
   *
   * @return true if no problem.
   */
  virtual bool Connect() CHK = 0;

  /**
   * @brief Thread that listen the server.
   */
  std::thread receiver_;

 private:
  /**
   * @brief If thread in trying to stop.
   */
  std::atomic<bool> disposing_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_SERVER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
