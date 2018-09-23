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

#ifndef NET_STRATEGY_LISTEN_OPEN_SSL_H_
#define NET_STRATEGY_LISTEN_OPEN_SSL_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/net/openssl.h>
#include <2lgc/pattern/strategy.h>
#include <openssl/ossl_typ.h>
#include <memory>
#include <string>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class PublisherTcpLinux;
}

namespace llgc::net
{
template <typename T>
class StrategyListenOpenSsl
    : public llgc::pattern::Strategy<
          llgc::pattern::publisher::PublisherTcpLinux<T>>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  StrategyListenOpenSsl(llgc::pattern::publisher::PublisherTcpLinux<T> *server,
                        int *client_sock,
                        llgc::net::OpenSsl::Presentation presentation,
                        std::string cert, std::string key,
                        std::weak_ptr<SSL_CTX> ctx, std::weak_ptr<SSL> ssl);
#ifndef SWIG
  StrategyListenOpenSsl(StrategyListenOpenSsl &&other) = delete;
  StrategyListenOpenSsl(StrategyListenOpenSsl const &other) = delete;
  StrategyListenOpenSsl &operator=(StrategyListenOpenSsl &&other) = delete;
  StrategyListenOpenSsl &operator=(StrategyListenOpenSsl const &other) = delete;
#endif  // !SWIG
  ~StrategyListenOpenSsl() override = default;

  bool Do() override CHK;

 private:
  int *client_sock_;
  llgc::net::OpenSsl::Presentation presentation_;
  const std::string cert_;
  const std::string key_;
  std::weak_ptr<SSL_CTX> ctx_;
  std::weak_ptr<SSL> ssl_;
};

}  // namespace llgc::net

#ifndef TEMPLATE_CLASS
#include <2lgc/net/strategy_listen_open_ssl.cc>
#endif

#endif  // NET_STRATEGY_LISTEN_OPEN_SSL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
