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

#include <2lgc/net/linux.h>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <iostream>
#include <type_traits>

void llgc::net::Linux::DisableSigPipe()
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  struct sigaction sig;  // NOLINT(hicpp-member-init)

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
  sig.sa_handler = SIG_IGN;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  sig.sa_flags = 0;
  sigemptyset(&sig.sa_mask);
  sigaction(SIGPIPE, &sig, nullptr);
}

bool llgc::net::Linux::Bind(int sockfd, const struct sockaddr *addr,
                            socklen_t addrlen, size_t timeout)
{
  int retval_bind = bind(sockfd, addr, addrlen);  // NS
  // We have to wait the end of the timeout.
  if (retval_bind == -1 && errno == EADDRINUSE)
  {
    std::cout << "Opening TCP port socket in use. Please wait…" << std::endl;

    // setsockopt(SO_REUSEADDR) doesn't work.
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    do
    {
      retval_bind = bind(sockfd, addr, addrlen);
      end = std::chrono::system_clock::now();
    } while (
        retval_bind == -1 && errno == EADDRINUSE &&
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count()) < timeout);
  }

  return retval_bind == 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
