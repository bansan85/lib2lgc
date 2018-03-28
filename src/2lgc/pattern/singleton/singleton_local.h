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

#ifndef PATTERN_SINGLETON_SINGLETON_LOCAL_H_
#define PATTERN_SINGLETON_SINGLETON_LOCAL_H_

#include <memory>
#include <mutex>
#include <string>

/**
 * @file singleton_local.h
 *
 * Helper to simply add a singleton to a class.
 */
namespace llgc::pattern::singleton
{
/**
 * @brief Class that contains the getInstance for a local singleton.
 *
 * @tparam T Type of the singleton.
 */
template <class T>
class Local
{
 public:
  /**
   * @brief Get the instance and allocate it if not already.
   *
   * @return Return an instance never null.
   */
  std::shared_ptr<T> GetInstanceLocal();

  /**
   * @brief Tell if an instance is allocated.
   *
   * @return true if allocated.
   */
  bool IsInstanceLocal();

  /**
   * @brief Send the message to all subscriber.
   *
   * @param[in] message The message to send.
   */
  void Forward(const std::shared_ptr<const std::string>& message);

 private:
  /**
   * @brief Store the instance of the singleton
   */
  std::shared_ptr<T> instance_local_;

  /**
   * @brief A mutex to implement the singleton.
   */
  std::recursive_mutex mutex_local_;
};

}  // namespace llgc::pattern::singleton

#endif  // PATTERN_SINGLETON_SINGLETON_LOCAL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
