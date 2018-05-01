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

#ifndef NET_TCP_SERVER_H_
#define NET_TCP_SERVER_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/publisher/publisher.h>
#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <thread>

/**
 * @brief This is all about net.
 */
namespace llgc::net
{
/**
 * @brief Interface to create a TCP server.
 *
 * @tparam T Message from protobuf.
 */
template <typename T>
class TcpServer : public llgc::pattern::publisher::Publisher<T>
{
 public:
  /**
   * @brief Constructor with port for the TCP server.
   *
   * @param[in] port The port to listen from.
   */
  explicit TcpServer(uint16_t port);

  /**
   * @brief Destructor. Make sure that thread is finished.
   */
  virtual ~TcpServer();

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer(TcpServer&& other) = delete;
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer(TcpServer const& other) = delete;
  /**
   * @brief Delete move operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer& operator=(TcpServer&& other) & = delete;
  /**
   * @brief Delete copy operator.
   *
   * @param[in] other Don't care.
   *
   * @return Nothing.
   */
  TcpServer& operator=(TcpServer const& other) & = delete;

  /**
   * @brief Start the server and the listening the port.
   *
   * @return true if no problem.
   */
  virtual bool Listen() CHK = 0;

  /**
   * @brief Wait for client.
   *
   * @param[in] wait_function function.
   *
   * @return true if no problem.
   */
  virtual bool Wait(
      std::function<void(llgc::net::TcpServer<T>*, int)> wait_function) CHK = 0;

  /**
   * @brief If thread is in stopping.
   *
   * @return true if thread in trying to stop.
   */
  bool IsStopping();

  /**
   * @brief Stop the thread.
   */
  void Stop();

  /**
   * @brief Join the waiting thread.
   */
  void JoinWait();

 protected:
  /**
   * @brief Port to listen from.
   */
  uint16_t port_;

  /**
   * @brief Thread that run the Wait function and add socket to thread_sockets_.
   */
  std::thread thread_wait_;

  /**
   * @brief Store thread based on the socket file descriptor.
   */
  std::map<int, std::thread> thread_sockets_;  // NS

 private:
  /**
   * @brief If thread in trying to stop.
   */
  std::atomic<bool> disposing_;
};

}  // namespace llgc::net

#endif  // NET_TCP_SERVER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
