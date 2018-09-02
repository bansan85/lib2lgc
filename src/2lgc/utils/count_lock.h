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

#ifndef UTILS_COUNT_LOCK_H_
#define UTILS_COUNT_LOCK_H_

#include <2lgc/compat.h>
#include <functional>
#include <mutex>

namespace llgc::utils::thread
{
template <typename M>
class CountLock
{
 public:
  CountLock(M* const ref, std::recursive_mutex* mutex_forward,
            std::function<void()> function_zero)
      : ref_(*ref),
        mutex_forward_(*mutex_forward),
        function_zero_(std::move(function_zero))
  {
    std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
    ref_++;
  }

#ifndef SWIG
  CountLock(CountLock&& other) = default;  // gcc-6.4 needs default.
  CountLock(CountLock const& other) = delete;
  CountLock& operator=(CountLock&& other) = delete;
  CountLock& operator=(CountLock const& other) = delete;
#endif  // !SWIG

  ~CountLock()
  {
    std::lock_guard<std::recursive_mutex> my_lock(mutex_forward_);
    ref_--;
    if (ref_ == 0)
    {
      function_zero_();
    }
  }

 private:
  M& ref_;
  std::recursive_mutex& mutex_forward_;
  std::function<void()> function_zero_;
};

}  // namespace llgc::utils::thread

#endif  // UTILS_COUNT_LOCK_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
