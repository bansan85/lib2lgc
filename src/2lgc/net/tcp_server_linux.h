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

#ifndef NET_TCP_SERVER_LINUX_H_
#define NET_TCP_SERVER_LINUX_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/net/tcp_server.h>
#include <cstdint>
#include <functional>

/**
 * @brief This is all about net.
 */
namespace llgc::net
{
/**
 * @brief Interface to create a TCP server.
 */
template <typename T>
class TcpServerLinux : public TcpServer<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit TcpServerLinux(uint16_t port);

  /**
   * @brief Wait for client.
   *
   * @param[in] wait_function function.
   *
   * @return true if no problem.
   */
  bool Wait(std::function<void(llgc::net::TcpServer<T>*, int)> wait_function)
      override CHK;

 protected:
  /**
   * @brief Socket file descriptor.
   */
  int sockfd_;  // NS
};

}  // namespace llgc::net

#endif  // NET_TCP_SERVER_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
