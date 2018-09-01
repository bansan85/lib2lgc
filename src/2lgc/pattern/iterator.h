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

/// \file iterator.h

#ifndef PATTERN_ITERATOR_H_
#define PATTERN_ITERATOR_H_

#include <2lgc/compat.h>
#include <cstddef>

namespace llgc::pattern
{
template <class T, class U>
class Iterator
{
 public:
  Iterator(const T& data, size_t pos) : data_(data), pos_(pos) {}
#ifndef SWIG
  Iterator(Iterator&& other) = default;
  Iterator(Iterator const& other) = delete;
  Iterator& operator=(Iterator&& other) = delete;
  Iterator& operator=(Iterator const& other) = delete;
#endif  // !SWIG
  virtual ~Iterator() = default;

  bool operator!=(const Iterator& other) const { return pos_ != other.pos_; }
  virtual const U& operator*() const = 0;
  const Iterator& operator++()
  {
    ++pos_;
    return *this;
  }

 protected:
  const T& data_;
  size_t pos_;
};

}  // namespace llgc::pattern

#endif  // PATTERN_ITERATOR_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
