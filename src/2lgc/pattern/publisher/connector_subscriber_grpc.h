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

#ifndef PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_GRPC_H_
#define PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_GRPC_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <grpcpp/support/sync_stream_impl.h>
#include <memory>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

template <typename T>
class ConnectorSubscriberGrpc : public ConnectorSubscriber<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  ConnectorSubscriberGrpc(std::shared_ptr<SubscriberInterface<T>> subscriber,
                          grpc_impl::ServerReaderWriter<T, T> *stream);
#ifndef SWIG
  ConnectorSubscriberGrpc(ConnectorSubscriberGrpc &&other) = delete;
  ConnectorSubscriberGrpc(ConnectorSubscriberGrpc const &other) = delete;
  ConnectorSubscriberGrpc &operator=(ConnectorSubscriberGrpc &&other) = delete;
  ConnectorSubscriberGrpc &operator=(ConnectorSubscriberGrpc const &other) =
      delete;
#endif  // !SWIG
  ~ConnectorSubscriberGrpc() override = default;

  bool Send(const T &message) override CHK;

 private:
  grpc_impl::ServerReaderWriter<T, T> *stream_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/connector_subscriber_grpc.cc>
#endif

#endif  // PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
