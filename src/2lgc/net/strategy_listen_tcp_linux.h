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

#ifndef NET_STRATEGY_LISTEN_TCP_LINUX_H_
#define NET_STRATEGY_LISTEN_TCP_LINUX_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/strategy.h>
#include <functional>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::net
{
template <typename T, typename U>
class StrategyListenTcpLinux : public llgc::pattern::Strategy<U>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  StrategyListenTcpLinux(U *server, int *client_sock,
                         std::function<bool(const T &)> function);
#ifndef SWIG
  StrategyListenTcpLinux(StrategyListenTcpLinux &&other) = delete;
  StrategyListenTcpLinux(StrategyListenTcpLinux const &other) = delete;
  StrategyListenTcpLinux &operator=(StrategyListenTcpLinux &&other) = delete;
  StrategyListenTcpLinux &operator=(StrategyListenTcpLinux const &other) =
      delete;
#endif  // !SWIG
  ~StrategyListenTcpLinux() override = default;

  bool Do() override CHK;

 private:
  int *client_sock_;
  std::function<bool(const T &)> function_;
};

}  // namespace llgc::net

#ifndef TEMPLATE_CLASS
#include <2lgc/net/strategy_listen_tcp_linux.cc>
#endif

#endif  // NET_STRATEGY_LISTEN_TCP_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
