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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_H_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_H_

// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <atomic>
#include <cstdint>
#include <map>
#include <thread>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class PublisherTcp : public PublisherIp<T>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  explicit PublisherTcp(uint16_t port);
#ifndef SWIG
  PublisherTcp(PublisherTcp&& other) = delete;
  PublisherTcp(PublisherTcp const& other) = delete;
  PublisherTcp& operator=(PublisherTcp&& other) = delete;
  PublisherTcp& operator=(PublisherTcp const& other) = delete;
#endif  // !SWIG
  ~PublisherTcp() override;

  void JoinWait() override;
  void Stop() override;

 protected:
  std::atomic<bool> disposing_;
  std::map<int, std::thread> thread_sockets_;

 private:
#ifndef SWIG
  virtual void AddSubscriberLocal(
      int socket, decltype(std::declval<T>().msg(0)) message) = 0;
#endif  // !SWIG
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/publisher_tcp.cc>
#endif

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
