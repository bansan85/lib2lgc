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

// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/subscriber_interface.h>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberServerGrpc : public SubscriberInterface<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  explicit SubscriberServerGrpc(grpc_impl::ServerReaderWriter<T, T> *stream)
      : stream_(stream)
  {
  }
#ifndef SWIG
  SubscriberServerGrpc(SubscriberServerGrpc &&other) = delete;
  SubscriberServerGrpc(SubscriberServerGrpc const &other) = delete;
  SubscriberServerGrpc &operator=(SubscriberServerGrpc &&other) = delete;
  SubscriberServerGrpc &operator=(SubscriberServerGrpc const &other) = delete;
#endif  // !SWIG
  ~SubscriberServerGrpc() override = default;

  bool Listen(const T &messages) override;
  bool Equals(const SubscriberInterface<T> &connector) const override;

 private:
  grpc_impl::ServerReaderWriter<T, T> *stream_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/subscriber_server_grpc.cc>
#endif

#endif  // PATTERN_PUBLISHER_SUBSCRIBER_SERVER_GRPC_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
