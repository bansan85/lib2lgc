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

#ifndef NET_LINUX_H_
#define NET_LINUX_H_

#include <sys/socket.h>
#include <cstddef>

/**
 * @brief This is all about net.
 */
namespace llgc::net
{
/**
 * @brief Class to manipulate Linux functions.
 */
class Linux
{
 public:
  /**
   * @brief Disable interrupt on signal PIPE.
   */
  static void DisableSigPipe();

  /**
   * @brief bind function with timeout.
   *
   * @param[in] sockfd Socket file descriptor.
   * @param[in] addr Address of the socket description.
   * @param[in] addrlen Length of the socket description.
   * @param[in] timeout Timeout in millisecond.
   *
   * @return false if timeout or failed to bind.
   */
  static bool Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen,
                   size_t timeout);
};

}  // namespace llgc::net

#endif  // NET_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
