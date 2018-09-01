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

#ifndef PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_
#define PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_

#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <memory>

namespace llgc::pattern::publisher
{
template <typename T>
class PublisherDirect
    : public PublisherInterface<T, std::weak_ptr<ConnectorInterface<T>>>
{
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T must be a descendant of ::google::protobuf::Message.");

 public:
  PublisherDirect() = default;
#ifndef SWIG
  PublisherDirect(PublisherDirect&& other) = delete;
  PublisherDirect(PublisherDirect const& other) = delete;
  PublisherDirect& operator=(PublisherDirect&& other) = delete;
  PublisherDirect& operator=(PublisherDirect const& other) = delete;
#endif  // !SWIG
  ~PublisherDirect() override = default;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
