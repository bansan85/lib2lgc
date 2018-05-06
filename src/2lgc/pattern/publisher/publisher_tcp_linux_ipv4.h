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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_IPV4_H_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_IPV4_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <cstdint>

/**
 * @brief This is all about net.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Interface to create a TCP server.
 */
template <typename T>
class PublisherTcpLinuxIpv4 : public PublisherTcpLinux<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit PublisherTcpLinuxIpv4(uint16_t port);

  /**
   * @brief Start the server and the listening the port.
   *
   * @return true if no problem.
   */
  bool Listen() override CHK;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_IPV4_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
