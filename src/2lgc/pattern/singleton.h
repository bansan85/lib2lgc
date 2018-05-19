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

#ifndef PATTERN_SINGLETON_H_
#define PATTERN_SINGLETON_H_

#include <2lgc/compat.h>
#include <memory>
#include <mutex>

/**
 * @file singleton.h
 *
 * Helper to simply add a singleton to a class.
 */

namespace llgc::pattern {

/**
 * @brief Class that contains the getInstance for a local singleton.
 *
 * @tparam T Type of the singleton.
 */
template <class T>
class Singleton
{
 public:
  /**
   * @brief Tell if an instance is allocated.
   *
   * @return true if allocated.
   */
  bool IsInstance();

  /**
   * @brief Get the instance and allocate it if not already.
   *
   * @return Return an instance never null.
   */
  std::shared_ptr<T> GetInstance();

 private:
  /**
   * @brief A mutex to implement the singleton.
   */
  std::recursive_mutex mutex_;

  /**
   * @brief Store the instance of the singleton
   */
  std::shared_ptr<T> instance_;
};

}

#endif  // PATTERN_SINGLETON_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
