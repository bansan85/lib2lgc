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
