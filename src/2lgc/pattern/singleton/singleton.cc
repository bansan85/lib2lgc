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

#include <2lgc/pattern/singleton/singleton.h>

template <class T>
T* llgc::pattern::singleton::Static<T>::GetInstanceStatic()
{
  T* retval = m_instance_static_.load(std::memory_order_acquire).get();

  if (retval == nullptr)
  {
    std::lock_guard<std::mutex> lock(m_mutex_static_);
    retval = m_instance_static_.load(std::memory_order_relaxed).get();
    if (retval == nullptr)
    {
      std::unique_ptr<T> unique_ret_val = std::make_unique<T>();
      retval = unique_ret_val.release();
      m_instance_static_.store(retval, std::memory_order_release);
    }
  }
  return retval;
}

template <class T>
bool llgc::pattern::singleton::Static<T>::IsInstanceStatic()
{
  T* retval = m_instance_static_.load(std::memory_order_acquire);

  if (retval == nullptr)
  {
    std::lock_guard<std::mutex> lock(m_mutex_static_);
    retval = m_instance_static_.load(std::memory_order_relaxed);
    if (retval == nullptr)
    {
      m_instance_static_.store(retval, std::memory_order_release);
    }
  }
  return retval != nullptr;
}

template <class T>
T* llgc::pattern::singleton::Local<T>::GetInstanceLocal()
{
  T* retval = m_instance_local_.load(std::memory_order_acquire).get();

  if (retval == nullptr)
  {
    std::lock_guard<std::mutex> lock(m_mutex_local_);
    retval = m_instance_local_.load(std::memory_order_relaxed).get();
    if (retval == nullptr)
    {
      std::unique_ptr<T> unique_ret_val = std::make_unique<T>();
      retval = unique_ret_val.release();
      m_instance_local_.store(retval, std::memory_order_release);
    }
  }
  return retval;
}

template <class T>
bool llgc::pattern::singleton::Local<T>::IsInstanceLocal()
{
  T* retval = m_instance_local_.load(std::memory_order_acquire);

  if (retval == nullptr)
  {
    std::lock_guard<std::mutex> lock(m_mutex_local_);
    retval = m_instance_local_.load(std::memory_order_relaxed);
    if (retval == nullptr)
    {
      m_instance_local_.store(retval, std::memory_order_release);
    }
  }
  return retval != nullptr;
}
