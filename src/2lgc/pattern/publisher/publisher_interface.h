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

#ifndef PATTERN_PUBLISHER_PUBLISHER_INTERFACE_H_
#define PATTERN_PUBLISHER_PUBLISHER_INTERFACE_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/utils/count_lock.h>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <type_traits>

namespace google::protobuf
{
class Message;
}

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;

template <typename T, typename U>
class PublisherInterface
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  struct Options
  {
    bool add_fail_if_already_subscribed = false;
  };

  PublisherInterface();
#ifndef SWIG
  PublisherInterface(PublisherInterface &&other) = delete;
  PublisherInterface(PublisherInterface const &other) = delete;
  PublisherInterface &operator=(PublisherInterface &&other) = delete;
  PublisherInterface &operator=(PublisherInterface const &other) = delete;
#endif  // !SWIG
  virtual ~PublisherInterface() = default;

  virtual bool AddSubscriber(uint32_t id_message, U subscriber) CHK;
  bool Forward(const T &messages) CHK;
  bool ForwardPending() CHK;
  virtual bool RemoveSubscriber(uint32_t id_message, U subscriber) CHK;

  bool GetOptionFailAlreadySubscribed() const;
  void SetOptionFailAlreadySubscribed(bool value);
  llgc::utils::thread::CountLock<size_t> LockForward() CHK;

 private:
  using SubscriberMap = std::multimap<uint32_t, U>;

  SubscriberMap subscribers_;
  Options options_;
  size_t lock_forward_ = 0;
  std::recursive_mutex mutex_forward_;

  static std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
  GetConn(
      std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<T>> connector);
  static std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
  GetConn(std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>
              connector);
};

}  // namespace llgc::pattern::publisher

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/publisher_interface.cc>
#endif

#endif  // PATTERN_PUBLISHER_PUBLISHER_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
