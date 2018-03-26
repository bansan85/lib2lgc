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
std::shared_ptr<T> llgc::pattern::singleton::Static<T>::GetInstanceStatic()
{
  std::lock_guard<std::recursive_mutex> myLock(mutex_static_);

  if (instance_static_ == nullptr)
  {
    instance_static_ = std::make_shared<T>();
  }

  return instance_static_;
}

template <class T>
bool llgc::pattern::singleton::Static<T>::IsInstanceStatic()
{
  std::lock_guard<std::recursive_mutex> myLock(mutex_static_);

  return instance_static_ != nullptr;
}

template <class T>
std::shared_ptr<T> llgc::pattern::singleton::Local<T>::GetInstanceLocal()
{
  std::lock_guard<std::recursive_mutex> myLock(mutex_local_);

  if (instance_local_ == nullptr)
  {
    instance_local_ = std::make_shared<T>();
  }

  return instance_local_;
}

template <class T>
bool llgc::pattern::singleton::Local<T>::IsInstanceLocal()
{
  std::lock_guard<std::recursive_mutex> myLock(mutex_local_);

  return instance_local_ != nullptr;
}
