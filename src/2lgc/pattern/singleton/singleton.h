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

#ifndef PATTERN_SINGLETON_SINGLETON_H_
#define PATTERN_SINGLETON_SINGLETON_H_

#include <atomic>
#include <memory>
#include <mutex>

/**
 * @file singleton.h
 *
 * Helper to simply add a singleton to a class.
 */
namespace pattern
{
namespace singleton
{
/**
 * @brief Class that contains the getInstance for the static singleton.
 *
 * @tparam T Type of the singleton.
 */
template <class T>
class SingletonStatic
{
 public:
  static T* getInstanceStatic();
  static bool isInstanceStatic();

 private:
  static std::atomic<std::unique_ptr<T>> m_instance_static_;
  static std::mutex m_mutex_static_;
};

/**
 * @brief Class that contains the getInstance for a local singleton.
 *
 * @tparam T Type of the singleton.
 */
template <class T>
class SingletonLocal
{
 public:
  T* getInstanceLocal();
  bool isInstanceLocal();

 private:
  std::atomic<std::unique_ptr<T>> m_instance_local_;
  std::mutex m_mutex_local_;
};

}  // namespace singleton
}  // namespace pattern

#endif  // PATTERN_SINGLETON_SINGLETON_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
