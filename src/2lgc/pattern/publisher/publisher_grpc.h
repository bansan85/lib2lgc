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

#ifndef PATTERN_PUBLISHER_PUBLISHER_GRPC_H_
#define PATTERN_PUBLISHER_PUBLISHER_GRPC_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <atomic>
#include <cstdint>
#include <map>
#include <memory>
#include <thread>
#include <type_traits>
#include <grpcpp/grpcpp.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Interface to create a TCP server.
 *
 * @tparam T Message from protobuf.
 *
 * @dotfile pattern/publisher/publisher_tcp.dot
 */
template <typename T>
class PublisherGrpc : public PublisherIp<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  PublisherGrpc(uint16_t port, std::unique_ptr<grpc::Service> service);

  /**
   * @brief Destructor. Make sure that thread is finished.
   */
  virtual ~PublisherGrpc() override;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherGrpc(PublisherGrpc&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherGrpc(PublisherGrpc const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherGrpc& operator=(PublisherGrpc&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  PublisherGrpc& operator=(PublisherGrpc const& other) & = delete;
#endif  // !SWIG

  /**
   * @brief Join the waiting thread.
   */
  void JoinWait() override;

  /**
   * @brief Start the server and the listening the port.
   *
   * @return true if no problem.
   */
  bool Listen() override CHK;

  /**
   * @brief Wait for client.
   *
   * @return true if no problem.
   */
  bool Wait() override CHK;

  /**
   * @brief Stop the thread.
   */
  void Stop() override;

 private:
  grpc::ServerBuilder builder_;

  std::unique_ptr<grpc::Service> service_;

  std::unique_ptr<grpc::Server> server_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
