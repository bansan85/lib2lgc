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

/**
 * @brief This is all about net.
 */
START_NAMESPACE2(llgc, net)

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
  static bool Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen,
                   size_t timeout) CHK;

  /**
   * @brief send function with error handle.
   *
   * @param[in] sockfd Socket file descriptor.
   * @param[in] message Message to send.
   *
   * @return true if no problem.
   */
  static bool Send(int sockfd, const std::string& message) CHK;

  /**
   * @brief Execute a function. If this function return -1, check if errno ==
   * EINTR. If so, reexecute the function.
   * RepeteOnEintr should be used with select and accept4.
   *
   * @param[in] function Function to execute.
   *
   * @return The value returned by function.
   */
  static int RepeteOnEintr(const std::function<int()>& function) CHK;  // NS

  /**
   * @brief Class that auto close a socket on destructor.
   */
  class AutoCloseSocket
  {
   public:
    /**
     * @brief Constructor with the socket to handle.
     *
     * @param[in] socket The socket.
     */
    explicit AutoCloseSocket(int* socket);

    /**
     * @brief Dont delete the socket on destrutor.
     */
    void DontDeleteSocket();

    /**
     * @brief Destructor that close the socket.
     */
    ~AutoCloseSocket();

   private:
    /**
     * @brief The socket to handle.
     */
    int* socket_;  // NS

    /**
     * @brief If the socket must be deleted on destructor. True by default.
     */
    bool delete_socket_;
  };
};

END_NAMESPACE2(llgc, net)

#endif  // NET_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
