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

#ifndef PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_OPEN_SSL_H_
#define PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_OPEN_SSL_H_

// TEMPLATE_CLASS needs it.
#include <2lgc/compat.h>
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/strategy.h>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class PublisherTcpLinux;

template <typename T>
class StrategyPublisherTcpLinuxOpenSsl
    : public llgc::pattern::Strategy<
          llgc::pattern::publisher::PublisherTcpLinux<T>>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  explicit StrategyPublisherTcpLinuxOpenSsl(
      llgc::pattern::publisher::PublisherTcpLinux<T> *server, int &client_sock,
      llgc::pattern::publisher::Presentation presentation,
      const std::string &cert, const std::string &key);
  virtual ~StrategyPublisherTcpLinuxOpenSsl() = default;

  bool Do() override CHK;

 private:
  int &client_sock_;
  llgc::pattern::publisher::Presentation presentation_;
  const std::string cert_;
  const std::string key_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_open_ssl.cc>
#endif

#endif  // PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_OPEN_SSL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
