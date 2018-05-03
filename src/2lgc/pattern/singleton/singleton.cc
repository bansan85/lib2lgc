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

#include <2lgc/pattern/singleton/singleton.h>

template <class T>
bool llgc::pattern::singleton::Local<T>::IsInstance()
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_);

  return instance_ != nullptr;
}

template <class T>
std::shared_ptr<T> llgc::pattern::singleton::Local<T>::GetInstance()
{
  std::lock_guard<std::recursive_mutex> my_lock(mutex_);

  if (instance_ == nullptr)
  {
    instance_ = std::make_shared<T>();
  }

  return instance_;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
