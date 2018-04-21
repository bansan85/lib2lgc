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

#ifndef UTILS_THREAD_COUNT_LOCK_H_
#define UTILS_THREAD_COUNT_LOCK_H_

#include <functional>
#include <mutex>

/**
 * @brief Namespace for all classes related with thread.
 */
namespace llgc::utils::thread
{
/**
 * @brief Class that increment variable with constructor and decrease with
 * destrctor.
 */
template <typename M>
class CountLock
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] ref Pointer to the ref variable that will be increment on
   * constructor and decrement on destructor.
   * @param[in] mutex_forward Pointer to the mutex that will be use to use
   * thread-safe ref.
   * @param[in] function_zero Function that be be called if on destructor, ref
   * value is zero.
   */
  CountLock(M* ref, std::recursive_mutex* mutex_forward,
            std::function<void()> function_zero)
      : ref_(*ref),
        mutex_forward_(*mutex_forward),
        function_zero_(function_zero)
  {
    std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
    ref_++;
  }

  /**
   * @brief Default destructor.
   */
  ~CountLock()
  {
    std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
    ref_--;
    if (ref_ == 0)
    {
      function_zero_();
    }
  }

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  CountLock(CountLock&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  CountLock(CountLock const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  CountLock& operator=(CountLock&& other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  CountLock& operator=(CountLock const& other) & = delete;

 private:
  /**
   * @brief Options for the behavious of server.
   */
  M& ref_;

  /**
   * @brief The mutex to lock ref.
   */
  std::recursive_mutex& mutex_forward_;

  /**
   * @brief Function to execute if a destruction, the ref is zero;
   */
  std::function<void()> function_zero_;
};

}  // namespace llgc::utils::thread

#endif  // UTILS_THREAD_COUNT_LOCK_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
