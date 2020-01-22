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
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/connector_interface.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

template <typename T, typename S>  // T: type of message, S: service
class ConnectorPublisherGrpc : public ConnectorInterface<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  ConnectorPublisherGrpc(std::shared_ptr<SubscriberInterface<T>> subscriber,
                         std::string ip, uint16_t port);
#ifndef SWIG
  ConnectorPublisherGrpc(ConnectorPublisherGrpc &&other) = delete;
  ConnectorPublisherGrpc(ConnectorPublisherGrpc const &other) = delete;
  ConnectorPublisherGrpc &operator=(ConnectorPublisherGrpc &&other) = delete;
  ConnectorPublisherGrpc &operator=(ConnectorPublisherGrpc const &other) =
      delete;
#endif  // !SWIG
  ~ConnectorPublisherGrpc() override;

  bool Send(const T &message) override CHK;
  bool AddSubscriber(uint32_t id_message) override CHK;
  bool RemoveSubscriber(uint32_t id_message) override CHK;

 private:
  const std::string ip_;
  uint16_t port_;
  std::shared_ptr<grpc_impl::ClientReaderWriter<T, T>> stream_;
  std::shared_ptr<grpc_impl::Channel> channel_;
  grpc::ClientContext context_;
  std::unique_ptr<typename S::Stub> stub_;
  std::thread receiver_;

  bool Connect() CHK;
  void Receiver();
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/connector_publisher_grpc.cc>
#endif

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
