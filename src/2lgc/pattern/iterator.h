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

/**
 * @file iterator.h
 * @brief Interface for the ranged-base loop iterator.
 */

#ifndef PATTERN_ITERATOR_H_
#define PATTERN_ITERATOR_H_

#include <2lgc/compat.h>
#include <cstddef>

namespace llgc::pattern
{
/**
 * @brief Helper class to implement range-based for loop.
 *
 * @tparam T Class that will be loop.
 * @tparam U Object that will be return by the loop.
 */
template <class T, class U>
class Iterator
{
 public:
  /**
   * @brief The constructor
   *
   * @param[in] data The data contains the list of object.
   * @param[in] pos The position of the current object.
   */
  Iterator(const T& data, size_t pos) : data_(data), pos_(pos) {}

  /**
   * @brief Default destructor.
   */
  virtual ~Iterator() = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  Iterator(Iterator&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  Iterator(Iterator const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  Iterator& operator=(Iterator&& other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  Iterator& operator=(Iterator const& other) & = delete;
#endif  // !SWIG

  /**
   * @brief Compare operator
   *
   * @param[in] other The iterator to compare with.
   *
   * @return true if different.
   */
  bool operator!=(const Iterator& other) const { return pos_ != other.pos_; }

  /**
   * @brief Dereference an iterator return the current object.
   *
   * @return Return the current object.
   */
  virtual const U& operator*() const = 0;

  /**
   * @brief Increment the current object.
   *
   * @return Himself.
   */
  const Iterator& operator++()
  {
    ++pos_;
    return *this;
  }

 protected:
  /**
   * @brief The data that contains all objects.
   */
  const T& data_;

  /**
   * @brief The position of the current object.
   */
  size_t pos_;
};

}  // namespace llgc::pattern

#endif  // PATTERN_ITERATOR_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
