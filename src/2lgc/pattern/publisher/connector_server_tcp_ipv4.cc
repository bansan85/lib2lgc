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

#include <2lgc/pattern/publisher/connector_server_tcp.h>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <utility>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

template <typename T>
llgc::pattern::publisher::ConnectorServerTcpIpv4<T>::ConnectorServerTcpIpv4(
    std::shared_ptr<SubscriberInterface<T>> subscriber, const std::string ip,
    uint16_t port)
    : ConnectorServerTcp<T>(subscriber, ip, port)
{
}

template <typename T>
llgc::pattern::publisher::ConnectorServerTcpIpv4<T>::~ConnectorServerTcpIpv4() =
    default;

template <typename T>
bool llgc::pattern::publisher::ConnectorServerTcpIpv4<T>::Connect()
{
  if (this->socket_ != -1)
  {
    return true;
  }

  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socket_ == -1)
  {
    return false;
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sockaddr_in server;  // NOLINT(hicpp-member-init)
  server.sin_addr.s_addr = inet_addr(this->ip_.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(this->port_);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (connect(this->socket_, reinterpret_cast<struct sockaddr *>(&server),
              sizeof(server)) < 0)
  {
    close(this->socket_);
    this->socket_ = -1;
    return false;
  }

  std::thread t(&ConnectorServerTcp<T>::Receiver, this);
  this->receiver_ = std::move(t);

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
