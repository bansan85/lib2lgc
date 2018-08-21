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
#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <grpcpp/impl/codegen/status.h>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>
namespace grpc
{
class Server;

class ServerContext;

template <class W, class R>
class ServerReaderWriter;
}  // namespace grpc

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorSubscriberGrpc;

template <typename T>
class SubscriberServerGrpc;
}  // namespace llgc::pattern::publisher

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Service that will be run by each client.
 *
 * @tparam T Class of the protobuf message.
 * @tparam S Class of the service.
 */
template <typename T, typename S>
class PublisherGrpcService : public S, public PublisherIp<T>
{
 public:
  /**
   * @brief Constructor.
   *
   * @param[in,out] parent gRPC publisher.
   */
  explicit PublisherGrpcService(PublisherIp<T>* parent)
      : llgc::pattern::publisher::PublisherIp<T>(parent->GetPort()),
        parent_(*parent)
  {
  }

  /**
   * @brief Wait, read and forward messages from a client to subscribers.
   *
   * @param[in] context Don't care.
   * @param[in] stream Listen from this stream.
   *
   * @return The return value when the stream is closed by the server.
   */
  grpc::Status Talk(grpc::ServerContext* __attribute__((unused)) context,
                    grpc::ServerReaderWriter<T, T>* stream) override
  {
    // One endless thread for each connexion.
    // But same 'this' for everyone.
    T messages;
    while (stream->Read(&messages))
    {
      for (int i = 0; i < messages.msg_size(); i++)
      {
        auto& message = messages.msg(i);

        auto enumeration = message.data_case();

        if (enumeration ==
            std::remove_reference<decltype(message)>::type::kAddSubscriber)
        {
          std::shared_ptr<llgc::pattern::publisher::SubscriberServerGrpc<T>>
              subscriber = std::make_shared<
                  llgc::pattern::publisher::SubscriberServerGrpc<T>>(stream);
          std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>
              connector = std::make_shared<
                  llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>(
                  subscriber, stream);
          // Ignore return value.
          // Can fail if SetOptionFailAlreadySubscribed is set.
          if (parent_.AddSubscriber(message.add_subscriber().id_message(),
                                    connector))
          {
            // Empty body
          }
        }
        else if (enumeration == std::remove_reference<decltype(
                                    message)>::type::kRemoveSubscriber)
        {
          std::shared_ptr<llgc::pattern::publisher::SubscriberServerGrpc<T>>
              subscriber = std::make_shared<
                  llgc::pattern::publisher::SubscriberServerGrpc<T>>(stream);
          std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>
              connector = std::make_shared<
                  llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>(
                  subscriber, stream);
          // Don't assert if failed because it will stop server.
          BUGCONT(std::cout,
                  parent_.RemoveSubscriber(
                      message.remove_subscriber().id_message(), connector),
                  grpc::Status::CANCELLED);
        }
      }

      BUGCONT(std::cout, parent_.Forward(messages), grpc::Status::CANCELLED);
    }

    return grpc::Status::OK;
  }

  /**
   * @brief Run the Listen function of the server.
   *
   * @return The return value from the server.
   */
  bool Listen() override CHK { return parent_.Listen(); }

  /**
   * @brief Run the Wait function of the server.
   *
   * @return The return value from the server.
   */
  bool Wait() override CHK { return parent_.Wait(); }

  /**
   * @brief Run the Stop function of the server.
   */
  void Stop() override { parent_.Stop(); }

 private:
  /**
   * @brief Reference to the gRPC publisher.
   */
  PublisherIp<T>& parent_;
};

/**
 * @brief Interface to create a TCP server.
 *
 * @tparam T Message from protobuf.
 *
 * @dotfile pattern/publisher/publisher_tcp.dot
 */
template <typename T, typename S>
class PublisherGrpc : public PublisherIp<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit PublisherGrpc(uint16_t port);

  /**
   * @brief Destructor. Make sure that thread is finished.
   */
  ~PublisherGrpc() override;

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

 private:
  /**
   * @brief Service that will be run by the server.
   */
  PublisherGrpcService<T, S> service_;

  /**
   * @brief Server gRPC.
   */
  std::unique_ptr<grpc::Server> server_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
