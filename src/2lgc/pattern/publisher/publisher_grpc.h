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
}
namespace grpc
{
class ServerContext;
}
namespace grpc
{
template <class W, class R>
class ServerReaderWriter;
}
namespace llgc
{
namespace pattern
{
namespace publisher
{
template <typename T>
class ConnectorSubscriberGrpc;
}
}  // namespace pattern
}  // namespace llgc
namespace llgc
{
namespace pattern
{
namespace publisher
{
template <typename T>
class SubscriberServerGrpc;
}
}  // namespace pattern
}  // namespace llgc

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T, typename S>  // S : class of service
class PublisherGrpcService : public S, public PublisherIp<T>
{
 public:
  explicit PublisherGrpcService(PublisherIp<T>* parent)
      : llgc::pattern::publisher::PublisherIp<T>(parent->GetPort()),
        parent_(*parent)
  {
  }

  // One endless thread for each connexion.
  // But same 'this' for everyone.
  grpc::Status Talk(grpc::ServerContext* /*context*/,
                    grpc::ServerReaderWriter<T, T>* stream) override
  {
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

  bool Listen() override CHK { return parent_.Listen(); }

  bool Wait() override CHK { return parent_.Wait(); }

  void Stop() override { parent_.Stop(); }

 private:
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
  PublisherGrpcService<T, S> service_;

  std::unique_ptr<grpc::Server> server_;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
