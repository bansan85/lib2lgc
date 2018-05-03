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

#include <2lgc/net/tcp_server.h>
#include <2lgc/pattern/publisher/publisher.h>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorInterface;
}

template <typename T>
llgc::net::TcpServer<T>::TcpServer(uint16_t port)
    : llgc::pattern::publisher::Publisher<
          T,
          std::shared_ptr<llgc::pattern::publisher::ConnectorInterface<T>>>(),
      port_(port),
      disposing_(false)
{
}

template <typename T>
llgc::net::TcpServer<T>::~TcpServer()
{
  // Can't destroy a thread if it's still running.
  JoinWait();
}

template <typename T>
bool llgc::net::TcpServer<T>::IsStopping()
{
  return disposing_;
}

template <typename T>
void llgc::net::TcpServer<T>::Stop()
{
  disposing_ = true;
}

template <typename T>
void llgc::net::TcpServer<T>::JoinWait()
{
  if (thread_wait_.joinable())
  {
    thread_wait_.join();
  }
  for (auto& thread_i : thread_sockets_)
  {
    if (thread_i.second.joinable())
    {
      thread_i.second.join();
    }
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
