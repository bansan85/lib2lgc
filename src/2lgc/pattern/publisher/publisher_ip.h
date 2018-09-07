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

#ifndef PATTERN_PUBLISHER_PUBLISHER_IP_H_
#define PATTERN_PUBLISHER_PUBLISHER_IP_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <cstdint>
#include <memory>
#include <thread>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

template <typename T>
class PublisherIp
    : public llgc::pattern::publisher::PublisherInterface<
          T, std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  explicit PublisherIp(uint16_t port);
#ifndef SWIG
  PublisherIp(PublisherIp&& other) = delete;
  PublisherIp(PublisherIp const& other) = delete;
  PublisherIp& operator=(PublisherIp&& other) = delete;
  PublisherIp& operator=(PublisherIp const& other) = delete;
#endif  // !SWIG
  ~PublisherIp() override;

  virtual bool Listen() CHK = 0;
  virtual bool Wait() CHK = 0;
  virtual void Stop() = 0;

  virtual void JoinWait();
  uint16_t GetPort() { return port_; }

 protected:
  std::thread thread_wait_;

 private:
  uint16_t port_;
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/publisher_ip.cc>
#endif

#endif  // PATTERN_PUBLISHER_PUBLISHER_IP_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
