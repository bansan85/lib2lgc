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

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Server that will be used to managed subscribers and to keep and send
 *        messages.
 *
 * @dotfile pattern/publisher/publisher_direct.dot
 */
template <typename T>
class PublisherDirect
    : public PublisherInterface<T, std::weak_ptr<ConnectorInterface<T>>>
{
 public:
  /**
   * @brief Default constructor.
   */
  PublisherDirect() = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  PublisherDirect(PublisherDirect&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  PublisherDirect(PublisherDirect const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  PublisherDirect& operator=(PublisherDirect&& other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  PublisherDirect& operator=(PublisherDirect const& other) & = delete;
#endif  // !SWIG
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
