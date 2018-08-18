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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_H_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/impl/codegen/sync_stream.h>

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
template <typename T, typename S> // T: type of message, S: service
class ConnectorPublisherGrpc : public ConnectorInterface<T>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] subscriber The subscriber.
   * @param[in] ip The IP of the server.
   * @param[in] port The port of the server.
   */
  ConnectorPublisherGrpc(std::shared_ptr<SubscriberInterface<T>> subscriber,
                        std::string ip, uint16_t port);

  /**
   * @brief Default virtual destructor.
   */
  ~ConnectorPublisherGrpc() override;

#ifndef SWIG
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorPublisherGrpc(ConnectorPublisherGrpc &&other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorPublisherGrpc(ConnectorPublisherGrpc const &other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorPublisherGrpc &operator=(ConnectorPublisherGrpc &&other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorPublisherGrpc &operator=(ConnectorPublisherGrpc const &other) & =
      delete;
#endif  // !SWIG

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
   *
   * @dotfile pattern/publisher/publisher_tcp_send_message.dot
   */
  bool Send(const T &message) override CHK;

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

  std::unique_ptr<typename S::Stub> stub_;
  std::shared_ptr<grpc::ClientReaderWriter<T, T> > stream_;
  std::shared_ptr<grpc::Channel> channel_;
  grpc::ClientContext context_;

  /**
   * @brief Start connection with server.
   *
   * @return true if no problem.
   */
  bool Connect() CHK;

  /**
   * @brief Thread that listen the server.
   */
  std::thread receiver_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */