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

#include <2lgc/compat.h>
#include <sys/socket.h>
#include <cstddef>
#include <functional>
#include <string>

namespace llgc::net
{
class Linux
{
 public:
  static bool DisableSigPipe() CHK;
  static bool Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen,
                   size_t timeout) CHK;
  static bool Send(int sockfd, const std::string &message) CHK;
  static int RepeteOnEintr(const std::function<int()> &function) CHK;

  class AutoCloseSocket
  {
   public:
    explicit AutoCloseSocket(int *socket);

#ifndef SWIG
    AutoCloseSocket(AutoCloseSocket &&other) = delete;
    AutoCloseSocket(AutoCloseSocket const &other) = delete;
    AutoCloseSocket &operator=(AutoCloseSocket &&other) = delete;
    AutoCloseSocket &operator=(AutoCloseSocket const &other) = delete;
#endif  // !SWIG

    ~AutoCloseSocket();
    void DontDeleteSocket();

   private:
    int *socket_;
    bool delete_socket_ = true;
  };
};

}  // namespace llgc::net

#endif  // NET_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
