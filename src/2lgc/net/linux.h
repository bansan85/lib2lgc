/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
