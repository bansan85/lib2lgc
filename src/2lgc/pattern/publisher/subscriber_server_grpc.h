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

#ifndef PATTERN_PUBLISHER_SUBSCRIBER_SERVER_GRPC_H_
#define PATTERN_PUBLISHER_SUBSCRIBER_SERVER_GRPC_H_

#include <2lgc/pattern/publisher/subscriber_interface.h>
#include <grpcpp/impl/codegen/sync_stream.h>

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
class SubscriberServerGrpc : public SubscriberInterface<T>
{
 public:
  /**
   * @brief Default constructor
   *
   * @param[in] socket Socket to communicate with server.
   */
  explicit SubscriberServerGrpc(grpc::ServerReaderWriter<T, T>* stream) : stream_(stream) {}

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerGrpc(SubscriberServerGrpc&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerGrpc(SubscriberServerGrpc const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerGrpc& operator=(SubscriberServerGrpc&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  SubscriberServerGrpc& operator=(SubscriberServerGrpc const& other) & = delete;
#endif  // !SWIG

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] messages Message from the publisher in protobuf format.
   *
   * @return true if no problem.
   */
  bool Listen(const T& messages) override;

  /**
   * @brief Compare in connector is the same than the object.
   *
   * @param[in,out] connector The connector to compare with.
   *
   * @return true if the same.
   */
  bool Equals(const SubscriberInterface<T>& connector) const override;

  /**
   * @brief Default virtual destructor.
   */
  ~SubscriberServerGrpc() override {}

 private:
  /**
   * @brief The id of the connector.
   */
  grpc::ServerReaderWriter<T, T>* stream_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_SERVER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
